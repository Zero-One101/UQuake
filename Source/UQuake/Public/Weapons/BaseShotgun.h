// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UQuakeWeapon.h"
#include "BaseShotgun.generated.h"

/**
 * 
 */
UCLASS()
class UQUAKE_API ABaseShotgun : public AUQuakeWeapon
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
    int32 shotsPerSpread;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
    int32 maxSpread;

protected:
    void FireSpread(FRotator Rotation, FVector MuzzleLocation);	
	
};
