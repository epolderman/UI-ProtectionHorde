// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


class SGameOverlay;
class AHMHUD;

class HORDEMODE_API SKillContentContainer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SKillContentContainer)
	{}
	SLATE_ARGUMENT(class AHMHUD*, OwnerHud)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	AHMHUD * OwnerHud;
};
