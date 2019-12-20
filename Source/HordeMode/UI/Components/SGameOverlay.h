// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class AHMHUD;

/**
 * 
 */
class HORDEMODE_API SGameOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameOverlay)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class AHMHUD>, OwnerHud)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:

	TWeakObjectPtr<class AHMHUD> OwnerHud;
};
