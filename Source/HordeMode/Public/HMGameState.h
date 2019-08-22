#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HMGameState.generated.h"

/*
	Handles Client Game State and replication. 
	Game Mode doesnt exist on the clients. 
	Game State can hold the state for the clients. 
 */

UENUM(BlueprintType)
enum class EWaveState: uint8 {
	WaitingToStart,
	WaveStart,
	WaveInProgress,
	WaveComplete,
	GameOver,
};

// on rep gives use the ability of the old state, for example: old weapon, we can call unequip, new weapon, equip;

UCLASS()
class HORDEMODE_API AHMGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	void SetWaveState(EWaveState NewWaveState);
protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
	EWaveState CurrentGameState;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
	void WaveStateChanged(EWaveState NewState, EWaveState OldState);

	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);
	
};
