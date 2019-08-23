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

		Slate Attribute = Needs to be TAttribute i.e. TAttribute<FText> TextContent
		Attribute is a special type which allows you to bind functions and variable pointers

		SLATE_ARGUMENT which is non-attribute argument and you can use normal types with it without 
		TAttribute, but then of course you lose ability to bind functions

		Owner HUD
		Weak Pointers are similar to Shared Pointers, but do not own the object they reference, and
		therefore do not affect its lifecycle. This property can be very useful, as it
		breaks reference cycles, but it also means that a Weak Pointer can become null at any time, without warning.
		For this reason, a Weak Pointer can produce a
		Shared Pointer to the object it references, ensuring programmers safe access to the object on a temporary basis.

		Pointer to active widget container
		SWeakWidget = Implements a widget that holds a weak pointer to one child widget.
		Weak widgets encapsulate a piece of content without owning it.
		e.g. A tooltip is owned by the hovered widget but displayed
		by a floating window. That window cannot own the tooltip
		and must therefore use an SWeakWidget.
*/
class HORDEMODE_API SSTitleWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSTitleWidget)
	{}

	SLATE_ARGUMENT(TWeakObjectPtr<class UWorld>, OwnerWorld)
	SLATE_ARGUMENT(TWeakObjectPtr<class AHMHUD>, OwnerHud)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetTitleText(FString NewTitle);

	void ShowTitle(FString Title);

	void HideTitle();
protected:
	TWeakObjectPtr<class UWorld> OwnerWorld;

	TWeakObjectPtr<class AHMHUD> OwnerHud;

	TSharedPtr<SWeakWidget> TitleContainer;

	/* Display Information */
	FText TitleText;

	/* Returns title string to display */
	FText GetTitleText() const;

	/* Returns a modified font style for animations */
	FSlateFontInfo GetTitleFont() const;

	float TitleRequestedTime;

	float DurationOfTitle;

	float FadeAnimationDuration;

	float GetTimeAlive() const;

private: 

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
};
