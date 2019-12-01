
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HMGameMode.h"
#include "HMPlayerState.generated.h"


UCLASS()
class HORDEMODE_API AHMPlayerState : public APlayerState
{
	GENERATED_BODY()

	virtual void OnRep_Score() override;

	UFUNCTION()
	void OnRep_Kills();

	UPROPERTY(ReplicatedUsing = OnRep_Kills, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Kills")
	int32 Kills;
public:
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(float DeltaScore);

	float GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Kills")
	void AddKill(int32 KillCount);

	int32 GetKills() const;
};


/*
GameInstance (an object spawned when you launch the application and that remains the same until you close it)
GameMode (spawned when a level is loaded)
GameState (spawned by the gameMode)
PlayerState (spawned when a PlayerController is spawned = when a player arrives in the game)

You have to pay close attention to what you do and where you do it when coding a multiplayer game.
When it comes to singleplayer, you can't really "have it wrong" except for GameInstance stuff.

But here are the general guidelines I follow:
GameInstance - Holds any non-pointer persistent variables (persistent means that you need to store in
between two levels and that you don't need to store in a SaveGame)

GameMode - The overall game manager - starts and stops the current game space you're in, handles the
GameStates and how they rotate - an example might be "King of the Hill"

GameState - Keeps track of every data relative to the current state of the game (timers, scores, winning team)
that all players in the game need to know about, handles scripted events related to the state
For instance:
PregameState: Prevents player from performing any action, starts a timer and display it to everyone. When timer
expires, ask KotH (KingOfTheHill game mode) to rotate to IngameState

IngameState: Enable player input, spawn a big loud noise and open players' spawn gates. Open the
Capture Point and store the amount of capture time both teams have. When one of the team reaches max score, asks KotH to switch to EndgameState.

EndgameState: Destroy every player's characters and starts a cinematic showing the PlayOfTheGame then asks
KotH to rotate to ScoreGameState etc.

PlayerController - HUD, Camera, Mouse, Keyboard, Gamepad, Inputs calling actions on the Character.

PlayerCharacter - Actions in response of Controller's input + Holds personal infos and stats (Health, Ammo -
but Ammo might on your Weapon Class if you can switch Weapons).

PlayerState - Holds every variable non related to the PlayerCharacter that
needs to be known by everyone (best scores, current killing streak, player name...).
 */
