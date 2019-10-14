// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class AHMHUD;

class HORDEMODE_API SScoreWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SScoreWidget)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class AHMHUD>, OwnerHud);
	SLATE_ARGUMENT(FText, TextToSet)
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);

	void SetScoreText(FText Score);

	FText GetScoreText() const;
private: 
	TWeakObjectPtr<class AHMHUD> OwnerHUD;

	FText ScoreText;
};
