// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UQuakeProjectile.h"
#include "UQuakeWeapon.generated.h"

class AUQuakeCharacter;

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
    ENone       UMETA(DisplayName="None"),
    EShell      UMETA(DisplayName="Shells"),
    ENail       UMETA(DisplayName="Nails")
};

UCLASS()
class UQUAKE_API AUQuakeWeapon : public AActor
{
    GENERATED_BODY()
	
public:	
    // Sets default values for this actor's properties
    AUQuakeWeapon();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
	
    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    virtual void Fire(class AUQuakeCharacter* Player);

    virtual void EndFire();

protected:
    /** Returns whether or not the weapon is able to fire */
    bool CanFire(class AUQuakeCharacter* Player);

    /** Decrements weapon ammo */
    void ConsumeAmmo(class AUQuakeCharacter* Player);

    /** Plays the weapon fire sound */
    UFUNCTION(NetMulticast, Unreliable, WithValidation)
    void ServerPlayFireSound();
    void ServerPlayFireSound_Implementation();
    bool ServerPlayFireSound_Validate();

    /** Plays the weapon alt-fire sound */
    UFUNCTION(NetMulticast, Unreliable, WithValidation)
    void ServerPlayAltFireSound(class USoundBase* AltFireSound);
    void ServerPlayAltFireSound_Implementation(class USoundBase* AltFireSound);
    bool ServerPlayAltFireSound_Validate(class USoundBase* AltFireSound);
	
public:
    /** The damage per round */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
    int32 Damage;

    /** The range of the projectiles, if applicable */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
    int32 Range;

    /** The time between shots */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon)
    float ReloadTime;

    /** The type of ammo the weapon consumes */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon)
    EAmmoType AmmoType;

    /** The skeletal mesh representing the weapon */
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    class USkeletalMeshComponent* WeaponMesh;

    /** The sound to play when the weapon fires */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Audio)
    class USoundBase* FireSound;

    /** The projectile class of the weapon, if applicable */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
    TSubclassOf<AUQuakeProjectile> Projectile;

    /** The default amount of ammo the weapon holds */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
    int32 DefaultAmmo;

protected:
    /** The time until the weapon can fire again */
    float RemainingReloadTime;
};
