#include "STitleWidget.h"
#include "SlateOptMacros.h"
#include "UI/HMHUD.h"
#include <DeclarativeSyntaxSupport.h>
#include <Engine/Engine.h>
#include <UnrealMathUtility.h>
#include "HordeMode.h"
#include <Engine/GameViewportClient.h>


void STitleWidget::Construct(const FArguments& InArgs)
{
	OwnerWorld = InArgs._OwnerWorld;
	OwnerHud = InArgs._OwnerHud;
	TitleRequestedTime = 0.0f;
	FadeAnimationDuration = 2.0f;
	DurationOfTitle = 2.0f;

		ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Center)
		[
		SNew(STextBlock)
		.Font(this, &STitleWidget::GetTitleFont)
		.Text(this, &STitleWidget::GetTitleText)
		]
		];


}

void STitleWidget::SetTitleText(FText Title)
{
	TitleText = Title;
}

void STitleWidget::ShowTitle(FText Title)
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

void STitleWidget::HideTitle()
{
	// there is a bug in here because the actual widget is not being removed on one client
	// the hud slam of the owning pointer is set to null on hidwavetitle
	if (GEngine  && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(TitleContainer.ToSharedRef());
		OwnerHud->HideWaveTitle();
	}
}

FText STitleWidget::GetTitleText() const
{
	return TitleText;
}

FSlateFontInfo STitleWidget::GetTitleFont() const
{
	FSlateFontInfo ResultFont;
	const int32 StartFontSize = 12;

	const int32 AnimatedFontSize = 70;
	const float AnimTime = 0.75f;
	float AnimPercentage = FMath::Min(1.0f, GetTimeAlive() / AnimTime);

	ResultFont = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), FMath::TruncToInt(StartFontSize + AnimatedFontSize * AnimPercentage));
	return ResultFont;
}

void STitleWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	const float TotalLifespan = (FadeAnimationDuration + DurationOfTitle);
	if (TitleRequestedTime > 0.0f && GetTimeAlive() >= TotalLifespan)
	{
		HideTitle();
	}

}

float STitleWidget::GetTimeAlive() const {

	if (OwnerWorld == nullptr) {
		return 0.0f;
	}

	return OwnerWorld->GetTimeSeconds() - TitleRequestedTime;
}

/*
if (OwnerWorld != nullptr)
{
	const APlayerController* MyOwner = GEngine->GetFirstLocalPlayerController(OwnerWorld.Get());
	AStrategyHUD* const MyHUD = (MyOwner) ? Cast<AStrategyHUD>(MyOwner->GetHUD()) : nullptr;
	if (MyHUD)
	{
		if (MyHUD->IsPauseMenuUp() == true)
		{
			ResultColor.A = 0.0f;
		}
	}
}
*/
