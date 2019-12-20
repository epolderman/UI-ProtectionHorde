// Fill out your copyright notice in the Description page of Project Settings.

#include "STotalScoresWidget.h"
#include "SlateOptMacros.h"
#include <SUniformGridPanel.h>
#include <STextBlock.h>
#include "HMPlayerState.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void STotalScoresWidget::Construct(const FArguments& InArgs)
{
	
	OwnerHUD = InArgs._OwnerHud;
	ScoreArray = InArgs._ScoreArray;

	FSlateFontInfo ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24);

	// @todo: We are restricting the player count to 2:
	// set up a more robust score widget that takes an array of vals

	ChildSlot.VAlign(VAlign_Top).HAlign(HAlign_Right)
	[
		SNew(SUniformGridPanel).SlotPadding(FMargin(5.0))
		+ SUniformGridPanel::Slot(0, 0).HAlign(HAlign_Right)[
			SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(ResultFont)
				.WrapTextAt(10)
				.Text(this, &STotalScoresWidget::GetFirstPlayerName)
		]
		+ SUniformGridPanel::Slot(0, 1).HAlign(HAlign_Right)[
			SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(ResultFont)
				.WrapTextAt(10)
				.Text(this, &STotalScoresWidget::GetSecondPlayerName)
		]
		+ SUniformGridPanel::Slot(1, 0).HAlign(HAlign_Right)[
			SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(ResultFont)
				.Text(this, &STotalScoresWidget::GetFirstPlayerScore)
				]
				+ SUniformGridPanel::Slot(1, 1).HAlign(HAlign_Right)[
			SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(ResultFont)
				.Text(this, &STotalScoresWidget::GetSecondPlayerScore)
				]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

/* Todo: Optimize these functions */
void STotalScoresWidget::SetPlayerScores(TArray<APlayerState*> PlayerScores)
{
	ScoreArray = PlayerScores;
	AHMPlayerState * PlayerOnePS = Cast<AHMPlayerState>(ScoreArray[0]);
	AHMPlayerState * PlayerTwoPS = Cast<AHMPlayerState>(ScoreArray[1]);
	if (PlayerOnePS)
	PlayerOne = FPlayerData(PlayerOnePS->GetScore(), PlayerOnePS->UniqueId->ToString());

	if (PlayerTwoPS) 
	PlayerTwo = FPlayerData(PlayerTwoPS->GetScore(), PlayerTwoPS->UniqueId->ToString());
}



FText STotalScoresWidget::GetFirstPlayerScore() const
{
	float UpdatedScore = 0.0f;
	FText ScoreUpdate = FText::Format(NSLOCTEXT("GameFlow", "ScoreNr", "Score {0}"), FText::AsNumber(UpdatedScore));

	if (PlayerOne.Score == 0.0f)
	return ScoreUpdate;

	ScoreUpdate = FText::Format(NSLOCTEXT("GameFlow", "ScoreNr", "Score {0}"), FText::AsNumber(PlayerOne.Score));
	return ScoreUpdate;
}

FText STotalScoresWidget::GetFirstPlayerName() const
{
	FString Name = FString("Empty Name");
	FText NameUpdate = FText::FromString(Name);

	if (PlayerOne.Name.IsEmpty())
	return NameUpdate;

	NameUpdate = FText::FromString(PlayerOne.Name);
	return NameUpdate;
}

FText STotalScoresWidget::GetSecondPlayerScore() const
{
	float UpdatedScore = 0.0f;
	FText ScoreUpdate = FText::Format(NSLOCTEXT("GameFlow", "ScoreNr", "Score {0}"), FText::AsNumber(UpdatedScore));

	if (PlayerTwo.Score == 0.0f)
	return ScoreUpdate;

	ScoreUpdate = FText::Format(NSLOCTEXT("GameFlow", "ScoreNr", "Score {0}"), FText::AsNumber(PlayerTwo.Score));
	return ScoreUpdate;
}

FText STotalScoresWidget::GetSecondPlayerName() const
{
	FString Name = FString("Empty Name");
	FText NameUpdate = FText::FromString(Name);

	if (PlayerTwo.Name.IsEmpty())
	return NameUpdate;

	NameUpdate = FText::FromString(PlayerTwo.Name);
	return NameUpdate;
}


