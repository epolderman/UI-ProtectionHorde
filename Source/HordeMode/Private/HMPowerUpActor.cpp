

#include "HordeMode/Public/HMPowerUpActor.h"
#include <UnrealNetwork.h>

AHMPowerUpActor::AHMPowerUpActor()
{	
	PowerUpInterval = 0.0f;
	TotalNumberOfTicks = 0;
	bIsPowerUpActive = false;
	SetReplicates(true);
}

void AHMPowerUpActor::OnTickPowerUp()
{

	TickedProcessed++;
	OnPowerUpTicked();
	
	if (TickedProcessed >= TotalNumberOfTicks) {
		OnExpired();

		// notify clients
		bIsPowerUpActive = false;
		OnRep_PowerupActive();
		

		// remove timer
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerUpTick);
	}
}

void AHMPowerUpActor::OnRep_PowerupActive()
{
	// blue print will handle this
	OnPowerupStateChanged(bIsPowerUpActive);
}

void AHMPowerUpActor::ActivatePowerUp(AActor * ActivateFor)
{
	// actor that is targeted = activatedFor
	OnActivated(ActivateFor);

	// replicate on clients
	bIsPowerUpActive = true;
	OnRep_PowerupActive(); // don't get called on server -> call on client

	if (PowerUpInterval > 0.0f)
		GetWorldTimerManager().SetTimer(TimerHandle_PowerUpTick, this, &AHMPowerUpActor::OnTickPowerUp, PowerUpInterval, true);
	else
		OnTickPowerUp();

}

void AHMPowerUpActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHMPowerUpActor, bIsPowerUpActive);
}








