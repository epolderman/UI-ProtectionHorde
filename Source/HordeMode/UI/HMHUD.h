// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HMHUD.generated.h"

/**
 * 
 */
class SSTitleWidget;

UCLASS()
class HORDEMODE_API AHMHUD : public AHUD
{
	GENERATED_BODY()

	AHMHUD();
	TSharedPtr<SSTitleWidget> MyUIWidget;
};
