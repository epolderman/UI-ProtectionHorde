

#include "HMGameState.h"
#include <UnrealNetwork.h>

void AHMGameState::OnRep_WaveState(EWaveState OldState)
{
	WaveStateChanged(CurrentGameState,OldState);
}	

void AHMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHMGameState, CurrentGameState);
}
