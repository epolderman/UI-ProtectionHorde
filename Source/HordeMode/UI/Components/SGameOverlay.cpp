// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameOverlay.h"
#include "SlateOptMacros.h"
#include <DeclarativeSyntaxSupport.h>
#include <SOverlay.h>
#include <SlateFontInfo.h>
#include <Paths.h>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGameOverlay::Construct(const FArguments& InArgs)
{
	OwnerHud = InArgs._OwnerHud;

	FSlateFontInfo ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24);

	//@todo try out different overlays

	ChildSlot
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
		SNew(SOverlay)
			+ SOverlay::Slot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Center)
		[
		SNew(STextBlock)
			.Font(ResultFont)
			.Text(FText::FromString("Game Menu Overlay"))
		]
		];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
