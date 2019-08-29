

#include "HMGameState.h"
#include <UnrealNetwork.h>

void AHMGameState::SetWaveState(EWaveState NewWaveState)
{
	if (Role == ROLE_Authority) {
		EWaveState OldState = CurrentGameState;
		CurrentGameState = NewWaveState;
		OnRep_WaveState(OldState);
		
	}

	WaveNumber = 0;
}

void AHMGameState::OnRep_WaveState(EWaveState OldState)
{
	if (CurrentGameState == EWaveState::WaveStart) {
		UE_LOG(LogTemp, Warning, TEXT("GameState: WaveNumber"));
		WaveNumber++;
	}
	WaveStateChanged(CurrentGameState, OldState, WaveNumber);
}	

void AHMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHMGameState, CurrentGameState);
}
