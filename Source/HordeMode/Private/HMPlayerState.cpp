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
	Score += deltaScore;
	UE_LOG(LogTemp, Warning, TEXT("PlayerState: Updating Score %f"), Score);
	// @todo notify hud / player to display score
	// does this need to be replicated so other players can see this information?
}

float AHMPlayerState::GetScore() const {
	return Score;
}
