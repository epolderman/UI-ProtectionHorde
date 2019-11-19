
#include "HMPlayerState.h"
#include <Engine/Engine.h>
#include "UI/HMHUD.h"
#include "HMCharacter.h"
#include "HMGameState.h"
#include "HMGameMode.h"
#include <Kismet/GameplayStatics.h>
#include <UnrealNetwork.h>


/*
	
	Persistent information for the player. Player controller only exists
	on your machine and server. 

	Hierarchy in ue4 for player state

	UObjectBase
	UObjectBaseUtility
	UObject
	AActor
	AInfo
	APlayerState
*/


// Score is Replicated : Direct change to server
void AHMPlayerState::AddScore(float deltaScore)
{
	Score += deltaScore;
}

float AHMPlayerState::GetScore() const
{
	return Score;
}

void AHMPlayerState::AddKill(int32 Kill)
{
	Kills += Kill;
}

//void AHMPlayerState::UpdateWeaponIndex(EWeaponState WeaponIndex)
//{
//	int32 index = WeaponIndex == EWeaponState::Regular ? 0 : 1;
//	UE_LOG(LogTemp, Warning, TEXT("PlayerState settings weapon index %i"), index);
//	CurrentWeaponIndex = index;
//}

int32 AHMPlayerState::GetKills() const
{
	return Kills;
}

//int32 AHMPlayerState::GetCurrentWeaponIndex() const
//{
//	return CurrentWeaponIndex;
//}

// Notify HUD
void AHMPlayerState::OnRep_Kills()
{
	AHMHUD * PlayerHud = Cast<AHMHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (PlayerHud)
	PlayerHud->UpdateTotalKills();
}

//void AHMPlayerState::OnRep_CurrentWeaponIndex()
//{
//	UE_LOG(LogTemp, Warning, TEXT("OnRepIndex %i"), CurrentWeaponIndex);
//	AHMCharacter * Character = Cast<AHMCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetCharacter());
//	if (Character) {
//		// handle weapon change / notify / spawn weapon
//		UE_LOG(LogTemp, Warning, TEXT("OnRepIndex %i"), CurrentWeaponIndex);
//	}
//}

/* Callback to tell the client it has been replicated to clients */
void AHMPlayerState::OnRep_Score()
{
	AHMHUD * PlayerHud = Cast<AHMHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (PlayerHud)
	PlayerHud->UpdateTotalScores();
}

void AHMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHMPlayerState, Kills);
	// DOREPLIFETIME(AHMPlayerState, CurrentWeaponIndex);
}

