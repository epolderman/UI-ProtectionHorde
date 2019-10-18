// Fill out your copyright notice in the Description page of Project Settings.

#include "SSTotalScoresWidget.h"
#include "SlateOptMacros.h"
#include <SUniformGridPanel.h>
#include <STextBlock.h>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSTotalScoresWidget::Construct(const FArguments& InArgs)
{
	
	OwnerHUD = InArgs._OwnerHud;
	FSlateFontInfo ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24);

	// @todo: set up replication fetching and binding data

	ChildSlot.VAlign(VAlign_Top).HAlign(HAlign_Right)
	[
		SNew(SUniformGridPanel).SlotPadding(FMargin(5.0))
		+ SUniformGridPanel::Slot(0, 0).HAlign(HAlign_Right)[
			SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(ResultFont)
				.Text(FText::FromString("Player 1 Name"))
		]
		+ SUniformGridPanel::Slot(0, 1).HAlign(HAlign_Right)[
			SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(ResultFont)
				.Text(FText::FromString("Player 2 Name"))
		]
		+ SUniformGridPanel::Slot(1, 0).HAlign(HAlign_Right)[
			SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(ResultFont)
				.Text(FText::FromString("Player 1 Score"))
				]
				+ SUniformGridPanel::Slot(1, 1).HAlign(HAlign_Right)[
			SNew(STextBlock)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(ResultFont)
				.Text(FText::FromString("Player 2 Score"))
				]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
