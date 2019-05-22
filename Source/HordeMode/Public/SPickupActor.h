

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupActor.generated.h"

class USphereComponent;
class UDecalComponent;
class AHMPowerUpActor;

UCLASS()
class HORDEMODE_API ASPickupActor : public AActor
{
	GENERATED_BODY()
public:	
	ASPickupActor();

	virtual void NotifyActorBeginOverlap(AActor* otherActor) override;
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UDecalComponent * DecalComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent * SphereComp;

	UFUNCTION()
	void Respawn();

	AHMPowerUpActor * CurrentPowerUp;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	TSubclassOf<AHMPowerUpActor> PowerUpClass;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	float coolDownDuration;

	FTimerHandle TimerHandle_RespawnTimer;
};
