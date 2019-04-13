#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HMWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UCameraShake;

UCLASS()
class HORDEMODE_API AHMWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AHMWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void StopFire();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent * MeshComponent;

	// TODO: Investigate Damage System
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem * MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem * DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem * FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem * BulletTracer;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float BaseHitPointDamage;

	virtual void PlayerWeaponFireEffects(FVector &TracerEndPoint);

	virtual void Fire();

	FTimerHandle TimerHandle_TimeBtwnShots;

	float LastFireTime;

	/* RPM = Bullets Per Minute */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float RateOfFire;

	float TimeBetweenShots;

	virtual void BeginPlay() override;
};
