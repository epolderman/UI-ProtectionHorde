
#include "HMPlayerState.h"
#include <Engine/Engine.h>
#include "UI/HMHUD.h"
#include "HMCharacter.h"
#include "HMGameState.h"
#include "HMGameMode.h"
#include <Kismet/GameplayStatics.h>


/*
	
	Persistent information for the player. Player controller only exists
	on your machine and server. 

	hierarchy in ue4 for player state

	UObjectBase
	UObjectBaseUtility
	UObject
	AActor
	AInfo
	APlayerState
*/


void AHMPlayerState::AddScore(float deltaScore)
{
	// Score is Replicated : Direct change to server
	Score += deltaScore;
}


float AHMPlayerState::GetScore() const {
	return Score;
}

/* Callback to tell the client it has been replicated to clients */
void AHMPlayerState::OnRep_Score()
{
	AHMHUD * PlayerHud = Cast<AHMHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (PlayerHud)
	{
		PlayerHud->UpdatePlayerScore();
		PlayerHud->UpdateTotalScores();
	}
}
