// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameOverlay.h"
#include "SlateOptMacros.h"
#include <DeclarativeSyntaxSupport.h>
#include <SOverlay.h>
#include <SlateFontInfo.h>
#include <Paths.h>

/* Take a look at FCurveSequence and FCurveHandle */

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGameOverlay::Construct(const FArguments& InArgs)
{
	OwnerHud = InArgs._OwnerHud;

	FSlateFontInfo ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24);

	// @todo try out different overlays
	// set position in viewport function to move overlay over

	ChildSlot
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
	
		SNew(SBorder)
.DesiredSizeScale(this, &SGameOverlay::GetItemScale)
		[
			SNew(STextBlock)
			.Font(ResultFont)
			.Text(FText::FromString("Game Menu Overlay"))
		]
		];

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGameOverlay::TransitionIn()
{
	this->SetVisibility(EVisibility::Visible);
	IntroAnimation = FCurveSequence();
	ScaleCurveX = IntroAnimation.AddCurve(0.2f, 0.3f, ECurveEaseFunction::QuadOut);
	ScaleCurveY = IntroAnimation.AddCurve(0.f, 0.2f);
	IntroAnimation.Play(this->AsShared());
}

void SGameOverlay::TransitionOut()
{
	this->SetVisibility(EVisibility::Hidden);
}


FVector2D SGameOverlay::GetItemScale() const
{
	return FVector2D(ScaleCurveX.GetLerp(), ScaleCurveY.GetLerp());
}

void SGameOverlay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (IntroAnimation.IsPlaying()) {
		UE_LOG(LogTemp, Warning, TEXT("Is Playing--->"));
	}
}
