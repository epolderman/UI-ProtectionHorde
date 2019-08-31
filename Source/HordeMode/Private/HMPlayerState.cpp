// Fill out your copyright notice in the Description page of Project Settings.

#include "HMPlayerState.h"

/*
	
	Persistent information for the player. Player controller only exists
	on your machine and server. 

*/


void AHMPlayerState::AddScore(float deltaScore)
{
	Score += deltaScore;
}
