// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class AHMHUD;

/*
	
	SLATE_BEGIN_ARGS and SLATE_END_ARGS actually create a struct containing variables added by the 
	SLATE_ARGUMENT, SLATE_ATTRIBUTE, and SLATE_EVENT macros. The name given to those variables is in 
	the form of _TheNameYouGaveIt.

	SLATE_ARGUMENT creates a variable of the exact type you gave it, so SLATE_ARGUMENT(int, MyThing)
	makes a variable of type int, with the name _MyThing.

	SLATE_ATTRIBUTE creates a TAttribute for the type you gave it, so SLATE_ATTRIBUTE(int, MyThing)
	makes a variable of type TAttribute, with the name _MyThing.

	Additionally the macros define a series of functions to allow you to set the variable they add,
	which are named based on TheNameYouGaveIt.

	@todo use attribute to bind score delegate at creation of widget, then UpdateScore() just returns
	a float value from the player controller player state.
*/

class HORDEMODE_API SScoreWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SScoreWidget): _TextToSet(FText::FromString(""))
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
