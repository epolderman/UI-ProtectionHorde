// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameOverlay.h"
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
void SGameOverlay::Construct(const FArguments& InArgs)
{
	OwnerHud = InArgs._OwnerHud;
	Text = InArgs._Text;
	CurrentState = EVisibleState::VS_Hidden;
	this->SetVisibility(EVisibility::Collapsed);
	FSlateFontInfo ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24);
	VisibleAnimation = FCurveSequence();
	ScaleCurveX = VisibleAnimation.AddCurve(0.2f, 0.3f, ECurveEaseFunction::QuadOut);
	ScaleCurveY = VisibleAnimation.AddCurve(0.0f, 0.2f);

		ChildSlot
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Center)
	[
		SNew(SBorder).DesiredSizeScale(this, &SGameOverlay::GetItemScale)
		[
			SNew(STextBlock).Font(ResultFont).Text(Text)
		]
	];

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGameOverlay::TransitionIn()
{
	if (CurrentState == EVisibleState::VS_Visible)
		return;

	this->SetVisibility(EVisibility::Visible);
	VisibleAnimation.Play(this->AsShared());
	CurrentState = EVisibleState::VS_Animating;
}

void SGameOverlay::TransitionOut()
{
	if (CurrentState == EVisibleState::VS_Hidden)
		return;

	VisibleAnimation.PlayReverse(this->AsShared());
	CurrentState = EVisibleState::VS_Animating;
}


FVector2D SGameOverlay::GetItemScale() const
{
	return FVector2D(ScaleCurveX.GetLerp(), ScaleCurveY.GetLerp());
}


void SGameOverlay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{

	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (VisibleAnimation.IsAtStart() && CurrentState == EVisibleState::VS_Animating)
	{
		SetVisibility(EVisibility::Collapsed);
		CurrentState = EVisibleState::VS_Hidden;
	}

	if (VisibleAnimation.IsAtEnd() && CurrentState == EVisibleState::VS_Animating)
		CurrentState = EVisibleState::VS_Visible;
}

	// TranslateAnimation = FCurveSequence();
	// TranslateX = TranslateAnimation.AddCurve(0.0, 1.0, ECurveEaseFunction::QuadIn);

	/* Perfect Layout for Erik killed AI_Player_One Widget*/
	//ChildSlot
	//	.VAlign(VAlign_Center)
	//	.HAlign(HAlign_Center)
	//	[
	//		SNew(SBorder).DesiredSizeScale(this, &SGameOverlay::GetItemScale)
	//		[
	//			SNew(STextBlock)
	//			.Font(ResultFont)
	//	.Text(FText::FromString("Game Menu Overlay"))
	//		]
	//	];


			/*SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()*/

		/* SNew(SCanvas)
		+ SCanvas::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Position(FVector2D(0, 0)) // doesnt take a delegate for polling & animation. how to translate then?
		.Size(FVector2D(100.0f, 40.0f))*/