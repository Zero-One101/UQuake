// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseShotgun.h"
#include "DoubleShotgun.generated.h"

/**
 * 
 */
UCLASS()
class UQUAKE_API ADoubleShotgun : public ABaseShotgun
{
    GENERATED_BODY()
	
public:
    virtual void Fire(class AUQuakeCharacter* player) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
    class USoundBase* SingleBarrelFire;
};
