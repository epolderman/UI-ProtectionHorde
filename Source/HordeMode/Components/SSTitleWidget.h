// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include <SWeakWidget.h>
#include "SlateBasics.h"
#include "SlateExtras.h"

class AHMHUD;

/*
		Generic Widget to show titles on our HUD w/ Animations
 */
class HORDEMODE_API SSTitleWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSTitleWidget)
	{}

	// SLATE_ARGUMENT(TWeakObjectPtr<class UWorld>, OwnerWorld)
	SLATE_ARGUMENT(TWeakObjectPtr<class AHMHUD>, OwnerHud)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
protected:

/*		
		pointer to active widget container 
		Implements a widget that holds a weak pointer to one child widget.
 
		Weak widgets encapsulate a piece of content without owning it.
		e.g. A tooltip is owned by the hovered widget but displayed
		by a floating window. That window cannot own the tooltip
		and must therefore use an SWeakWidget.
 */
	TSharedPtr<SWeakWidget> TitleContainer;

	/** Owner world */
	// TWeakObjectPtr<class UWorld> OwnerWorld;

	/* Owner HUD */
	TWeakObjectPtr<class AHMHUD> OwnerHud;

	FText TitleText = FText::FromString("THIS IS A HUD");

	/** returns title string to display */
	FText GetTitleText() const;

private: 

	/** SWidget overrides */
	// virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
};
