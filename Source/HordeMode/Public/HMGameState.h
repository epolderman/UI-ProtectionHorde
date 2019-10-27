#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HMGameState.generated.h"

/*
	Handles Client Game State and replication. 
	Game Mode doesnt exist on the clients. 
	Game State can hold the state for the clients

	Set Up Replicated Information In this class

	Rep gives use the ability of the old state, for example: 
	old weapon, we can call unequip, new weapon, equip
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
	AHMGameState();

	void SetWaveState(EWaveState NewWaveState);

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
	EWaveState CurrentGameState;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
	void WaveStateChanged(EWaveState NewState, EWaveState OldState, int WaveNumber);

	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);

	int32 WaveNumber;
};

/*
	onRep (Persisted Change -> Clients) 
	vs 
	MultiCast (Need to happen right now, if actors are not around, changes will not be seen)
	https://forums.unrealengine.com/development-discussion/c-gameplay-programming/25318-onrep-vs-multicast
*/
