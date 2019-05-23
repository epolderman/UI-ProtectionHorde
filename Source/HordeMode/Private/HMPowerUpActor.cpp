

#include "HordeMode/Public/HMPowerUpActor.h"

AHMPowerUpActor::AHMPowerUpActor()
{	
	PowerUpInterval = 0.0f;
	TotalNumberOfTicks = 0;
}

void AHMPowerUpActor::BeginPlay()
{
	Super::BeginPlay();
}

void AHMPowerUpActor::OnTickPowerUp()
{

	TickedProcessed++;

	OnPowerUpTicked();
	

	if (TickedProcessed >= TotalNumberOfTicks) {
		OnExpired();
		UE_LOG(LogTemp, Warning, TEXT("OnExpired()"));

		// remove timer
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerUpTick);
	}
}

void AHMPowerUpActor::ActivatePowerUp()
{
	OnActivated();

	if (PowerUpInterval > 0.0f)
		GetWorldTimerManager().SetTimer(TimerHandle_PowerUpTick, this, &AHMPowerUpActor::OnTickPowerUp, PowerUpInterval, true);
	else
		OnTickPowerUp();

}






