// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UQuake.h"
#include "UQuakeCharacter.h"
#include "UQuakeProjectile.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/InputSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AUQuakeCharacter

AUQuakeCharacter::AUQuakeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)

    WeaponIndex = DefaultWeaponIndex;

    bReplicates = true;
}

void AUQuakeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME(AUQuakeCharacter, CurrentWeapon);
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AUQuakeCharacter::BeginPlay()
{
    CreateInventory();
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUQuakeCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AUQuakeCharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    InputComponent->BindAction("NextWeapon", IE_Pressed, this, &AUQuakeCharacter::NextWeapon);
    InputComponent->BindAction("PrevWeapon", IE_Pressed, this, &AUQuakeCharacter::PrevWeapon);

	InputComponent->BindAxis("MoveForward", this, &AUQuakeCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUQuakeCharacter::MoveRight);
    InputComponent->BindAxis("FireHeld", this, &AUQuakeCharacter::FireHeld);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AUQuakeCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AUQuakeCharacter::LookUpAtRate);
}

void AUQuakeCharacter::CreateInventory()
{
    if (Role != ROLE_Authority)
    {
        ServerCreateInventory();
    }
    else
    {
        for (auto& curWeapon : DefaultInventory)
        {
            AUQuakeWeapon *weapon = GetWorld()->SpawnActor<AUQuakeWeapon>(curWeapon);
            WeaponInventory.Emplace(weapon);
            weapon->SetActorHiddenInGame(true);
        }

        UpdateCurrentWeapon();
    }
}

void AUQuakeCharacter::ServerCreateInventory_Implementation()
{
    CreateInventory();
}

bool AUQuakeCharacter::ServerCreateInventory_Validate()
{
    return true;
}

void AUQuakeCharacter::NextWeapon()
{
    if (Role != ROLE_Authority)
    {
        ServerNextWeapon();
    }
    else
    {
        auto inventoryLength = WeaponInventory.Num();
        if (WeaponIndex == inventoryLength - 1)
        {
            WeaponIndex = 0;
        }
        else
        {
            WeaponIndex++;
        }

        UpdateCurrentWeapon();
    }
}

void AUQuakeCharacter::ServerNextWeapon_Implementation()
{
    NextWeapon();
}

bool AUQuakeCharacter::ServerNextWeapon_Validate()
{
    return true;
}

void AUQuakeCharacter::PrevWeapon()
{
    auto inventoryLength = WeaponInventory.Num();
    if (WeaponIndex == 0)
    {
        WeaponIndex = inventoryLength - 1;
    }
    else
    {
        WeaponIndex--;
    }

    UpdateCurrentWeapon();
}

int32 AUQuakeCharacter::GetAmmo(EAmmoType ammoType)
{
    switch (ammoType)
    {
        case EAmmoType::ENone:
            return 1;
            break;
        case EAmmoType::EShell:
            return Shells;
            break;
        case EAmmoType::ENail:
            return Nails;
            break;
        default:
            return 0;
            break;
    }
}

void AUQuakeCharacter::SetAmmo(EAmmoType ammoType, int32 value)
{
    switch (ammoType)
    {
        case EAmmoType::EShell:
            Shells = value;
            break;
        case EAmmoType::ENail:
            Nails = value;
            break;
    }
}

void AUQuakeCharacter::FireHeld(float Val)
{
    if (Val > 0)
    {
        if (Role == ROLE_Authority)
        {
            CurrentWeapon->Fire(this);
        }
        else
        {
            ServerFireHeld(Val);
        }
    }
}

void AUQuakeCharacter::ServerFireHeld_Implementation(float Val)
{
    FireHeld(Val);
}

bool AUQuakeCharacter::ServerFireHeld_Validate(float Val)
{
    return true;
}

void AUQuakeCharacter::Jump()
{
    if (CanJump())
    {
        UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());
    }

    Super::Jump();
}

void AUQuakeCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AUQuakeCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AUQuakeCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void AUQuakeCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AUQuakeCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AUQuakeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUQuakeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUQuakeCharacter::UpdateCurrentWeapon()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
        CurrentWeapon->SetActorHiddenInGame(true);
    }
    CurrentWeapon = WeaponInventory[WeaponIndex];
    CurrentWeapon->SetActorHiddenInGame(false);
    CurrentWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

bool AUQuakeCharacter::EnableTouchscreenMovement(class UInputComponent* InputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AUQuakeCharacter::BeginTouch);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &AUQuakeCharacter::EndTouch);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AUQuakeCharacter::TouchUpdate);
	}
	return bResult;
}
