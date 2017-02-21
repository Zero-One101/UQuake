// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "UQuakeWeapon.h"
#include "UQuakeCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AUQuakeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;
public:
	AUQuakeCharacter();

    virtual void BeginPlay();

    /** Returns the ammo count of the specified ammo type */
    UFUNCTION(BlueprintCallable, Category = Ammo)
    int32 GetAmmo(EAmmoType ammoType);

    /** Returns the ammo count of the currently equipped weapon */
    UFUNCTION(BlueprintCallable, Category = Ammo)
    int32 GetCurrentWeaponAmmo();

    /** Returns the maximum ammo of the currently equipped weapon */
    UFUNCTION(BlueprintCallable, Category = Ammo)
    int32 GetCurrentWeaponMaxAmmo();

    /** Returns the player's current health */
    int32 GetHealth();

    /** Returns the player's current armour */
    int32 GetArmour();

    /** Sets the ammo count of the specified ammo type to the specified value, clamping between 0 and max */
    void SetAmmo(EAmmoType ammoType, int32 value);

    /** Handles overlapping with a WeaponPickup class */
    bool PickupWeapon(TSubclassOf<AUQuakeWeapon> WeaponClass);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

    /** The starting inventory of the player */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
    TArray<TSubclassOf<class AUQuakeWeapon>> DefaultInventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
    /** The index of the default weapon. Should be 1 for the Shotgun */
    int32 DefaultWeaponIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
    /** The player's jump sound */
    class USoundBase* JumpSound;

    /** The current health of the player */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Player)
    int32 Health;

    /** The maximum health of the player */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Player)
    int32 MaxHealth;

    /** The current armour of the player */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Player)
    int32 Armour;

    /** The maximum armour of the player */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Player)
    int32 MaxArmour;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Ammo)
    /** The number of Shells the player has */
    int32 Shells;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
    /** The maximum number of Shells the player can carry */
    int32 MaxShells;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Ammo)
    /** The number of Nails the player has */
    int32 Nails;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
    /** The maximum number of Nails the player can carry */
    int32 MaxNails;

private:
    /** The current inventory of the player */
    UPROPERTY(Replicated)
    TArray<AUQuakeWeapon*> WeaponInventory;

    /** The currently equipped weapon */
    UPROPERTY(Replicated)
    class AUQuakeWeapon* CurrentWeapon;

    /** The index of the currently equipped weapon */
    int32 WeaponIndex;

    /** Creates an instance of the specified weapon class and adds it to the inventory */
    void AddWeapon(TSubclassOf<AUQuakeWeapon> WeaponClass);


protected:
    /** Creates the default inventory */
    void CreateInventory();
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerCreateInventory();
    void ServerCreateInventory_Implementation();
    bool ServerCreateInventory_Validate();

    /** Switches to the next weapon in the inventory */
    void NextWeapon();
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerNextWeapon();
    void ServerNextWeapon_Implementation();
    bool ServerNextWeapon_Validate();

    /** Switches to the previous weapon in the inventory */
    void PrevWeapon();
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerPrevWeapon();
    void ServerPrevWeapon_Implementation();
    bool ServerPrevWeapon_Validate();

    void FireHeld(float Val);
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerFireHeld(float Val);
    void ServerFireHeld_Implementation(float Val);
    bool ServerFireHeld_Validate(float Val);

    virtual void Jump() override;
    UFUNCTION(NetMulticast, Unreliable, WithValidation)
    void ServerPlayJumpSound();
    void ServerPlayJumpSound_Implementation();
    bool ServerPlayJumpSound_Validate();

    /** Returns the maximum ammo capacity of the specified ammo type */
    int32 GetMaxAmmo(EAmmoType ammoType);

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

    void UpdateCurrentWeapon();

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

