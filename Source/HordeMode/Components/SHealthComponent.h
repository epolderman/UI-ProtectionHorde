// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

protected:

	bool bIsDead;
	
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category="HealthComponent")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float DefaultHealth;

	UFUNCTION()
	void HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
