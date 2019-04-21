#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HMCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AHMWeapon;
class USHealthComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8 {
	Grenade UMETA(DisplayName = "Grenade"),
	Regular UMETA(DisplayName = "Rifle"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChangeSignature, AHMCharacter *, owningChar, EWeaponState, currentWeaponIndex);

UCLASS()
class HORDEMODE_API AHMCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	AHMCharacter();	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWeaponChangeSignature OnWeaponChange;
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Inputs && Movement */
	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void JumpAction();

	void BeginZoomAction();

	void EndZoomAction();

	void SwitchWeapon();

	bool isZooming;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
	float zoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom", meta = (ClampMin = 0.1, ClampMax = 100))
	float zoomSpeed;

	float defaultFOV;
	/* End Inputs && Movement */

	/* Weapons */
	AHMWeapon * currentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AHMWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AHMWeapon> SecondaryWeaponClass;

	TArray<TSubclassOf<AHMWeapon>> Weapons;

	UPROPERTY(VisibleDefaultsOnly, Category="Weapon")
	FName WeaponAttachmentSocketName;

	void SpawnWeapon(EWeaponState weaponIndex);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	EWeaponState currentWeaponIndex;
	/* End Weapons */

	/* Health */
	UFUNCTION()
	void OnHealthChanged(USHealthComponent* HealthComp, float Health, float HealthDelta,
			const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(BlueprintReadOnly, Category="Player")
	bool isDead;
	/* End Health */	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent * HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent * CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent * SpringArmComponent;

	UFUNCTION()
	void HandleWeaponChange(AHMCharacter * owningChar, EWeaponState currentWeaponIndex);
};
