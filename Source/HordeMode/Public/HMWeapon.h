#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <EnumAsByte.h>
#include "HMWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UCameraShake;

// Container for information hit scan weapon line trace
// Vectors are less precise, less data over the network, "NetQuantize"

USTRUCT()
struct FHitScanTrace {

	GENERATED_BODY()

public:
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceEnd;
};

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

	// push request to the hosting server, withValidation = ...
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	//todo: clean up inheritance with grenade weapon and replication
	virtual void Fire();

	FTimerHandle TimerHandle_TimeBtwnShots;

	float LastFireTime;

	/* RPM = Bullets Per Minute */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float RateOfFire;

	float TimeBetweenShots;

	virtual void BeginPlay() override;

	// trigger a function every time this property gets replicated
	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScan;

	UFUNCTION()
	void OnRep_HitScanTrace();

	void PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint);
};
