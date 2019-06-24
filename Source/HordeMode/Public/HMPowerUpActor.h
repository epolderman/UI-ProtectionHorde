#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HMPowerUpActor.generated.h"

/*
	Specific Power Ups, total number applied - REPLICATED
	// Currently the resync issues are solved. In the blueprint of this component
	// when onExpired is called if we call the parentOnExpired it will kill the component
	// and since it is replicated it will die before the client can update its onExpired call
	// which results in resync issues. 
*/

UCLASS()
class HORDEMODE_API AHMPowerUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHMPowerUpActor();

	UFUNCTION(BlueprintImplementableEvent, Category="PowerUps")
	void OnActivated(AActor * ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnExpired();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnPowerUpTicked();

	void ActivatePowerUp(AActor * ActiveFor);
protected:

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

	// keeps state of the power up
	UPROPERTY(ReplicatedUsing=OnRep_PowerupActive)
	bool bIsPowerUpActive;

	// keeps state of the power up
	UPROPERTY(ReplicatedUsing=OnRep_PowerupActive)
	AActor * aActorPowerUp;

	UFUNCTION()
	void OnRep_PowerupActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnPowerupStateChanged(bool bNewIsActive);


};
