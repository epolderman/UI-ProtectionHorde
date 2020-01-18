
#include "HMHUD.h"
#include "HMGameMode.h"
#include <Engine/Engine.h>
#include <DeclarativeSyntaxSupport.h>
#include "UI/Components/STitleWidget.h"
#include "HMPlayerState.h"
#include "Widgets/SWeakWidget.h" 
#include "UI/Components/SKillWidget.h"
#include "UI/Components/STotalScoresWidget.h"
#include "UI/Components/SSlideInText.h"
#include "HMGameState.h"
#include "UI/Components/SKillContentContainer.h"


AHMHUD::AHMHUD()
{
	bIsTitleVisible = false;
	bisKillWidgetInitialized = false;
	bisTotalScoreWidgetInitialized = false;
	bisOverlayMenuVisible = false;
}

void AHMHUD::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

void AHMHUD::BeginPlay()
{
	Super::BeginPlay();
	InitializeTotalKillsWidget();
	InitializeTotalScoresWidget();
	InitializeOverlayMenu();
}

void AHMHUD::InitializeTotalKillsWidget()
{
	if (bisKillWidgetInitialized)
	return;

	APlayerController * OwningPlayerController = this->GetOwningPlayerController();
	AHMPlayerState * PlayerState = OwningPlayerController != nullptr ? Cast<AHMPlayerState>(OwningPlayerController->PlayerState) : nullptr;
	int32 PlayerKills = PlayerState != nullptr ? PlayerState->GetKills() : 0;

	FText ScoreUpdate = FText::Format(NSLOCTEXT("GameFlow", "KillNr", "Kills {0}"), FText::AsNumber(PlayerKills));
	TotalKillsWidget = SNew(SKillWidget).OwnerHud(this).TextToSet(ScoreUpdate);
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(TotalKillsWidget.ToSharedRef()));
	bisKillWidgetInitialized = true;

	if (PlayerKills == 0)
	TotalKillsWidget->SetVisibility(EVisibility::Hidden);
}

void AHMHUD::InitializeTotalScoresWidget()
{
	if (bisTotalScoreWidgetInitialized)
	return;

	UWorld * const MyWorld = GetWorld();
	AHMGameState * GameState = MyWorld != nullptr ? Cast<AHMGameState>(MyWorld->GetGameState()) : nullptr;
	TotalScoresWidget = SNew(STotalScoresWidget).OwnerHud(this).ScoreArray(GameState->PlayerArray);
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(TotalScoresWidget.ToSharedRef()));
	bisTotalScoreWidgetInitialized = true;

	if (GameState->PlayerArray.Num() != 0)
	TotalScoresWidget->SetVisibility(EVisibility::Visible);
}

/* @todo This needs to be a bigger container component that holds a list of these SGameOverlay
	Widgets so in case 2 enemies die at the same time, it displays in a list.
*/

void AHMHUD::InitializeOverlayMenu() {

	UWorld* const MyWorld = GetWorld();
	KillList = SNew(SKillContentContainer).OwnerHud(this).OwnerWorld(MyWorld);
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(KillList.ToSharedRef()));
}

void AHMHUD::ToggleGameMenu()
{
	// handle this
}
void AHMHUD::ShowWaveTitle(int WaveNumber) {

	UWorld* const MyWorld = GetWorld();

	if (MyWorld == nullptr || bIsTitleVisible) 
	return;

	SAssignNew(this->TitleWaveWidget, STitleWidget).OwnerWorld(MyWorld).OwnerHud(this);
	FText Wave = FText::Format(NSLOCTEXT("GameFlow", "WaveNr", "Wave {0}"), FText::AsNumber(WaveNumber));
	this->TitleWaveWidget->ShowTitle(Wave);
	bIsTitleVisible = true;

}

void AHMHUD::HideWaveTitle() {

	TitleWaveWidget = nullptr;
	bIsTitleVisible = false;
}

void AHMHUD::UpdateTotalKills() {

	if (!bisKillWidgetInitialized)
	return;

	APlayerController * OwningPlayerController = this->GetOwningPlayerController();
	if (OwningPlayerController == nullptr)
	return;

	AHMPlayerState * PlayerState = Cast<AHMPlayerState>(OwningPlayerController->PlayerState);
	int32 UpdatedKills = PlayerState != nullptr ? PlayerState->GetKills() : NULL;
	if (UpdatedKills == NULL)
	return;

	TotalKillsWidget->SetVisibility(EVisibility::Visible);
	FText ScoreUpdate = FText::Format(NSLOCTEXT("GameFlow", "KillNr", "Kills {0}"), FText::AsNumber(UpdatedKills));
	TotalKillsWidget->SetScoreText(ScoreUpdate);
}

void AHMHUD::UpdateTotalScores()
{
	if(!bisTotalScoreWidgetInitialized)
	return;

	UWorld * const MyWorld = GetWorld();
	AHMGameState * GameState = MyWorld != nullptr ? Cast<AHMGameState>(MyWorld->GetGameState()) : nullptr;
	if (GameState == nullptr)
	return;

	KillList->AddSlot("Erik Killed Enemy 1");

	TotalScoresWidget->SetVisibility(EVisibility::Visible);
	TotalScoresWidget->SetPlayerScores(GameState->PlayerArray);
}

/*
	A Shared Reference acts like a Shared Pointer, in the sense that it owns the
	object it references. They differ with regard to null objects; Shared References must
	always reference a non-null object. Because Shared Pointers don't have that restriction,
	a Shared Reference can always be converted to a Shared Pointer, and that Shared Pointer is
	guaranteed to reference a valid object. Use Shared References when you want a guarantee that the
	referenced object is non-null, or if you want to indicate shared object ownership.

	Each client (including server) gets a unique Player Controller, which is specified in the game mode,
	so creating local gui elements and other objects are probably best created in the player controller as
	there is no way to distinct if a character is controlled by one client or another. My understanding is that
	each instance of the game has one player controller but infinite possible characters so you can be assured
	when you are creating objects in the player controller that they will be local to that instance of the game.

	issue is wwhat owns this? server? clients? how to display only on the client?

	So SAssignNew is just more convenient way of assigning widgets to variables and it makes sure
	that you don't disturb you declarative syntax code and split it into multiple blocks.
	Of course there are scenarios when you would prefer using SNew instead SAssignNew
	but thats totally up to you.

*/

/*
	https://forums.unrealengine.com/development-discussion/c-gameplay-programming/10029-what-exactly-is-sassignnew-doing
*/

