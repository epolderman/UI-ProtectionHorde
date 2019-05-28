#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HMGameState.generated.h"

/*
	Handles Client Game State
 */

UENUM(BlueprintType)
enum class EWaveState: uint8 {
	WaitingToStart,
	WaveStart,
	WaveInProgress,
	WaveComplete,
	GameOver,
};

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
