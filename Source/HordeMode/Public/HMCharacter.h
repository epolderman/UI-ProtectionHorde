#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HMCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class HORDEMODE_API AHMCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AHMCharacter();	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
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

	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
	float zoomSpeed;

	float defaultFOV;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent * CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent * SpringArmComponent;
};
