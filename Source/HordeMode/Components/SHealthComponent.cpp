// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"
#include <GameFramework/Actor.h>
#include <UnrealMathUtility.h>


USHealthComponent::USHealthComponent()
{
	DefaultHealth = 100.0f;
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor * myOwner = GetOwner();
	if (myOwner) {

		myOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleDamage);

	}

	Health = DefaultHealth;
}

void USHealthComponent::HandleDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f) {
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}
