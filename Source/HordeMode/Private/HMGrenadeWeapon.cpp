#include "HordeMode/Public/HMGrenadeWeapon.h"
#include "HordeMode/Public/HMProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/PlayerController.h>

/* Todo: Network the projectile, redo the reticle, update the projectile reticle */

AHMGrenadeWeapon::AHMGrenadeWeapon()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponentz"));
	RootComponent = MeshComponent;
	MuzzleSocketName = "MuzzleSocket";
	// spawn on a server, spawn it on clients, sets up actor channel
	SetReplicates(true);
}

void AHMGrenadeWeapon::Fire()
{
	AActor * WeaponOwner = GetOwner();
	if (WeaponOwner && ProjectileClass) 
	{
		
		FVector EyeLocation;
		FRotator EyeRotation;
		WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
	
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Owner = WeaponOwner;

		GetWorld()->SpawnActor<AHMProjectile>(ProjectileClass, MuzzleLocation, EyeRotation, ActorSpawnParams);

		if (MuzzleEffect) {
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
		}
		
		PlayerWeaponFireEffects(EyeLocation);
	}
}

void AHMGrenadeWeapon::PlayerWeaponFireEffects(FVector &TracerEndPoint) {

	APawn * myOwner = Cast<APawn>(GetOwner());
	if (myOwner) {
		APlayerController * controller = Cast<APlayerController>(myOwner->GetController());
		if (controller && FireCameraShake) {
			controller->ClientPlayCameraShake(FireCameraShake);
		}
	}

}

