

#include "HordeMode/Public/HMGrenadeWeapon.h"
#include "HordeMode/Public/HMProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

AHMGrenadeWeapon::AHMGrenadeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	// TODO: figure out inheriting mesh components from a parent, find out the build error!
	// MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	// RootComponent = MeshComponent;
	MuzzleSocketName = "MuzzleSocket";
}


void AHMGrenadeWeapon::Fire()
{
	// try and fire a projectile
	if (ProjectileClass)
	{
		FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = MeshComponent->GetSocketRotation(MuzzleSocketName);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		// ActorSpawnParams.Instigator = this;
		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AHMProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}
}
