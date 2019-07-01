// Fill out your copyright notice in the Description page of Project Settings.

#include "SSTitleWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSTitleWidget::Construct(const FArguments& InArgs)
{

	OwnerWorld = InArgs._OwnerWorld;

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
		.Text(this, &SSTitleWidget::GetTitleText)
		]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FText SSTitleWidget::GetTitleText() const
{
	return TitleText;
}
