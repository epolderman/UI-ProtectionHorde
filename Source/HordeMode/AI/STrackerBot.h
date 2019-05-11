// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class UStaticMeshComponent;
class USHealthComponent;

UCLASS()
class HORDEMODE_API ASTrackerBot : public APawn
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Display")
		UStaticMeshComponent * MeshComponent;

	virtual void BeginPlay() override;

	FVector getNextLocation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent * HealthComponent;

	UFUNCTION()
	void OnTakeDamage(USHealthComponent* HealthComp, float Health, float HealthDelta,
		const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
public:	
	ASTrackerBot();

	UPROPERTY(VisibleAnywhere, Category="AI")
	FVector NextPathVector;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float moveMentForce;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float requiredDistanceToTarget;
};
