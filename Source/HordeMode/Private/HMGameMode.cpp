#include "HordeMode/Public/HMGameMode.h"

AHMGameMode::AHMGameMode()
{
	TimeBetweenWaves = 2.0f;
}

void AHMGameMode::StartPlay()
{
	Super::StartPlay();

	InitNextWave();
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

	InitNextWave();
}

void AHMGameMode::InitNextWave()
{
	FTimerHandle TimerHandle_NextWaveStart;
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AHMGameMode::StartWave, TimeBetweenWaves, false);
}

void AHMGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NumberOfBotsToSpawnInCurrentWave--;

	if (NumberOfBotsToSpawnInCurrentWave <= 0)
	EndWave();
}
