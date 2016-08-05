// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UQuakeWeapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class UQUAKE_API AShotgun : public AUQuakeWeapon
{
	GENERATED_BODY()
	
public:
    virtual void Tick(float DeltaSeconds) override;

    virtual void Fire(class AUQuakeCharacter* player) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
    int32 maxSpread;

private:
    void FireSpread(FRotator Rotation);
};
