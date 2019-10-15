

#include "HMGameState.h"
#include <UnrealNetwork.h>
#include "HMPlayerState.h"


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

		// Display Scores at every round @todo Put into a widget
		for (int i = 0; i < PlayerArray.Num(); i++) {
			AHMPlayerState * Player = Cast<AHMPlayerState>(PlayerArray[i]);
			if (Player) {
				FString uID = Player->UniqueId->ToString();
				float Score = Player->GetScore();
				UE_LOG(LogTemp, Warning, TEXT("GameState: Server Score: %f %s"), Score, *uID);
			}
		}
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
