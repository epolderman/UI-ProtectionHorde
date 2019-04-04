#include "HordeMode/Public/HMGrenadeWeapon.h"
#include "HordeMode/Public/HMProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

AHMGrenadeWeapon::AHMGrenadeWeapon()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponentz"));
	RootComponent = MeshComponent;
	MuzzleSocketName = "MuzzleSocket";
}


void AHMGrenadeWeapon::Fire()
{
	AActor * Owner = GetOwner();
	if (Owner && ProjectileClass) 
	{
		
		FVector EyeLocation;
		FRotator EyeRotation;
		Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
	
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		GetWorld()->SpawnActor<AHMProjectile>(ProjectileClass, MuzzleLocation, EyeRotation, ActorSpawnParams);
	}
}
