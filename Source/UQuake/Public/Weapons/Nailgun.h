// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UQuakeWeapon.h"
#include "Nailgun.generated.h"

/**
 * 
 */
UCLASS()
class UQUAKE_API ANailgun : public AUQuakeWeapon
{
    GENERATED_BODY()
	
public:
    virtual void Fire(class AUQuakeCharacter* player) override;

private:
    bool LeftFire = true;
};
