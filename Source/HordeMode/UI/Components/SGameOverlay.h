// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include <CurveHandle.h>
#include "Animation/CurveSequence.h"

class AHMHUD;

enum class EVisibleState : uint8 {
	VS_None,
	VS_Animating,
	VS_Visible,
	VS_Hidden,
};

/*
	Overlay to hold in game menu data.
	Could be: Ammo, Weapons, &Character, Game Settings.
 */
class HORDEMODE_API SGameOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameOverlay)
	{}
	SLATE_ARGUMENT(class AHMHUD*, OwnerHud)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void TransitionIn();

	void TransitionOut();

protected:
	AHMHUD * OwnerHud;

	EVisibleState CurrentState;

	FCurveSequence VisibleAnimation;

	FCurveHandle ScaleCurveX;

	FCurveHandle ScaleCurveY;

	FVector2D GetItemScale() const;

private:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
};
