#include "HordeMode/Public/HMWeapon.h"
#include <Components/SkeletalMeshComponent.h>
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include <Components/SkeletalMeshComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include "HordeMode/HordeMode.h"
#include <PhysicalMaterials/PhysicalMaterial.h>
#include <TimerManager.h>

//Notes: Shift + Alt + R = quick rename

const int DISTANCE = 10000;

AHMWeapon::AHMWeapon()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
	BaseHitPointDamage = 20.0f;
	RateOfFire = 600.0f;
}

void AHMWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60.0f / RateOfFire;
}

void AHMWeapon::StartFire()
{
	// Note: Guarantee's delay of TimeBetwenShots between single shots, automatic fire for everything else
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimeBtwnShots,this, &AHMWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}
void AHMWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBtwnShots);
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
		QParams.bReturnPhysicalMaterial = true;
		QParams.bTraceComplex = true; // more expensive, but nails the polygon on the mesh(accurate)

		// tracer particle
		FVector TracerEndPoint = TraceEnd;

		// struct that holds our hit data, normal, etc..
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_HMWEAPON, QParams)) {
			//blocking hit! process damage
			AActor * HitActor = Hit.GetActor();
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			float damageApplied = BaseHitPointDamage;
			if (SurfaceType == SURFACE_FLESH_VULNERABLE) {
				damageApplied = 40.0f;
			}
			UGameplayStatics::ApplyPointDamage(HitActor, damageApplied, ShotDirection, Hit, Owner->GetInstigatorController(), this, DamageType);

			UParticleSystem * SelectedEffect = nullptr;
			switch (SurfaceType)
			{
			case SURFACE_FLESH_DEFAULT:
			case SURFACE_FLESH_VULNERABLE:
				SelectedEffect = FleshImpactEffect;
				break;
			default:
				SelectedEffect = DefaultImpactEffect;
				break;
			}

			if (SelectedEffect) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			TracerEndPoint = Hit.ImpactPoint;
			
		}

		PlayerWeaponFireEffects(TracerEndPoint);

		LastFireTime = GetWorld()->TimeSeconds;
	}
}

void AHMWeapon::PlayerWeaponFireEffects(FVector &TracerEndPoint) {

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

	APawn * myOwner = Cast<APawn>(GetOwner());
	AActor * GunOwner = GetOwner();
	if (myOwner) {
		APlayerController * controller = Cast<APlayerController>(myOwner->GetController());
		if (controller && FireCameraShake) {
			controller->ClientPlayCameraShake(FireCameraShake);
		}
	}

}
