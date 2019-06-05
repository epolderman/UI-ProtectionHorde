// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"
#include <GameFramework/Actor.h>
#include <UnrealMathUtility.h>
#include "HMGameMode.h"



USHealthComponent::USHealthComponent()
{
	DefaultHealth = 100.0f;
	bIsDead = false;
}

float USHealthComponent::GetHealth() const
{
	return Health;
}

void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor * myOwner = GetOwner();
	if (myOwner) {
		// binding a delegate on the OnTakeAnyDamage delegate => which then broadcasts another delegate
		myOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleDamage);
	}
	Health = DefaultHealth;
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
	if (Damage <= 0.0f || bIsDead) {
		return;
	}

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
