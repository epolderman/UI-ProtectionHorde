

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

		// server update
		bIsPowerUpActive = false;
		// update clients
		OnRep_PowerupActive();
		// server update
		aActorPowerUp = nullptr;

		// remove timer
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerUpTick);
	}
}
// client calls
void AHMPowerUpActor::OnRep_PowerupActive()
{
	UE_LOG(LogTemp, Warning, TEXT("Client: PowerUpActive()"), bIsPowerUpActive);
	// client: blue print will handle this
	OnPowerupStateChanged(bIsPowerUpActive);

	if (bIsPowerUpActive) {
		UE_LOG(LogTemp, Warning, TEXT("Client: OnActivate"));
		OnActivated(aActorPowerUp);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Client: OnExpired"));
		OnExpired();
		aActorPowerUp = nullptr;
	}
}

void AHMPowerUpActor::ActivatePowerUp(AActor * ActivateFor)
{
	// server call:actor that is targeted = activatedFor
	OnActivated(ActivateFor);

	// replicate on clients
	bIsPowerUpActive = true;
	aActorPowerUp = ActivateFor;
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

	DOREPLIFETIME(AHMPowerUpActor, aActorPowerUp);
}


/*	UCharacterMovementComponent * CharMoveComp = Cast<UCharacterMovementComponent>(otherActor->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
if (CharMoveComp) {
	if (Role == ROLE_Authority) {
		UE_LOG(LogTemp, Warning, TEXT("Client: Active Power Up()"));
		const float walkSpeed = CharMoveComp->MaxWalkSpeed;
		UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed B: %f"), CharMoveComp->MaxWalkSpeed);
		CharMoveComp->MaxWalkSpeed = walkSpeed * 2.0f;

		UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed A: %f"), CharMoveComp->MaxWalkSpeed);
	}

}*/





