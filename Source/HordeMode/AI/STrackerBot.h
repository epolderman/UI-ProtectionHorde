// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class UStaticMeshComponent;
class USHealthComponent;
class UMaterialInstanceDynamic;
class UParticleSystem;
class USphereComponent;
class USoundCue;


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

	// dynamic material on damage
	UMaterialInstanceDynamic * currentMaterialOnMesh;

	UFUNCTION()
	void OnTakeDamage(USHealthComponent* HealthComp, float Health, float HealthDelta,
		const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent * SphereComponent;

	FTimerHandle TimerHandle;

	FTimerHandle FindClosestPlayerTimerSearch;

	void DamageSelf();

	bool bisStartedSelfDestruct;

	void RefreshPath();
public:	
	ASTrackerBot();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	float maxDamageLevel;

	UPROPERTY(VisibleAnywhere, Category="AI")
	FVector NextPathVector;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float moveMentForce;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float requiredDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem * ExplodeEffect;

	void SelfDestruct();
	
	bool isDead;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	float explosionRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	float damageAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	USoundCue * SelfDestructSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	USoundCue * ExplosionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	float Timer_Damage_Interval; 

	virtual void NotifyActorBeginOverlap(AActor* otherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
