// Fill out your copyright notice in the Description page of Project Settings.

#include "SKillContentContainer.h"
#include "SlateOptMacros.h"
#include <SWrapBox.h>
#include <STextBlock.h>
#include <SOverlay.h>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SKillContentContainer::Construct(const FArguments& InArgs)
{

	OwnerHud = InArgs._OwnerHud;
	FSlateFontInfo ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24);
	ChildSlot.VAlign(VAlign_Center).HAlign(HAlign_Right)
	[
		
		SNew(SWrapBox)
		.Visibility(EVisibility::Visible)
		.PreferredWidth(500.f)
		+ SWrapBox::Slot().Padding(5)
		[
			SNew(STextBlock).Text(FText::FromString("Some Random String")).Font(ResultFont).ColorAndOpacity(FColor::Red)
		]
		+ SWrapBox::Slot()
		.Padding(5)
		[
			SNew(STextBlock).Text(FText::FromString("Some")).Font(ResultFont).ColorAndOpacity(FColor::Yellow)
		]
		
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
