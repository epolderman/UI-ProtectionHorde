#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HMCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AHMWeapon;

UCLASS()
class HORDEMODE_API AHMCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	AHMCharacter();	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void JumpAction();

	void BeginZoomAction();

	void EndZoomAction();

	bool isZooming;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
	float zoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom", meta = (ClampMin = 0.1, ClampMax = 100))
	float zoomSpeed;

	float defaultFOV;

	AHMWeapon * currentWeapon;

	// TODO: How does this subclass work in regards so setting up current weapon?
	UPROPERTY(EditDefaultsOnly, Category = "Player Weapon")
	TSubclassOf<AHMWeapon> StarterWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Player Weapon")
	FName WeaponAttachmentSocketName;

	UFUNCTION(BlueprintCallable, Category="Player Weapon")
	void Fire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent * CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent * SpringArmComponent;
};
