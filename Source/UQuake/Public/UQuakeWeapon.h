// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UQuakeCharacter.h"
#include "UQuakeWeapon.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
    ENone,
    EShell,
    ENail
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

    bool CanFire();

	
public:
    // The ammo currently held by the weapon
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
    int32 currentAmmo;

    // The maximum amount of ammo that can be held by the weapon
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
    int32 maxAmmo;

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

protected:
    // The time until the weapon can fire again
    float remainingReloadTime;
};
