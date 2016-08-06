// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UQuakeWeapon.h"
#include "DoubleShotgun.generated.h"

/**
 * 
 */
UCLASS()
class UQUAKE_API ADoubleShotgun : public AUQuakeWeapon
{
    GENERATED_BODY()
	
public:
    virtual void Fire(class AUQuakeCharacter* player) override;
};
