#include "HordeMode/Public/HMCharacter.h"
#include <Components/InputComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/PawnMovementComponent.h>
#include "HordeMode/Public/HMWeapon.h"
#include <Engine/World.h>
#include <Components/SkeletalMeshComponent.h>

// Notes
// TEXT macro helps us construct strings
// Shift + Alt + O = Search entire source
// Root Component = Capsule
// Blueprint values can override C++ values (Example: bUsePawnControlRotation)

AHMCharacter::AHMCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	zoomedFOV = 65.0f;
	zoomSpeed = 10.0f;
	WeaponAttachmentSocketName = "WeaponSocket";
}

void AHMCharacter::BeginPlay()
{
	Super::BeginPlay();

	defaultFOV = CameraComponent->FieldOfView;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	currentWeapon = GetWorld()->SpawnActor<AHMWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (currentWeapon) {
		currentWeapon->SetOwner(this);
		currentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachmentSocketName);
		
	}
}

void AHMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float targetFOV = isZooming ? zoomedFOV : defaultFOV;
	float currentFOV = FMath::FInterpTo(CameraComponent->FieldOfView,targetFOV,DeltaTime,zoomSpeed);

	CameraComponent->SetFieldOfView(currentFOV);
}

// Called to bind functionality to input
void AHMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHMCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AHMCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AHMCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AHMCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AHMCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHMCharacter::JumpAction);

	PlayerInputComponent->BindAction("BeginZoom", IE_Pressed, this, &AHMCharacter::BeginZoomAction);
	PlayerInputComponent->BindAction("BeginZoom", IE_Released, this, &AHMCharacter::EndZoomAction);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHMCharacter::Fire);
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
	
	if (Value != 0.0f) {
		AddMovementInput(GetActorForwardVector() * Value);
	}
}

void AHMCharacter::MoveRight(float Value) {
	if (Value != 0.0f) {
		AddMovementInput(GetActorRightVector() * Value);
	}
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

void AHMCharacter::BeginZoomAction() {
	if (!isZooming) {
		isZooming = true;
	}
}

void AHMCharacter::EndZoomAction() {
	if (isZooming) {
		isZooming = false;
	}
}

void AHMCharacter::Fire()
{
	if (currentWeapon) {
		currentWeapon->Fire();
	}
}

