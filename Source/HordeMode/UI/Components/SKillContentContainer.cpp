#include "SKillContentContainer.h"
#include "SlateOptMacros.h"
#include <SWrapBox.h>
#include <SListView.h>
#include <SButton.h>
#include <SharedPointer.h>
#include "SSlideInText.h"

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
	// @todo Do I need to clear items? 
	// Do I need to clear after every round? Performance implications?
	Items.Push(MakeShareable(new FString(Value)));
	ListViewWidget->RequestListRefresh();
}


TSharedRef<ITableRow> SKillContentContainer::OnGenerateRowForList(TSharedPtr<FString> Item, 
																  const TSharedRef<STableViewBase>& OwnerTable)
{

	return SNew(STableRow< TSharedPtr<FString> >, OwnerTable).Padding(2.0f)
		[
			SNew(SSlideInText).TextToShow(FText::FromString(*Item.Get()))
		];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


