
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HMPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HORDEMODE_API AHMPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(float deltaScore);
};
