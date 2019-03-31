// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeMode/Public/HMProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SphereComponent.h>
#include <DrawDebugHelpers.h>
#include <Particles/ParticleSystem.h>
#include <Particles/ParticleSystemComponent.h>


// Sets default values
AHMProjectile::AHMProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AHMProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AHMProjectile::OnExplode() {
	if (ExplodeEffect) {
		// DrawDebugSphere(GetWorld(), GetActorLocation(), 10.0f, 100, FColor::Red, false, 5.0f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorLocation());
	}
}

void AHMProjectile::LifeSpanExpired()
{
	UE_LOG(LogClass, Log, TEXT("lifespanexpired"));
	OnExplode();
	Super::LifeSpanExpired();
}

/* TODO: When grenade hits an actor -> explode. Fix this function*/
void AHMProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OnExplode();
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	MakeNoise(1.0f, Instigator);
	
	Destroy();
}

