// Fill out your copyright notice in the Description page of Project Settings.

#include "SSTotalScoresWidget.h"
#include "SlateOptMacros.h"
#include <SUniformGridPanel.h>
#include <STextBlock.h>
#include "HMPlayerState.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSTotalScoresWidget::Construct(const FArguments& InArgs)
{
	
	OwnerHUD = InArgs._OwnerHud;
	ScoreArray = InArgs._ScoreArray;

	FSlateFontInfo ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24);

	// @todo: set up replication fetching and binding data - WIP

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
				.Text(this, &SSTotalScoresWidget::GetFirstPlayerName)
		]
		+ SUniformGridPanel::Slot(0, 1).HAlign(HAlign_Right)[
			SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(ResultFont)
				.WrapTextAt(10)
				.Text(this, &SSTotalScoresWidget::GetSecondPlayerName)
		]
		+ SUniformGridPanel::Slot(1, 0).HAlign(HAlign_Right)[
			SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(ResultFont)
				.Text(this, &SSTotalScoresWidget::GetFirstPlayerScore)
				]
				+ SUniformGridPanel::Slot(1, 1).HAlign(HAlign_Right)[
			SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(ResultFont)
				.Text(this, &SSTotalScoresWidget::GetSecondPlayerScore)
				]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSTotalScoresWidget::SetPlayerScores(TArray<APlayerState*> PlayerScores)
{
	ScoreArray = PlayerScores;
	AHMPlayerState * PlayerOnePS = Cast<AHMPlayerState>(ScoreArray[0]);
	AHMPlayerState * PlayerTwoPS = Cast<AHMPlayerState>(ScoreArray[1]);
	if (PlayerOnePS) {
		PlayerOneScore = PlayerOnePS->GetScore();
		PlayerOneName = PlayerOnePS->UniqueId->ToString();
	}
	if (PlayerTwoPS) {
		PlayerTwoScore = PlayerTwoPS->GetScore();
		PlayerTwoName = PlayerTwoPS->UniqueId->ToString();
	}
}



FText SSTotalScoresWidget::GetFirstPlayerScore() const
{
	float UpdatedScore = 0.0f;
	FText ScoreUpdate = FText::Format(NSLOCTEXT("GameFlow", "ScoreNr", "Score {0}"), FText::AsNumber(UpdatedScore));
	if (PlayerOneScore == 0.0f) {
		return ScoreUpdate;
	}
	ScoreUpdate = FText::Format(NSLOCTEXT("GameFlow", "ScoreNr", "Score {0}"), FText::AsNumber(PlayerOneScore));
	return ScoreUpdate;
}

FText SSTotalScoresWidget::GetFirstPlayerName() const
{
	FString Name = FString("Empty Name");
	FText NameUpdate = FText::FromString(Name);
	if (PlayerOneName.IsEmpty()) {
		return NameUpdate;
	}
	NameUpdate = FText::FromString(PlayerOneName);
	return NameUpdate;
}

FText SSTotalScoresWidget::GetSecondPlayerScore() const
{
	float UpdatedScore = 0.0f;
	FText ScoreUpdate = FText::Format(NSLOCTEXT("GameFlow", "ScoreNr", "Score {0}"), FText::AsNumber(UpdatedScore));
	if (PlayerTwoScore == 0.0f) {
		return ScoreUpdate;
	}
	ScoreUpdate = FText::Format(NSLOCTEXT("GameFlow", "ScoreNr", "Score {0}"), FText::AsNumber(PlayerTwoScore));
	return ScoreUpdate;
}

FText SSTotalScoresWidget::GetSecondPlayerName() const
{
	FString Name = FString("Empty Name");
	FText NameUpdate = FText::FromString(Name);
	if (PlayerTwoName.IsEmpty()) {
		return NameUpdate;
	}
	NameUpdate = FText::FromString(PlayerTwoName);
	return NameUpdate;
}


