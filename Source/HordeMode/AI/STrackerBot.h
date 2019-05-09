// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"
class UStaticMeshComponent;

UCLASS()
class HORDEMODE_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector getNextLocation();
public:	
	UPROPERTY(VisibleDefaultsOnly, Category="Display")
	UStaticMeshComponent * MeshComponent;
};
