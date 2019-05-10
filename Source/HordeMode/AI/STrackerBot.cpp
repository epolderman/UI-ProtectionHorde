// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include <NavigationSystem/Public/NavigationPath.h>
#include <NavigationSystem/Public/NavigationSystem.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>


ASTrackerBot::ASTrackerBot()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;
	bUseVelocityChange = false;
	moveMentForce = 1000.0f;
	requiredDistanceToTarget = 100.0f;
}

void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float distanceToTarget = (GetActorLocation() - NextPathVector).Size();

	if (distanceToTarget <= requiredDistanceToTarget) {
		NextPathVector = getNextLocation();
	}
	else {
		FVector forceDirection = NextPathVector - GetActorLocation();
		forceDirection.Normalize(); //get direction
		forceDirection *= moveMentForce; //scale
		MeshComponent->AddForce(forceDirection, NAME_None, bUseVelocityChange);
	}
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	NextPathVector = getNextLocation();
}

FVector ASTrackerBot::getNextLocation()
{
	// hack to get player location, won't work in multi player
	ACharacter * playerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath * navPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), playerPawn);

	if (navPath->PathPoints.Num() > 1) {
		// current is at index 0, next is at 1
		return navPath->PathPoints[1];
	}

	return GetActorLocation();
}


