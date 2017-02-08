// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseShotgun.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class UQUAKE_API AShotgun : public ABaseShotgun
{
    GENERATED_BODY()

public:
    virtual void Fire(class AUQuakeCharacter* player) override;
};