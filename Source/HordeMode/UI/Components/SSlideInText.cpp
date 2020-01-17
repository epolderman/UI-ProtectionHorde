// Fill out your copyright notice in the Description page of Project Settings.

#include "SSlideInText.h"
#include "SlateOptMacros.h"
#include <DeclarativeSyntaxSupport.h>
#include <SOverlay.h>
#include <SlateFontInfo.h>
#include <Paths.h>
#include <SCanvas.h>
#include <SBorder.h>
#include <SConstraintCanvas.h>

/* Take a look at FCurveSequence and FCurveHandle */

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlideInText::Construct(const FArguments& InArgs)
{
	// OwnerHud = InArgs._OwnerHud;
	TextToShow = InArgs._TextToShow;
	CurrentState = EVisibleState::VS_Hidden;
	this->SetVisibility(EVisibility::Collapsed);
	FSlateFontInfo ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24);
	VisibleAnimation = FCurveSequence();
	ScaleCurveX = VisibleAnimation.AddCurve(0.2f, 0.3f, ECurveEaseFunction::QuadOut);
	ScaleCurveY = VisibleAnimation.AddCurve(0.0f, 0.2f);
	hasShownAndFaded = false;

		ChildSlot
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Center)
	[
		SNew(SBorder).DesiredSizeScale(this, &SSlideInText::GetItemScale)
		[
			SNew(STextBlock).Font(ResultFont).Text(TextToShow).ColorAndOpacity(FColor::Green)
		]
	];

	// starting animation ->
	this->TransitionIn();

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlideInText::TransitionIn()
{
	if (CurrentState == EVisibleState::VS_Visible)
		return;

	this->SetVisibility(EVisibility::Visible);
	VisibleAnimation.Play(this->AsShared());
	CurrentState = EVisibleState::VS_Animating;
}

void SSlideInText::TransitionOut()
{
	if (CurrentState == EVisibleState::VS_Hidden)
		return;

	ScaleCurveX = VisibleAnimation.AddCurve(0.0f, 2.5f, ECurveEaseFunction::QuadOut);
	ScaleCurveY = VisibleAnimation.AddCurve(0.0f, 2.0f);

	VisibleAnimation.PlayReverse(this->AsShared());

	CurrentState = EVisibleState::VS_Animating;
}


FVector2D SSlideInText::GetItemScale() const
{
	return FVector2D(ScaleCurveX.GetLerp(), ScaleCurveY.GetLerp());
}


void SSlideInText::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{

	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	// animating to end -> this code is nasty and technically works but needs a better solid animation state changes
	// and updates the top level container of when the animation is done
	if (VisibleAnimation.IsAtStart() && CurrentState == EVisibleState::VS_Animating)
	{
		SetVisibility(EVisibility::Collapsed);
		CurrentState = EVisibleState::VS_Hidden;
		hasShownAndFaded = true;
		UE_LOG(LogTemp, Warning, TEXT("Clearing Visible State of Widget"));
	}

	// animating to show -> 
	if (VisibleAnimation.IsAtEnd() && CurrentState == EVisibleState::VS_Animating) {
		CurrentState = EVisibleState::VS_Visible;
		TransitionOut();
	}
}