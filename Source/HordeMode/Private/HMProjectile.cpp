#include "HordeMode/Public/HMProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SphereComponent.h>
#include <DrawDebugHelpers.h>
#include <Particles/ParticleSystem.h>
#include <Particles/ParticleSystemComponent.h>


AHMProjectile::AHMProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AHMProjectile::OnHit);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	RootComponent = CollisionComp;
	LifeSpan = 1.0f;
	LaunchVelocity = 350.0f;
}

void AHMProjectile::BeginPlay() {
	SetLifeSpan(LifeSpan);
	const AActor * GunOwner = GetOwner();
	if (GunOwner ) {
		FVector EyeLocation;
		FRotator EyeRotation;
		GunOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		FVector ShotDirection = EyeRotation.Vector();
		CollisionComp->AddImpulseAtLocation(ShotDirection * LaunchVelocity, GetActorLocation());
	}
}

void AHMProjectile::OnExplode() {
	if (ExplodeEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorLocation(), GetActorRotation());
	}
}

void AHMProjectile::LifeSpanExpired()
{
	OnExplode();
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 3.0f, GetActorLocation(), 3.0f, DamageType, TArray<AActor*>());
	Super::LifeSpanExpired();
}

void AHMProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogClass, Log, TEXT("OnHit() onhit onhit"));
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		OnExplode();
		Destroy();
	}
}

