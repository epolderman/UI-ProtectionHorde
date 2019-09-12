// Fill out your copyright notice in the Description page of Project Settings.

#include "HMPlayerState.h"
#include <Engine/Engine.h>
#include "UI/HMHUD.h"
#include "HMCharacter.h"


/*
	
	Persistent information for the player. Player controller only exists
	on your machine and server. 

	"PlayerState is extension of PlayerController" is also wrong. PlayerState is its own 
	class hierarchy and the first parent class the two share is AActor. PlayerState is, as 
	Duncan Dam pointed out, 
	designed to store  data across sessions. It is a form of container, not a form of controller.

	Same thing with GameMode and GameState
*/

// @todo: look into the relationship heirarcy in ue4 for player state
void AHMPlayerState::AddScore(float deltaScore)
{
	Score += deltaScore;
	
	////const AActor * Owner = Cast<AHMCharacter>(GetOwner());
	////if (Owner) {
	////	 APlayerController * MyOwner = Owner
	////	 AHMHUD * const MyHUD = (MyOwner) ? Cast<AHMHUD>(MyOwner->GetHUD()) : nullptr;
	////	 if (MyHUD)
	////	 {
	////		 UE_LOG(LogTemp, Warning, TEXT("HUD: Updating Score %f"), Score);
	////		 MyHUD->UpdateScore(Score);
	////	 }
	////}
	//
	//UWorld* const MyWorld = GEngine->GetWorld();
	//if (MyWorld != nullptr) {
	//	const APlayerController* MyOwner = GEngine->GetFirstLocalPlayerController(MyWorld);
	//	AHMHUD * const MyHUD = (MyOwner) ? Cast<AHMHUD>(MyOwner->GetHUD()) : nullptr;
	//	if (MyHUD)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("HUD: Updating Score %f"), Score);
	//		MyHUD->UpdateScore(Score);
	//	}
	//	else {
	//		UE_LOG(LogTemp, Warning, TEXT("HUD: HUD is null"));
	//	}
	//}
	//else {
	//	UE_LOG(LogTemp, Warning, TEXT("HUD:World is null"));
	//}
}

float AHMPlayerState::GetScore() {
	return Score;
}
