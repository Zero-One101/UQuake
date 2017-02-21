// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UQuake.h"
#include "UQuakeCharacter.h"
#include "UQuakeProjectile.h"
#include "Animation/AnimInstance.h"
#include "UnrealNetwork.h"
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
	Mesh1P->SetOnlyOwnerSee(false);
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
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AUQuakeCharacter, DefaultInventory);
    DOREPLIFETIME(AUQuakeCharacter, CurrentWeapon);
    DOREPLIFETIME(AUQuakeCharacter, WeaponInventory);
    DOREPLIFETIME(AUQuakeCharacter, Shells);
    DOREPLIFETIME(AUQuakeCharacter, Nails);
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
            if (curWeapon)
            {
                // TODO: Find out why the second player is creating a null weapon
                AddWeapon(curWeapon);
            }
        }

        UpdateCurrentWeapon();
    }
}

void AUQuakeCharacter::AddWeapon(TSubclassOf<AUQuakeWeapon> WeaponClass)
{
    AUQuakeWeapon *weapon = GetWorld()->SpawnActor<AUQuakeWeapon>(WeaponClass);
    if (weapon)
    {
        WeaponInventory.Emplace(weapon);
        weapon->SetActorHiddenInGame(true);
        SetAmmo(weapon->AmmoType, GetAmmo(weapon->AmmoType) + weapon->DefaultAmmo);
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
    if (Role != ROLE_Authority)
    {
        ServerPrevWeapon();
    }
    else
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
}

void AUQuakeCharacter::ServerPrevWeapon_Implementation()
{
    PrevWeapon();
}

bool AUQuakeCharacter::ServerPrevWeapon_Validate()
{
    return true;
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

int32 AUQuakeCharacter::GetCurrentWeaponAmmo()
{
    if (CurrentWeapon != nullptr)
    {
        return GetAmmo(CurrentWeapon->AmmoType);
    }

    return 0;
}

int32 AUQuakeCharacter::GetCurrentWeaponMaxAmmo()
{
    if (CurrentWeapon != nullptr)
    {
        return GetMaxAmmo(CurrentWeapon->AmmoType);
    }

    return 0;
}

void AUQuakeCharacter::SetAmmo(EAmmoType ammoType, int32 value)
{
    switch (ammoType)
    {
        case EAmmoType::EShell:
            Shells = value;
            Shells = FMath::Clamp(Shells, 0, MaxShells);
            break;
        case EAmmoType::ENail:
            Nails = value;
            Nails = FMath::Clamp(Nails, 0, MaxNails);
            break;
    }
}

int32 AUQuakeCharacter::GetHealth()
{
    return Health;
}

int32 AUQuakeCharacter::GetArmour()
{
    return Armour;
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
        ServerPlayJumpSound();
    }

    Super::Jump();
}

void AUQuakeCharacter::ServerPlayJumpSound_Implementation()
{
    UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());
}

bool AUQuakeCharacter::ServerPlayJumpSound_Validate()
{
    return true;
}

bool AUQuakeCharacter::PickupWeapon(TSubclassOf<AUQuakeWeapon> WeaponClass)
{
    bool weaponExists = false;
    for (auto& weapon : WeaponInventory)
    {
        if (weapon && weapon->GetClass() == WeaponClass)
        {
            weaponExists = true;
            break;
        }
    }

    if (weaponExists)
    {
        EAmmoType weaponAmmoType = WeaponClass.GetDefaultObject()->AmmoType;
        if (GetAmmo(weaponAmmoType) == GetMaxAmmo(weaponAmmoType))
        {
            // We already have the weapon and full ammo, so we shouldn't pick it up
            return false;
        }
        else
        {
            SetAmmo(weaponAmmoType, GetAmmo(weaponAmmoType) + WeaponClass.GetDefaultObject()->DefaultAmmo);
            return true;
        }
    }

    // If the weapon doesn't exist, create it
    AddWeapon(WeaponClass);
    return true;
}

bool AUQuakeCharacter::PickupArmour(int32 Armour, bool CanExceedMax)
{
    if (((this->Armour >= MaxArmour) && !CanExceedMax) || this->Armour == OverchargedMaxArmour)
    {
        return false;
    }

    if (this->Armour < MaxArmour && !CanExceedMax)
    {
        this->Armour += Armour;
        this->Armour = FMath::Clamp(this->Armour, 0, MaxArmour);
    }
    else if (CanExceedMax)
    {
        this->Armour += Armour;
        this->Armour = FMath::Clamp(this->Armour, 0, OverchargedMaxArmour);
    }

    return true;
}

bool AUQuakeCharacter::PickupHealth(int32 Health, bool CanExceedMax)
{
    if (((this->Health >= MaxHealth) && !CanExceedMax) || this->Health == OverchargedMaxHealth)
    {
        return false;
    }

    if (this->Health < MaxHealth && !CanExceedMax)
    {
        this->Health += Health;
        this->Health = FMath::Clamp(this->Health, 0, MaxHealth);
    }
    else if (CanExceedMax)
    {
        this->Health += Health;
        this->Health = FMath::Clamp(this->Health, 0, OverchargedMaxHealth);
    }

    return true;
}

int32 AUQuakeCharacter::GetMaxAmmo(EAmmoType ammoType)
{
    switch (ammoType)
    {
        case EAmmoType::EShell:
            return MaxShells;
        case EAmmoType::ENail:
            return MaxNails;
        default:
            return 0;
    }
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
