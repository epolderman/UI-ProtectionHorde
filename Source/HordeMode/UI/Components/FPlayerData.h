// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPlayerData.generated.h"

USTRUCT()
struct HORDEMODE_API FPlayerData
{
	GENERATED_BODY()

	float Score;

	FString Name;

	FPlayerData(float score = 0.0f, FString name = FString("No Player Name")): Score(score), Name(name){}
};
