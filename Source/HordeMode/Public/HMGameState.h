#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HMGameState.generated.h"

/*
	Handles Client Game State and replication. 
	Game Mode doesnt exist on the clients. 
	Game State can hold the state for the clients

	Set Up Replicated Information In this class
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

	AHMGameState();

	void SetWaveState(EWaveState NewWaveState);

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
	EWaveState CurrentGameState;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
	void WaveStateChanged(EWaveState NewState, EWaveState OldState, int WaveNumber);

	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);

	int WaveNumber;
};

/*
		ReplicatedUsing = OnRep_WaveState = Setting WaveState var will trigger on the clients
		but not on server. Same mechanic does not work on server, we need to check if
		authority -> then manually call onRep change. 

		// on rep vs multicast
		https://forums.unrealengine.com/development-discussion/c-gameplay-programming/25318-onrep-vs-multicast
*/
