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

    virtual void Fire(class AUQuakeCharacter* player);

protected:

    bool CanFire(class AUQuakeCharacter* player);

	
public:
    // The damage per round
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
    int32 damage;

    // The range of the projectiles, if applicable
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
    int32 range;

    // The time between shots
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon)
    float reloadTime;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon)
    EAmmoType ammoType;

    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    class USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Audio)
    class USoundBase* FireSound;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
    TSubclassOf<AUQuakeProjectile> Projectile;

protected:
    // The time until the weapon can fire again
    float remainingReloadTime;
};
