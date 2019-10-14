

#include "HMGameState.h"
#include <UnrealNetwork.h>

/* this is the file you should set up events and talk to game mode through this */

AHMGameState::AHMGameState() {
	WaveNumber = 0;
}
void AHMGameState::SetWaveState(EWaveState NewWaveState)
{
	if (Role == ROLE_Authority) {

		EWaveState OldState = CurrentGameState;

		// triggers client change when var change
		CurrentGameState = NewWaveState;

		// manually call for the server
		OnRep_WaveState(OldState);
	}

}

void AHMGameState::SetScoreState(float Score)
{
	OnRep_PlayerScore(Score);
}


void AHMGameState::OnRep_WaveState(EWaveState OldState)
{
	if (CurrentGameState == EWaveState::WaveStart) {

		++WaveNumber;

		WaveStateChanged(CurrentGameState, OldState, WaveNumber);

	}
	
}

void AHMGameState::OnRep_PlayerScore(float Score)
{
	float OldScore = PlayerScore;
	PlayerScore = Score;
	ScoreStateChanged(OldScore, PlayerScore);
}


void AHMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHMGameState, CurrentGameState);

	DOREPLIFETIME(AHMGameState, PlayerScore);
}
