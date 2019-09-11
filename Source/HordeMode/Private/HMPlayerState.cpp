// Fill out your copyright notice in the Description page of Project Settings.

#include "HMPlayerState.h"
#include <Engine/Engine.h>
#include "UI/HMHUD.h"
#include "HMCharacter.h"


/*
	
	Persistent information for the player. Player controller only exists
	on your machine and server. 

*/

// @todo: look into the relationship heirarcy in ue4 for player state
void AHMPlayerState::AddScore(float deltaScore)
{
	Score += deltaScore;
	
	/*const AHMCharacter * Owner = Cast<AHMCharacter>(GetOwner());
	if (Owner) {
		const APlayerController* MyOwner = Owner->PlayerC
	}
	*/
	UWorld* const MyWorld = GEngine->GetWorld();
	if (MyWorld != nullptr) {
		const APlayerController* MyOwner = GEngine->GetFirstLocalPlayerController(MyWorld);
		AHMHUD * const MyHUD = (MyOwner) ? Cast<AHMHUD>(MyOwner->GetHUD()) : nullptr;
		if (MyHUD)
		{
			UE_LOG(LogTemp, Warning, TEXT("HUD: Updating Score %f"), Score);
			MyHUD->UpdateScore(Score);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("HUD: HUD is null"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("HUD:World is null"));
	}
}
