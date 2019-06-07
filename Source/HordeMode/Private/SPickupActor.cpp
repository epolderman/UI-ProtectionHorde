
#include "HordeMode/Public/SPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include <Engine/World.h>
#include "HordeMode/Public/HMPowerUpActor.h"

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


}

// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();

	Respawn();
}

void ASPickupActor::Respawn()
{
	if (PowerUpClass == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Warning for Power Up: Please assign PowerUpClass instance."))
		return;
	}

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentPowerUp = GetWorld()->SpawnActor<AHMPowerUpActor>(PowerUpClass, GetTransform(), params);
}

void ASPickupActor::NotifyActorBeginOverlap(AActor * otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);
	
	if (CurrentPowerUp) {

		CurrentPowerUp->ActivatePowerUp();
		CurrentPowerUp = nullptr;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickupActor::Respawn, coolDownDuration);
	}
}
