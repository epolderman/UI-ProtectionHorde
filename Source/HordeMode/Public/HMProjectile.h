#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HMProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;
class UDamageType;
class URadialForceComponent;

UCLASS()
class HORDEMODE_API AHMProjectile : public AActor
{
	GENERATED_BODY()
public:	
	AHMProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void LifeSpanExpired() override;

	virtual void BeginPlay() override;

	void OnExplode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem * ExplodeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LifeSpan")
	float LifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Velocity")
	float LaunchVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageRadius; 
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Force")
	URadialForceComponent * RadialFoceComponent;
};
