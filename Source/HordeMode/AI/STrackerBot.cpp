// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include <NavigationSystem/Public/NavigationPath.h>
#include <NavigationSystem/Public/NavigationSystem.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>
#include <DrawDebugHelpers.h>
#include "HordeMode/Components/SHealthComponent.h"
#include <Particles/ParticleSystemComponent.h>
#include "Public/HMCharacter.h"
#include <GameFramework/Actor.h>
#include <Sound/SoundCue.h>
#include <PhysicalMaterials/PhysicalMaterial.h>
#include <Materials/MaterialInterface.h>


ASTrackerBot::ASTrackerBot()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent")); 
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(explosionRadius);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	bUseVelocityChange = false;
	moveMentForce = 1000.0f;
	requiredDistanceToTarget = 100.0f;
	isDead = false;
	explosionRadius = 300.0f;
	damageAmount = 60.0f;;
	bisStartedSelfDestruct = false;
	maxDamageLevel = 100.0f;
	Timer_Damage_Interval = 0.25f;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	if(Role == ROLE_Authority)
	NextPathVector = getNextLocation();

	HealthComponent->OnHealthChanged.AddDynamic(this, &ASTrackerBot::OnTakeDamage);
}

void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority && !isDead) {
		const float distanceToTarget = (GetActorLocation() - NextPathVector).Size();

		if (distanceToTarget <= requiredDistanceToTarget) {
			NextPathVector = getNextLocation();
		}
		else {
			FVector forceDirection = NextPathVector - GetActorLocation();
			forceDirection.Normalize(); //get direction
			forceDirection *= moveMentForce; //scale
			MeshComponent->AddForce(forceDirection, NAME_None, bUseVelocityChange);
			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + forceDirection, 32, FColor::Green, false, 0.0);
		}


		DrawDebugSphere(GetWorld(), NextPathVector, 20, 12, FColor::Yellow, false, 0.0f, 1.0f);
	}

	
}

void ASTrackerBot::SelfDestruct()
{
	isDead = true;

	if(ExplodeEffect != nullptr)
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorLocation(), GetActorRotation());

	if (ExplosionSound != nullptr)
	UGameplayStatics::SpawnSoundAttached(ExplosionSound, RootComponent);

	MeshComponent->SetVisibility(false, true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Role = ROLE_Authority) {
		TArray<AActor *> IgnoredActors;
		IgnoredActors.Add(this);

		UGameplayStatics::ApplyRadialDamage(this, damageAmount, GetActorLocation(), explosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

		DrawDebugSphere(GetWorld(), GetActorLocation(), explosionRadius, 10, FColor::Green, false, 2.0f, 0, 1.0f);

		SetLifeSpan(2.0f);
		//Destroy();
	}
}

// check for nearby other instances
void ASTrackerBot::NotifyActorBeginOverlap(AActor * otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	ASTrackerBot * otherBot = Cast<ASTrackerBot>(otherActor);
	if (otherBot != nullptr) {

		if (currentMaterialOnMesh == nullptr)
			currentMaterialOnMesh = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));

		if (currentMaterialOnMesh != nullptr)
			currentMaterialOnMesh->SetScalarParameterValue("PowerLevelAlpha", damageAmount / maxDamageLevel);

		damageAmount += 10.0f;
	}

	if (!bisStartedSelfDestruct && !isDead) {
	

	AHMCharacter * PlayerPawn = Cast<AHMCharacter>(otherActor);
	if (PlayerPawn != nullptr && !USHealthComponent::IsFriendly(this, otherActor)) {

		if (Role == ROLE_Authority)
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ASTrackerBot::DamageSelf, Timer_Damage_Interval, true, 0.0f);

		bisStartedSelfDestruct = true;
		if (SelfDestructSound != nullptr)
			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
	}
	}
}

// check for nearby other instances
void ASTrackerBot::NotifyActorEndOverlap(AActor * otherActor)
{
	ASTrackerBot * otherBot = Cast<ASTrackerBot>(otherActor);
	if (otherBot != nullptr) {
		damageAmount -= 10.0f;

		// todo:: clean up
		if (currentMaterialOnMesh == nullptr)
			currentMaterialOnMesh = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));

		if (currentMaterialOnMesh != nullptr)
			currentMaterialOnMesh->SetScalarParameterValue("PowerLevelAlpha", damageAmount / maxDamageLevel);
	}
	
}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20.0f, GetInstigatorController(), this, nullptr);
}

void ASTrackerBot::RefreshPath()
{
	NextPathVector = getNextLocation();
}

FVector ASTrackerBot::getNextLocation()
{
	
	AActor * closestTargetActor = nullptr;
	// biggest float value
	float lastDistanceChecked = FLT_MAX;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn * testPawn = It->Get();

		if (testPawn == nullptr || USHealthComponent::IsFriendly(testPawn, this)) {
			continue; // skips rest of iteration, remaining for loop
		}

		USHealthComponent * HealthComp = Cast<USHealthComponent>(testPawn->GetComponentByClass(USHealthComponent::StaticClass()));

		if (HealthComp != nullptr && HealthComp->GetHealth() > 0.0f) {

			float distance = (testPawn->GetActorLocation() - this->GetActorLocation()).Size();
			if (distance < lastDistanceChecked) {
				closestTargetActor = testPawn;
				lastDistanceChecked = distance;
			}
		
		}
	
	}
	

	if (closestTargetActor) {
	
		
		UNavigationPath * navPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), closestTargetActor);
		
		GetWorldTimerManager().ClearTimer(FindClosestPlayerTimerSearch);
		GetWorldTimerManager().SetTimer(FindClosestPlayerTimerSearch, this, &ASTrackerBot::RefreshPath, 5.0f, false);
		
		if (navPath && navPath->PathPoints.Num() > 1) {
			// current is at index 0, next is at 1
			return navPath->PathPoints[1];
		}
	}

	return GetActorLocation();
}

void ASTrackerBot::OnTakeDamage(USHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// dynamic instance at runtime, in case of multiple instances
	if(currentMaterialOnMesh == nullptr)
	currentMaterialOnMesh = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));

	if (currentMaterialOnMesh != nullptr)
	currentMaterialOnMesh->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);

	if (Health <= 0.0f && !isDead)
		SelfDestruct();
}

