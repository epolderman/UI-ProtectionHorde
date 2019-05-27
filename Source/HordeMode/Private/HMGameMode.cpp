#include "HordeMode/Public/HMGameMode.h"
#include "Components/SHealthComponent.h"
#include <TimerManager.h>
#include <GameFramework/Actor.h>
#include <Engine/World.h>


// Tick Runs 60 times per sec / 1 for each frame

AHMGameMode::AHMGameMode()
{
	TimeBetweenWaves = 2.0f;

	PrimaryActorTick.bCanEverTick = true;

	// once a second
	PrimaryActorTick.TickInterval = 1.0f;;
}

void AHMGameMode::StartPlay()
{
	Super::StartPlay();

	InitNextWave();
}

void AHMGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
}

void AHMGameMode::StartWave()
{
	WaveCount++;

	NumberOfBotsToSpawnInCurrentWave = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &AHMGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);
}

void AHMGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
}

void AHMGameMode::InitNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AHMGameMode::StartWave, TimeBetweenWaves, false);
}

void AHMGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NumberOfBotsToSpawnInCurrentWave--;

	if (NumberOfBotsToSpawnInCurrentWave <= 0)
	EndWave();
}

void AHMGameMode::CheckWaveState()
{
	bool isPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
	if (NumberOfBotsToSpawnInCurrentWave > 0 || isPreparingForWave) {
		return;
	}

	bool bisAnyBotAlive = false;

	// check if any bots are alive
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn * testPawn = It->Get();

		if (testPawn == nullptr || testPawn->IsPlayerControlled()) {
			continue; // skips rest of iteration, remaining for loop
		}
		
		USHealthComponent * HealthComp = Cast<USHealthComponent>(testPawn->GetComponentByClass(USHealthComponent::StaticClass()));

		if (HealthComp != nullptr && HealthComp->GetHealth() > 0.0f) {
			bisAnyBotAlive = true;
			break;
		}
	}

	if (!bisAnyBotAlive) {
		InitNextWave();
	}
	

}
