#include "HordeMode/Public/HMCharacter.h"
#include <Components/InputComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/PawnMovementComponent.h>
#include "HordeMode/Public/HMWeapon.h"
#include <Engine/World.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include <HordeMode/HordeMode.h>
#include "HordeMode/Components/SHealthComponent.h"

// Notes / Hot keys 
// CTRL Shift F = Search Entire Solution & Replace UI
// Shift + Alt + O = Search entire source
// Alt + G => Takes you to the function definition in the Engine. 
// Shift + F1 => While playing game, can regain focus back in editor. 
// Root Component = Capsule
// Blueprint values can override C++ values (Example: bUsePawnControlRotation)
// Notes: Shift + Alt + R = quick rename
// In the UE4 Editor -> End on the keypad will place object on the floor

// 121 - 122 const effectiveness in UE

// DebugGameEditor will allows us to play the game but add breakpoints to see where we are breaking.
// At the top of Visual Studio to the left of Win64

AHMCharacter::AHMCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_HMWEAPON, ECR_Ignore);
	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	// default
	zoomedFOV = 65.0f;
	zoomSpeed = 10.0f;
	timePlayerIsRemovedFromWorld = 10.0f;
	WeaponAttachmentSocketName = "WeaponSocket";
}

void AHMCharacter::BeginPlay()
{
	Super::BeginPlay();

	defaultFOV = CameraComponent->FieldOfView;
	Weapons.Add(StarterWeaponClass);
	Weapons.Add(SecondaryWeaponClass);
	currentWeaponIndex = EWeaponState::Regular;
	SpawnWeapon(currentWeaponIndex);
	HealthComponent->OnHealthChanged.AddDynamic(this, &AHMCharacter::OnHealthChanged);
}

void AHMCharacter::SpawnWeapon(EWeaponState &weaponIndex) 
{
		int index = weaponIndex == EWeaponState::Grenade ? 1 : 0;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		currentWeapon = GetWorld()->SpawnActor<AHMWeapon>(Weapons[index], FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (currentWeapon) {

			currentWeapon->SetOwner(this);
			currentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachmentSocketName);
			HandleWeaponChange(weaponIndex);
		}
}

void AHMCharacter::SwitchWeapon()
{
	if (currentWeapon) {
		currentWeapon->Destroy();
	}
	currentWeaponIndex = currentWeaponIndex == EWeaponState::Regular ? EWeaponState::Grenade : EWeaponState::Regular;
	SpawnWeapon(currentWeaponIndex);
}

void AHMCharacter::HandleWeaponChange(EWeaponState currentWeaponIndex) {
	
	if (OnWeaponChange.IsBound()) {
		OnWeaponChange.Broadcast(currentWeaponIndex);
	}
}

void AHMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float targetFOV = isZooming ? zoomedFOV : defaultFOV;
	float currentFOV = FMath::FInterpTo(CameraComponent->FieldOfView,targetFOV,DeltaTime,zoomSpeed);

	CameraComponent->SetFieldOfView(currentFOV);
}

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

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHMCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AHMCharacter::StopFire);

	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AHMCharacter::SwitchWeapon);
}

FVector AHMCharacter::GetPawnViewLocation() const
{
	if (CameraComponent)
	return CameraComponent->GetComponentLocation();

	// protection
	return Super::GetPawnViewLocation();
}

/*
		Vector = Length / Magnitude and a Direction.
		Scalar Multiplication / Scaled the forward vector. 
		https://www.khanacademy.org/math/linear-algebra/vectors-and-spaces/vectors/v/multiplying-vector-by-scalar
*/
void AHMCharacter::MoveForward(float Value) 
{
	if (Value != 0.0f)
	AddMovementInput(GetActorForwardVector() * Value);
}

void AHMCharacter::MoveRight(float Value) 
{
	if (Value != 0.0f) 
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

void AHMCharacter::JumpAction() 
{
	Jump();
}

void AHMCharacter::BeginZoomAction() 
{
	if (!isZooming) 
	isZooming = true;
}

void AHMCharacter::EndZoomAction() 
{
	if (isZooming) 
	isZooming = false;
}

void AHMCharacter::StartFire()
{
	if (currentWeapon)
	currentWeapon->StartFire();
}

void AHMCharacter::StopFire()
{
	if (currentWeapon)
	currentWeapon->StopFire();
}

void AHMCharacter::OnHealthChanged(USHealthComponent* HealthComp, float Health, float HealthDelta, 
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !isDead) {
		isDead = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		currentWeapon->SetLifeSpan(timePlayerIsRemovedFromWorld);
		SetLifeSpan(timePlayerIsRemovedFromWorld);
	}
}


