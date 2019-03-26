// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeMode/Public/HMWeapon.h"
#include <Components/SkeletalMeshComponent.h>
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include <Components/SkeletalMeshComponent.h>
#include <Particles/ParticleSystemComponent.h>


const int DISTANCE = 10000;

// Sets default values
AHMWeapon::AHMWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
}

// Called when the game starts or when spawned
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
		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
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

// Called every frame
void AHMWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

