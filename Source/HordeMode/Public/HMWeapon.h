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

	virtual void PlayerWeaponFireEffects(FVector &TracerEndPoint);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();
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
	UParticleSystem * ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem * BulletTracer;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCameraShake;
};
