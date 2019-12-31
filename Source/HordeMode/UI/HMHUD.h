#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HMHUD.generated.h"

	/*
		One HUD Per Player Controller. 
		Responsibility: Incorporate UI Widgets, Show player health, ammo, wave states. 
		Explore Animations in slate. 
		

		Apparently this is derived from AActor.
		A Shared Pointer owns the object it references, indefinitely preventing deletion
		of that object, and ultimately handling its deletion when no Shared Pointer or
		Shared Reference (see below) references it. A Shared Pointer can be empty, meaning
		it doesn't reference any object.
		Any non-null Shared Pointer can produce a Shared Reference to the object it references.
	*/


class STitleWidget;
class SKillWidget;
class STotalScoresWidget;
class SGameOverlay;

UCLASS()
class HORDEMODE_API AHMHUD : public AHUD
{
	GENERATED_BODY()
public:
	AHMHUD();

	UFUNCTION(BlueprintCallable, Category = "UIActions")
	void ShowWaveTitle(int32 WaveNumber);

	UFUNCTION(BlueprintCallable, Category = "UIActions")
	void UpdateTotalKills();

	UFUNCTION(BlueprintCallable, Category = "UIActions")
	void UpdateTotalScores();

	void HideWaveTitle();

	void ToggleGameMenu();

protected:
	TSharedPtr<STitleWidget> TitleWaveWidget;

	TSharedPtr<SKillWidget> TotalKillsWidget;

	TSharedPtr<STotalScoresWidget> TotalScoresWidget;

	TSharedPtr<SGameOverlay> OverlayMenu;

	bool bIsTitleVisible;

	bool bisKillWidgetInitialized;

	bool bisTotalScoreWidgetInitialized;

	bool bisOverlayMenuVisible;

	void InitializeOverlayMenu();

	void InitializeTotalKillsWidget();

	void InitializeTotalScoresWidget();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
};

