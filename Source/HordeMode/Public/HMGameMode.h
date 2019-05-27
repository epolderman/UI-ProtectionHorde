#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HMGameMode.generated.h"

/*
	Generates all class files for our project, GameState, GameSession, Controller, etc..
	Should exist only on the server when networked. GameState [not this] class handles replicated information.
	Gives us MatchState, Delay, etc.. See GameModeBase.h
*/

UCLASS()
class HORDEMODE_API AHMGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AHMGameMode();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="GameMode")
	void SpawnNewBot();

	// start spawning
	void StartWave();

	// stop spawning
	void EndWave();

	// set time for next start wave
	void InitNextWave();

	void SpawnBotTimerElapsed();

	void CheckWaveState();

	FTimerHandle TimerHandle_NextWaveStart;
	// easily terminated
	FTimerHandle TimerHandle_BotSpawner;

	int32 NumberOfBotsToSpawnInCurrentWave;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category="Wave")
	float TimeBetweenWaves;
};
