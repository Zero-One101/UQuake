// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UQuakeWeapon.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "LightningGun.generated.h"

/**
 * 
 */
UCLASS()
class UQUAKE_API ALightningGun : public AUQuakeWeapon
{
	GENERATED_BODY()
		
public:
    ALightningGun();

    virtual void BeginPlay();

    virtual void Fire(class AUQuakeCharacter* Player) override;

    virtual void EndFire() override;

    UPROPERTY(VisibleDefaultsOnly, Category = "Particles")
    class UParticleSystemComponent* LightningParticle;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Audio")
    class USoundBase* HeldSound;

private:
    bool IsFiring = false;

    class UAudioComponent* HeldAudioComponent;
};
