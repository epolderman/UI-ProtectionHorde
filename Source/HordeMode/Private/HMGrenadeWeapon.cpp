#include "HordeMode/Public/HMGrenadeWeapon.h"
#include "HordeMode/Public/HMProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/PlayerController.h>

AHMGrenadeWeapon::AHMGrenadeWeapon()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponentz"));
	RootComponent = MeshComponent;
	MuzzleSocketName = "MuzzleSocket";
}

void AHMGrenadeWeapon::Fire()
{
	AActor * GunOwner = GetOwner();
	if (GunOwner && ProjectileClass) 
	{
		
		FVector EyeLocation;
		FRotator EyeRotation;
		GunOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
	
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Owner = GunOwner;

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
			UE_LOG(LogClass, Log, TEXT("Shake N Bake"));
			controller->ClientPlayCameraShake(FireCameraShake);
		}
	}

}

