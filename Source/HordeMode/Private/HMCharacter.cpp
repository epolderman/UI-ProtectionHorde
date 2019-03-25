// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeMode/Public/HMCharacter.h"
#include <Components/InputComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/PawnMovementComponent.h>

// Notes
// TEXT macro helps us construct strings
// Shift + Alt + O = Search entire source
// Root Component = Capsule
// Blueprint values can override C++ values (Example: bUsePawnControlRotation)


// Sets default values
AHMCharacter::AHMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AHMCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHMCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AHMCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AHMCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed,this, &AHMCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released,this, &AHMCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHMCharacter::JumpAction);
}

FVector AHMCharacter::GetPawnViewLocation() const
{
	if (CameraComponent) {
		return CameraComponent->GetComponentLocation();
	}
	// protection
	return Super::GetPawnViewLocation();
}

/*
		Vector = Length / Magnitude and a Direction.
		Scalar Multiplication / Scaled the forward vector. 
		https://www.khanacademy.org/math/linear-algebra/vectors-and-spaces/vectors/v/multiplying-vector-by-scalar
*/
void AHMCharacter::MoveForward(float Value) {
	
	AddMovementInput(GetActorForwardVector() * Value);
}

void AHMCharacter::MoveRight(float Value) {
	AddMovementInput(GetActorRightVector() * Value);
}

void AHMCharacter::BeginCrouch()
{
	Crouch();
}

void AHMCharacter::EndCrouch()
{
	UnCrouch();
}

void AHMCharacter::JumpAction() {
	Jump();
}

