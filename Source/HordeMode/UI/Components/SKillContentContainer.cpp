// Fill out your copyright notice in the Description page of Project Settings.

//@todo clean up after first iteration
#include "SKillContentContainer.h"
#include "SlateOptMacros.h"
#include <SWrapBox.h>
#include <STextBlock.h>
#include <SOverlay.h>
#include <SListView.h>
#include <SHeaderRow.h>
#include <Text.h>
#include <SScrollBox.h>
#include <SButton.h>
#include <SharedPointer.h>
#include "SGameOverlay.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SKillContentContainer::Construct(const FArguments& InArgs)
{

	OwnerHud = InArgs._OwnerHud;
	FontForKills = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24);
	ChildSlot.HAlign(HAlign_Right).VAlign(VAlign_Center)
		[
			SNew(SWrapBox)
			.Visibility(EVisibility::Visible)
			.PreferredWidth(500.f)
		+ SWrapBox::Slot().Padding(5)
		[
			SAssignNew(ListViewWidget, SListView<TSharedPtr<FString>>)
			.ItemHeight(24)
			.ListItemsSource(&Items)
			.OnGenerateRow(this, &SKillContentContainer::OnGenerateRowForList)
		]
		];
	
}

void SKillContentContainer::AddSlot(FString Value)
{
	Items.Push(MakeShareable(new FString(Value)));
	ListViewWidget->RequestListRefresh();
	/*
	
	clear list depending on predicate (how do we load it, toggle it for removing)
	for (int32 Index = 0; Index != Items.Num(); ++Index)
	{
		if(Items[Index]->)
	}
	
	*/
	UE_LOG(LogTemp, Warning, TEXT("List Size %i"), Items.Num());
}


TSharedRef<ITableRow> SKillContentContainer::OnGenerateRowForList(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow< TSharedPtr<FString> >, OwnerTable).Padding(2.0f)
		[
			SNew(SGameOverlay).TextToShow(FText::FromString(*Item.Get()))
		];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


