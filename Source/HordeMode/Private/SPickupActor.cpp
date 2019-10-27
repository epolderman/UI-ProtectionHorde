
#include "HordeMode/Public/SPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include <Engine/World.h>
#include "HordeMode/Public/HMPowerUpActor.h"
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
ASPickupActor::ASPickupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	// thickness
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);
	UE_LOG(LogTemp, Warning, TEXT("SPickupActor()"));
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	Respawn();
}

void ASPickupActor::Respawn()
{
	UE_LOG(LogTemp, Warning, TEXT("<------------Respawn()------------->"));

	if (PowerUpClass == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Warning for Power Up: Please assign PowerUpClass instance."))
		return;
	}
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentPowerUp = GetWorld()->SpawnActor<AHMPowerUpActor>(PowerUpClass, GetTransform(), params);
}


// Function Only Run On Server = Only Called on Server
void ASPickupActor::NotifyActorBeginOverlap(AActor * otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	if (Role == ROLE_Authority && CurrentPowerUp != nullptr) {
		CurrentPowerUp->ActivatePowerUp(otherActor);
		CurrentPowerUp = nullptr;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickupActor::Respawn, coolDownDuration);
	}
}

