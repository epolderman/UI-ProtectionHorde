// Fill out your copyright notice in the Description page of Project Settings.

#include "HMPlayerController.h"
#include "UI/HMHUD.h"
#include <DeclarativeSyntaxSupport.h>
#include "Components/SSTitleWidget.h"




AHMPlayerController::AHMPlayerController()
{
	UE_LOG(LogTemp, Warning, TEXT("AHMPlayerController"));
}

void AHMPlayerController::BeginPlay()
{
	//UWorld* const MyWorld = GetWorld();
	//if (MyWorld == nullptr) {
	//	UE_LOG(LogTemp, Warning, TEXT("PC: World is null()"));
	//}

	//AHMHUD * HUD = Cast<AHMHUD>(GetHUD());

	//if (HUD != nullptr && MyWorld != nullptr) {
	//	UE_LOG(LogTemp, Warning, TEXT("PC: GOOD 2 GO"));
	//}
	//else {
	//	UE_LOG(LogTemp, Warning, TEXT("PC: NOT good 2 GO"));
	//}

	//// basically, title is not added to clients, only server authority hud? how should this communication be established?
	//if (Role != ROLE_Authority) {
	//	SAssignNew(HUD->TitleWaveWidget, SSTitleWidget).OwnerWorld(MyWorld).OwnerHud(HUD);
	//	HUD->TitleWaveWidget->ShowTitle("Title Wave Text");
	//}
	//else {
	//	UE_LOG(LogTemp, Warning, TEXT("PC: This the the authority"));
	//}
}
