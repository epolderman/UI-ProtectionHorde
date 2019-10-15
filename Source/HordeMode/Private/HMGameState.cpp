

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

		// Display Scores at every round
		for (int i = 0; i < PlayerArray.Num(); i++) {
			AHMPlayerState * p = Cast<AHMPlayerState>(PlayerArray[i]);
			if (p) {
				FString uID = p->UniqueId->ToString();
				float Score = p->GetScore();
				UE_LOG(LogTemp, Warning, TEXT("GameState: Server Score: %f %s"), Score, *uID);
				// UE_LOG(LogTemp, Warning, TEXT("GameState: Server UID: %s"), *uID);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("GameState: Cast to PlayerState failed"));
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
