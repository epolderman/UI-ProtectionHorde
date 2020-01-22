// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include <CurveHandle.h>
#include "Animation/CurveSequence.h"
#include <Engine/EngineTypes.h>


class AHMHUD;

enum class EVisibleState : uint8 {
	VS_None,
	VS_Animating_To_Show,
	VS_Animating_To_Hide,
	VS_Visible,
	VS_Hidden,
};

/*
	Reducing the work done every frame:
	Epic converted some of the HUDs from blueprints to C++. 
	They added invalidation panels to widgets. Widgets with invalidation panels 
	are not updated every frame.
	‘Dumpticks enabled’ console command — lists out all tick functions currently 
	scheduled every frame. This can help developers to spot things that happen every 
	frame which might not be necessary.

	Slate prefers polling over invalidation. 
 */
class HORDEMODE_API SSlideInText : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlideInText)
	{}
	SLATE_ARGUMENT(FText, TextToShow)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void TransitionIn();

	void TransitionOut();

	FText TextToShow;
protected:

	EVisibleState CurrentState;

	FCurveSequence VisibleAnimation;

	FCurveHandle ScaleCurveX;

	FCurveHandle ScaleCurveY;

	FCurveSequence FadeAnimation;

	FCurveHandle FadeValue;

	FLinearColor GetColor() const;

	FVector2D GetItemScale() const;

	bool hasShownAndFaded;

private:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
};
