
#include "HMHUD.h"
#include "HMGameMode.h"
#include <Engine/Engine.h>
#include <DeclarativeSyntaxSupport.h>
#include "Components/SSTitleWidget.h"




AHMHUD::AHMHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("AHMHUD()"));
	/*
		A Shared Reference acts like a Shared Pointer, in the sense that it owns the
		object it references. They differ with regard to null objects; Shared References must
		always reference a non-null object. Because Shared Pointers don't have that restriction,
		a Shared Reference can always be converted to a Shared Pointer, and that Shared Pointer is
		guaranteed to reference a valid object. Use Shared References when you want a guarantee that the
		referenced object is non-null, or if you want to indicate shared object ownership.
	*/
	TitleWaveWidget = SNew(SSTitleWidget).Visibility(EVisibility::Visible).OwnerHud(this);
	TitleWaveWidget->SetTitleText("A HUD");
	if (GEngine != nullptr) {
		GEngine->GameViewport->AddViewportWidgetContent(
			TitleWaveWidget.ToSharedRef()
		);
	}
}
