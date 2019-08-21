
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Delegates/Delegate.h"
#include "HMGameMode.generated.h"


/*
	ONLY EXISTS ON SERVER. 
	Generates all class files for our project, GameState, GameSession, Controller, etc..
	Should exist only on the server when networked. GameState [not this] class handles replicated information.
	Gives us MatchState, Delay, etc.. See GameModeBase.h
*/

enum class EWaveState : uint8;

class SSTitleWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilledSig, AActor*, killer,  AActor*, victim, AController*, controlledBy);

UCLASS()
class HORDEMODE_API AHMGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:

	AHMGameMode();

	virtual void StartPlay() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnActorKilledSig OnActorKilled;

	TSharedPtr<SSTitleWidget> TitleWaveWidget;

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

	bool IsAnyPlayerAlive() const;

	void GameOver();

	void SetWaveState(EWaveState NewState);

	bool isGameOver;

	FTimerHandle TimerHandle_NextWaveStart;

	FTimerHandle TimerHandle_BotSpawner;

	int32 NumberOfBotsToSpawnInCurrentWave;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category="Wave")
	float TimeBetweenWaves;

	void RestartDeadPlayers();
};

