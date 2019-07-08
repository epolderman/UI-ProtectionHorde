#include "SSTitleWidget.h"
#include "SlateOptMacros.h"
#include "UI/HMHUD.h"
#include <DeclarativeSyntaxSupport.h>
#include <Engine/Engine.h>
#include <UnrealMathUtility.h>


// TODO: Animations on the title, bind to the gameMode wave

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSTitleWidget::Construct(const FArguments& InArgs)
{

	// OwnerHud = InArgs._OwnerHud;
	OwnerWorld = InArgs._OwnerWorld;
	TitleRequestedTime = 0.0f;
	FadeAnimationDuration = 5.0f;
	DurationOfTitle = 1.0f;

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

	UE_LOG(LogTemp, Warning, TEXT("ShowTitle"));
	
	if (GEngine != nullptr && OwnerWorld != nullptr) {

		UE_LOG(LogTemp, Warning, TEXT("Adding To ViewPort()"));

		TitleRequestedTime = OwnerWorld->GetTimeSeconds();

		GEngine->GameViewport->AddViewportWidgetContent(
			SAssignNew(TitleContainer, SWeakWidget)
			.PossiblyNullContent(SharedThis(this))
		);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can't add to viewport"));
	}
}

void SSTitleWidget::HideTitle()
{
	if (GEngine != nullptr && GEngine->GameViewport != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("HideTitle"));
		GEngine->GameViewport->RemoveViewportWidgetContent(TitleContainer.ToSharedRef());
		// GEngine->GameViewport->RemoveAllViewportWidgets();
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

	// Animation Code: TODO: 
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
		UE_LOG(LogTemp, Warning, TEXT("Removing Title"));
	
		HideTitle();
	}

}

float SSTitleWidget::GetTimeAlive() const {

	if (OwnerWorld == nullptr) {
		return 0.0f;
	}

	return OwnerWorld->GetTimeSeconds() - TitleRequestedTime;
}
