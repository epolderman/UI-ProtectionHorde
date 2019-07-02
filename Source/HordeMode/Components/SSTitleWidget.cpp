#include "SSTitleWidget.h"
#include "SlateOptMacros.h"
#include "UI/HMHUD.h"
#include <DeclarativeSyntaxSupport.h>
#include <Engine/Engine.h>


// TODO: Animations on the title, bind to the gameMode wave

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSTitleWidget::Construct(const FArguments& InArgs)
{

	OwnerHud = InArgs._OwnerHud;

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock).Font(this, &SSTitleWidget::GetTitleFont)
							.Text(this, &SSTitleWidget::GetTitleText)
		]
		];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSTitleWidget::SetTitleText(FString NewTitle)
{
	TitleText = FText::FromString(NewTitle);
}

void SSTitleWidget::ShowTitle(FString Title)
{
	TitleText = FText::FromString(Title);

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->AddViewportWidgetContent(
			SAssignNew(TitleContainer, SWeakWidget)
			.PossiblyNullContent(SharedThis(this))
		);
	}
}

FText SSTitleWidget::GetTitleText() const
{
	return TitleText;
}

FSlateFontInfo SSTitleWidget::GetTitleFont() const
{
	FSlateFontInfo ResultFont;
	const int32 StartFontSize = 50;

	// Animation Code: TODO: 
	// const int32 AnimatedFontSize = 70;
	// const float AnimTime = 1.0f;
	// float AnimPercentage = FMath::Min(1.0f, GetTimeAlive() / AnimTime);
	// End Animation Code

	ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), FMath::TruncToInt(StartFontSize));
	return ResultFont;
}
