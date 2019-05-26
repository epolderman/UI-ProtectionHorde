#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HMPowerUpActor.generated.h"

/*
	Specific Power Ups, total number applied
*/

UCLASS()
class HORDEMODE_API AHMPowerUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHMPowerUpActor();

	UFUNCTION(BlueprintImplementableEvent, Category="PowerUps")
	void OnActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnExpired();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnPowerUpTicked();

	void ActivatePowerUp();
protected:
	
	virtual void BeginPlay() override;

	/* Total between power up ticks */
	UPROPERTY(EditDefaultsOnly, Category="PowerUps")
	float PowerUpInterval;

	/* Total Times we apply the power up affect */
	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	int32 TotalNumberOfTicks;

	int32 TickedProcessed;

	FTimerHandle TimerHandle_PowerUpTick;

	UFUNCTION()
	void OnTickPowerUp();
};
