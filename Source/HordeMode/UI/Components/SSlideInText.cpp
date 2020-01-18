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
#include <Engine/World.h>

/* Take a look at FCurveSequence and FCurveHandle */

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlideInText::Construct(const FArguments& InArgs)
{
	// OwnerHud = InArgs._OwnerHud;
	TextToShow = InArgs._TextToShow;
	OwnerWorld = InArgs._OwnerWorld;
	CurrentState = EVisibleState::VS_Hidden;
	this->SetVisibility(EVisibility::Collapsed);
	FSlateFontInfo ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24);
	VisibleAnimation = FCurveSequence();
	ScaleCurveX = VisibleAnimation.AddCurve(1.0f, 1.0f, ECurveEaseFunction::QuadOut);
	ScaleCurveY = VisibleAnimation.AddCurve(1.0f, 1.0f, ECurveEaseFunction::QuadOut);

	FadeAnimation = FCurveSequence();
	Fade = FadeAnimation.AddCurve(1.0f, 1.0f, ECurveEaseFunction::QuadOut);

	hasShownAndFaded = false;

		ChildSlot
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Center)
	[
		SNew(SBorder).DesiredSizeScale(this, &SSlideInText::GetItemScale).ColorAndOpacity(this, &SSlideInText::GetColor)
		[
			SNew(STextBlock).Font(ResultFont).Text(TextToShow)
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
	FadeAnimation.Play(this->AsShared());

	CurrentState = EVisibleState::VS_Animating;
}

void SSlideInText::TransitionOut()
{
	if (CurrentState == EVisibleState::VS_Hidden)
	return;

	CurrentState = EVisibleState::VS_Animating;
	UE_LOG(LogTemp, Warning, TEXT("PlayReverse--->"));
	// VisibleAnimation.PlayReverse(this->AsShared());
	FadeAnimation.PlayReverse(this->AsShared());
}

FVector2D SSlideInText::GetItemScale() const
{
	return FVector2D(ScaleCurveX.GetLerp(), ScaleCurveY.GetLerp());
}

FLinearColor SSlideInText::GetColor() const 
{
	return FMath::Lerp(FLinearColor(1, 1, 1, 0), FLinearColor(1, 1, 1, 1), Fade.GetLerp());
}

void SSlideInText::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{

	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	//if (VisibleAnimation.IsAtStart() && CurrentState == EVisibleState::VS_Animating)
	//{
	//	SetVisibility(EVisibility::Collapsed);
	//	CurrentState = EVisibleState::VS_Hidden;
	//	hasShownAndFaded = true;
	//	UE_LOG(LogTemp, Warning, TEXT("Clearing Visible State of Widget"));
	//}

	//// animating to show -> 
	//if (VisibleAnimation.IsAtEnd() && CurrentState == EVisibleState::VS_Animating) {
	//	CurrentState = EVisibleState::VS_Visible;
	//	TransitionOut();
	//}
}