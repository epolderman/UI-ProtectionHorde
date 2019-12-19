// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Engine/EngineTypes.h>
#include "SHealthComponent.generated.h"

// OnHealthChanged Event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, USHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(HordeMode), meta=(BlueprintSpawnableComponent) )
class HORDEMODE_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USHealthComponent();

	UFUNCTION(BlueprintCallable, Category="Heal")
	void Heal(float HealAmount);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

	float GetHealth() const;

	// less memory 0 - 255 \ doesn't need to be replicated, default team we spawn with
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Team")
	uint8 TeamNumber;

	// no white execution pin = blueprint Pure
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="HealthComponent")
	static bool IsFriendly(AActor * left, AActor * right);
protected:

	bool bIsDead;
	
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="HealthComponent")
	float Health;

	UFUNCTION()
	void OnRep_Health(float LastHealthValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float DefaultHealth;

	UFUNCTION()
	void HandleRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);
	 
	UFUNCTION()
	void HandleDamageHit(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent,
			FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
};