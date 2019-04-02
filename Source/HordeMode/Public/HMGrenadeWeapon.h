// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../HordeMode/Public/HMWeapon.h"
#include "HMGrenadeWeapon.generated.h"

class AHMProjectile;


UCLASS()
class HORDEMODE_API AHMGrenadeWeapon : public AHMWeapon
{
	GENERATED_BODY()

public:
	AHMGrenadeWeapon();
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AHMProjectile> ProjectileClass;
protected:

	virtual void Fire() override;
};
