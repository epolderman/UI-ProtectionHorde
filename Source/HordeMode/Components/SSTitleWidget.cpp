#include "SSTitleWidget.h"
#include "SlateOptMacros.h"
#include "UI/HMHUD.h"
#include <DeclarativeSyntaxSupport.h>
#include <Engine/Engine.h>
#include <UnrealMathUtility.h>
#include "HordeMode.h"
#include <Engine/GameViewportClient.h>


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSTitleWidget::Construct(const FArguments& InArgs)
{
	OwnerWorld = InArgs._OwnerWorld;
	OwnerHud = InArgs._OwnerHud;
	TitleRequestedTime = 0.0f;
	FadeAnimationDuration = 5.0f;
	DurationOfTitle = 5.0f;

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
	SetTitleText(Title);
	if (GEngine != nullptr && OwnerWorld != nullptr) {
		TitleRequestedTime = OwnerWorld->GetTimeSeconds();
		GEngine->GameViewport->AddViewportWidgetContent(
			SAssignNew(TitleContainer, SWeakWidget)
			.PossiblyNullContent(SharedThis(this))
		);
	}
}

void SSTitleWidget::HideTitle()
{
	if (GEngine != nullptr && GEngine->GameViewport != nullptr)
	{
		// there is a bug in here because the actual widget is not being removed on one client
		// the hud slam of the owning pointer is set to null on hidwavetitle
		GEngine->GameViewport->RemoveViewportWidgetContent(TitleContainer.ToSharedRef());
		OwnerHud->HideWaveTitle();
		UE_LOG(LogTemp, Warning, TEXT("Title is Removed"));
	}
}

FText SSTitleWidget::GetTitleText() const
{
	return TitleText;
}

FSlateFontInfo SSTitleWidget::GetTitleFont() const
{
	FSlateFontInfo ResultFont;
	const int32 StartFontSize = 8;

	// Animation Code:
	const int32 AnimatedFontSize = 70;
	const float AnimTime = 1.0f;
	float AnimPercentage = FMath::Min(1.0f, GetTimeAlive() / AnimTime);
	// End Animation Code

	ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), FMath::TruncToInt(StartFontSize + AnimatedFontSize * AnimPercentage));
	return ResultFont;
}

void SSTitleWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	const float TotalLifespan = (FadeAnimationDuration + DurationOfTitle);
	if (TitleRequestedTime > 0.0f && GetTimeAlive() >= TotalLifespan)
	{
		HideTitle();
	}

}

float SSTitleWidget::GetTimeAlive() const {

	if (OwnerWorld == nullptr) {
		return 0.0f;
	}

	return OwnerWorld->GetTimeSeconds() - TitleRequestedTime;
}
