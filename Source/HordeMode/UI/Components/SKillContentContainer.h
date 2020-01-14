// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include <UnrealString.h>
#include <SharedPointer.h>
#include <SListView.h>
#include <STableRow.h>


class SGameOverlay;
class AHMHUD;

class HORDEMODE_API SKillContentContainer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SKillContentContainer)
	{}
	SLATE_ARGUMENT(class AHMHUD *, OwnerHud)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void AddSlot(FString Value);

protected:
	AHMHUD * OwnerHud;

	// list of strings
	TArray<TSharedPtr<FString>> Items;

	FReply ButtonPressed();

	// new textbox of string to list
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FString> NewItem, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedPtr< SListView< TSharedPtr<FString> > > ListViewWidget;

	FSlateFontInfo FontForKills;
};