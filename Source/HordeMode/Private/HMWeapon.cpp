#include "HordeMode/Public/HMWeapon.h"
#include <Components/SkeletalMeshComponent.h>
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include <Components/SkeletalMeshComponent.h>
#include <Particles/ParticleSystemComponent.h>


const int DISTANCE = 10000;

AHMWeapon::AHMWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
}

void AHMWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AHMWeapon::Fire()
{	
	AActor * Owner = GetOwner();
	if (Owner) {
		FVector EyeLocation;
		FRotator EyeRotation;
		Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		FVector TraceEnd = EyeLocation + (ShotDirection * DISTANCE);
		FCollisionQueryParams QParams;
		QParams.AddIgnoredActor(Owner);
		QParams.AddIgnoredActor(this);
		QParams.bTraceComplex = true; // more expensive, but nails the polygon on the mesh(accurate)

		// tracer particle
		FVector TracerEndPoint = TraceEnd;

		// struct that holds our hit data, normal, etc..
		// ECC_Visibility = channel to check our trace, anything that steps in its way, will collide
		FHitResult Hit;
		if(GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QParams)){
			//blocking hit! process damage
			AActor * HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, 1.0f, ShotDirection, Hit, Owner->GetInstigatorController(), this, DamageType);
			if (ImpactEffect) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			TracerEndPoint = Hit.ImpactPoint;
			
		}
	
		//Fname == lookups, FText = localizable
		if (MuzzleEffect) {
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
		}
		if (BulletTracer) {
			FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
			UParticleSystemComponent * TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletTracer, MuzzleLocation);
			if (TracerComp) {
				TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
			}
		}
		
	}

	
}

void AHMWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

