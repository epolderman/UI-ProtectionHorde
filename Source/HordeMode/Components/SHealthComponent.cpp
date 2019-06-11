// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"
#include <GameFramework/Actor.h>
#include <UnrealMathUtility.h>
#include "HMGameMode.h"
#include <UnrealNetwork.h>

// todo: Network grenade weapon, look into inheritance hierarchy via base class weapon
// todo: look into weapon replicated breaking UI widgets on the clients
// todo: do explosive barrel challenge networking
// todo: bug in tracker bot networking on clients

USHealthComponent::USHealthComponent()
{
	DefaultHealth = 100.0f;
	bIsDead = false;
	TeamNumber = 255;

	SetIsReplicated(true);
}

float USHealthComponent::GetHealth() const
{
	return Health;
}

bool USHealthComponent::IsFriendly(AActor * LeftActor, AActor * RightActor)
{
	if (LeftActor == nullptr || RightActor == nullptr)
		return false;

	USHealthComponent * HealthLeft = Cast<USHealthComponent>(LeftActor->GetComponentByClass(USHealthComponent::StaticClass()));
	USHealthComponent * HealthRight = Cast<USHealthComponent>(RightActor->GetComponentByClass(USHealthComponent::StaticClass()));

	if (HealthLeft == nullptr || HealthRight == nullptr)
		return true;

	if (HealthLeft->TeamNumber == HealthRight->TeamNumber)
		return true;

	return false;
}

void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// health should be dictated only on the server
	if (GetOwnerRole() == ROLE_Authority) {

		AActor * myOwner = GetOwner();
		if (myOwner) {
			// binding a delegate on the OnTakeAnyDamage delegate => which then broadcasts another delegate
			myOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleDamage);
		}
	}
	Health = DefaultHealth;
}

// on rep function trick, gets prev value
void USHealthComponent::OnRep_Health(float oldHealth)
{
	float damage = Health = oldHealth;
	OnHealthChanged.Broadcast(this, Health, damage, nullptr, nullptr, nullptr);
}

// todo: breakdown this functionality and see why it works under the hood? optimization?
void USHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || Health <= 0.0f) {
		return;
	}
	// clamp = value, min, max
	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);
	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
}

void USHealthComponent::HandleDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	return;

	if (DamageCauser != DamagedActor && IsFriendly(DamagedActor, DamageCauser))
	return;

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	bIsDead = Health <= 0.0f;
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead) {
		
		// only valid on server
		AHMGameMode * CurrentGameMode = Cast<AHMGameMode>(GetWorld()->GetAuthGameMode());
		if (CurrentGameMode) {
			CurrentGameMode->OnActorKilled.Broadcast(DamageCauser, GetOwner(), InstigatedBy);
		}
	}
}
void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, Health);
}


