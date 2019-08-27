

#include "HMGameState.h"
#include <UnrealNetwork.h>

void AHMGameState::SetWaveState(EWaveState NewWaveState)
{
	if (Role == ROLE_Authority) {

		EWaveState OldState = CurrentGameState;

		CurrentGameState = NewWaveState;

		OnRep_WaveState(OldState);
	}
	
}

void AHMGameState::OnRep_WaveState(EWaveState OldState)
{
	WaveStateChanged(CurrentGameState, OldState);
}	

void AHMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHMGameState, CurrentGameState);
}
