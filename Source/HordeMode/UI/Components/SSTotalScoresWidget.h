// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/*

	Holds Total Scores during the match.

 */
class HORDEMODE_API SSTotalScoresWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSTotalScoresWidget)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class AHMHUD>, OwnerHud);
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
private:

	TWeakObjectPtr<class AHMHUD> OwnerHUD;
};
