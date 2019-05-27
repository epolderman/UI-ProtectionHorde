#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HMGameMode.generated.h"

/*
	Generates all class files for our project, GameState, GameSession, Controller, etc..
	Should exist only on the server when networked. GameState class handles replicated information.
	Gives us MatchState, Delay, etc.. See GameModeBase.h
*/

UCLASS()
class HORDEMODE_API AHMGameMode : public AGameModeBase
{
	GENERATED_BODY()
};
