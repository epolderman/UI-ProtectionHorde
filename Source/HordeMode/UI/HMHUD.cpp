// Fill out your copyright notice in the Description page of Project Settings.

#include "HMHUD.h"
#include "HMGameMode.h"
#include <Engine/Engine.h>
#include <DeclarativeSyntaxSupport.h>
#include "Components/SSTitleWidget.h"




AHMHUD::AHMHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("AHMHUD()"));

	//UWorld* const MyWorld = GEngine->GetWorld();
	//AHMGameMode* const MyGameMode = MyWorld->GetAuthGameMode<AHMGameMode>();
	//if (MyGameMode != nullptr)
	//{

	//	SAssignNew(MyGameMode->Title, SSTitleWidget)
	//		.Visibility(EVisibility::HitTestInvisible)
	//		.OwnerWorld(MyWorld);
	//}

	MyUIWidget = SNew(SSTitleWidget);

	// Pass our viewport a weak ptr to our widget
	// View port's weak ptr will not give Viewport ownership of Widget
	if (GEngine != NULL) {
		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(MyUIWidget.ToSharedRef())
		);
	}

	// Set widget's properties as visible (sets child widget's properties recursively)
	MyUIWidget->SetVisibility(EVisibility::Visible);
}
