

#include "HMGameState.h"
#include <UnrealNetwork.h>

// todo: bug exists that this wave number is not replicated to clients. 
AHMGameState::AHMGameState() {

	WaveNumber = 0;

}
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
	if (CurrentGameState == EWaveState::WaveStart) {
		++WaveNumber;
		UE_LOG(LogTemp, Warning, TEXT("GameState: WaveNumber %i"), WaveNumber);
		WaveStateChanged(CurrentGameState, OldState, WaveNumber);
	}
	
}	

void AHMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHMGameState, CurrentGameState);
	// DOREPLIFETIME(AHMGameState, WaveNumber);
}
