

#include "HordeMode/Public/HMGrenadeWeapon.h"
#include "HordeMode/Public/HMProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

AHMGrenadeWeapon::AHMGrenadeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	// Note: figure out inheriting mesh components from a parent, find out the build error!
	// Why does it need a different FName for the mesh component
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponentz"));
	RootComponent = MeshComponent;
	MuzzleSocketName = "MuzzleSocket";
}


void AHMGrenadeWeapon::Fire()
{
	UE_LOG(LogClass, Log, TEXT("Fire()"));

	AActor * Owner = GetOwner();
	if (Owner && ProjectileClass) 
	{
		UE_LOG(LogClass, Log, TEXT("Inside Owner Fire()"));
		FVector EyeLocation;
		FRotator EyeRotation;
		Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
	

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, ActorSpawnParams);
	}
}
