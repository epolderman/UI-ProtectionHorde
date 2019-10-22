// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

/*

	Holds Total Scores during the match.

 */

class APlayerState;

class HORDEMODE_API SSTotalScoresWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSTotalScoresWidget)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class AHMHUD>, OwnerHud);
	SLATE_ARGUMENT(TArray<APlayerState *>, ScoreArray);
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetPlayerScores(TArray<APlayerState*> PlayerScores);

private:

	TWeakObjectPtr<class AHMHUD> OwnerHUD;

	TArray<APlayerState *> ScoreArray;

	float PlayerOneScore;

	FString PlayerOneName;

	float PlayerTwoScore;

	FString PlayerTwoName;

	FText GetFirstPlayerScore() const;

	FText GetFirstPlayerName() const;

	FText GetSecondPlayerScore() const;

	FText GetSecondPlayerName() const;
};


