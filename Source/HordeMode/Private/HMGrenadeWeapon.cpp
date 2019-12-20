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
	SetReplicates(true);
}

void AHMGrenadeWeapon::Fire()
{
	AActor * WeaponOwner = GetOwner();
	if (WeaponOwner && ProjectileClass) 
	{

		if (Role < ROLE_Authority) {
			ServerFire();
			return;
		}
		
		FVector EyeLocation;
		FRotator EyeRotation;
		WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
	
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ActorSpawnParams.Owner = WeaponOwner;

		GetWorld()->SpawnActor<AHMProjectile>(ProjectileClass, MuzzleLocation, EyeRotation, ActorSpawnParams);

		if (MuzzleEffect)
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
		
		PlayerWeaponFireEffects(EyeLocation);
	}
}


void AHMGrenadeWeapon::PlayerWeaponFireEffects(FVector &TracerEndPoint) {

	APawn * PawnOwner = Cast<APawn>(GetOwner());

	if (PawnOwner) {

		APlayerController * PlayerController = Cast<APlayerController>(PawnOwner->GetController());

		if (PlayerController && FireCameraShake)
		PlayerController->ClientPlayCameraShake(FireCameraShake);
	}

}

