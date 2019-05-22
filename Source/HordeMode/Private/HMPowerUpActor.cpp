

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
	UE_LOG(LogTemp, Warning, TEXT("TickedProcessed before %i"), TickedProcessed);

	TickedProcessed++;

	OnPowerUpTicked();

	UE_LOG(LogTemp, Warning, TEXT("TickedProcessed after %i"), TickedProcessed);
	

	if (TickedProcessed >= TotalNumberOfTicks) {
		OnExpired();
		UE_LOG(LogTemp, Warning, TEXT("OnExpired()"));

		// remove timer
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerUpTick);
	}
}

void AHMPowerUpActor::ActivatePowerUp()
{
	// blueprint implemented
	OnActivated();

	UE_LOG(LogTemp, Warning, TEXT("PowerupInterval %f"), PowerUpInterval);
	UE_LOG(LogTemp, Warning, TEXT("total number of ticks %i"), TotalNumberOfTicks);

	if (PowerUpInterval > 0.0f)
		GetWorldTimerManager().SetTimer(TimerHandle_PowerUpTick, this, &AHMPowerUpActor::OnTickPowerUp, PowerUpInterval, true);
	else
		OnTickPowerUp();

}






