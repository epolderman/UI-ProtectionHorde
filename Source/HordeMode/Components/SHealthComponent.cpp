// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"
#include <GameFramework/Actor.h>
#include <UnrealMathUtility.h>
#include "HMGameMode.h"
#include <UnrealNetwork.h>


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

	// health should be dictated on the server only
	if (GetOwnerRole() == ROLE_Authority) {
		AActor * MyOwner = GetOwner();
		if (MyOwner) {
			// Rifle Damage = onTakePointDamage delegate -> HandleDamageHit -> Blueprint / C++ Update UI
			MyOwner->OnTakePointDamage.AddDynamic(this, &USHealthComponent::HandleDamageHit);
			// Grenade Damage
			MyOwner->OnTakeRadialDamage.AddDynamic(this, &USHealthComponent::HandleRadialDamage);
		
		}
	}
	Health = DefaultHealth;
}

// on rep function trick, gets prev value
void USHealthComponent::OnRep_Health(float LastHealthValue)
{
	float Damage = Health - LastHealthValue;
	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}


void USHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || Health <= 0.0f)
	return;
	
	// clamp = value, min, max
	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);
	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
}

void USHealthComponent::HandleRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	return;

	AController * InstigatorController = DamageCauser && DamageCauser->GetOwner() != nullptr ? DamageCauser->GetOwner()->GetInstigatorController() : nullptr;

	if (DamageCauser && DamageCauser != DamagedActor && IsFriendly(DamagedActor, DamageCauser->GetOwner()))
	return;

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	bIsDead = Health <= 0.0f;
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	/* Call will only succeed on the server */
	AHMGameMode * CurrentGameMode = Cast<AHMGameMode>(GetWorld()->GetAuthGameMode());

	// Projectile -> Grenade Weapon -> Actor Owner
	if (bIsDead && CurrentGameMode && InstigatorController) 
	CurrentGameMode->OnActorKilled.Broadcast(DamageCauser, GetOwner(), InstigatorController);
}

void USHealthComponent::HandleDamageHit(AActor * DamagedActor, float Damage, AController * InstigatedBy, FVector HitLocation, UPrimitiveComponent * FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType * DamageType, AActor * DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	return;

	if (DamageCauser != DamagedActor && IsFriendly(DamagedActor, DamageCauser))
	return;

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
	
	/* Call will only succeed on the server */
	AHMGameMode * CurrentGameMode = Cast<AHMGameMode>(GetWorld()->GetAuthGameMode());

	if (CurrentGameMode)
		CurrentGameMode->OnHitEvent.Broadcast(HitLocation, ShotFromDirection, Damage, GetOwner());

	if (bIsDead && CurrentGameMode)
		CurrentGameMode->OnActorKilled.Broadcast(DamageCauser, GetOwner(), InstigatedBy);
}


void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USHealthComponent, Health);
}


