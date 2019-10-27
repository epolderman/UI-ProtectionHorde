

#include "HMGameState.h"
#include <UnrealNetwork.h>
#include "HMPlayerState.h"


AHMGameState::AHMGameState() {

	WaveNumber = 0;
}
void AHMGameState::SetWaveState(EWaveState NewWaveState)
{
	// Server Change -> Imperatively Call OnRep
	if (Role == ROLE_Authority) {
		EWaveState OldState = CurrentGameState;
		CurrentGameState = NewWaveState;
		OnRep_WaveState(OldState);
	}

}

void AHMGameState::OnRep_WaveState(EWaveState OldState)
{
	if (CurrentGameState == EWaveState::WaveStart) {
		++WaveNumber;
		WaveStateChanged(CurrentGameState, OldState, WaveNumber);
	}
}

void AHMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHMGameState, CurrentGameState);
}
