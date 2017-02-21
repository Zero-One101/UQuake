// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickups/BasePickup.h"
#include "BaseArmourPickup.generated.h"

/**
 * 
 */
UCLASS()
class UQUAKE_API ABaseArmourPickup : public ABasePickup
{
	GENERATED_BODY()

public:
    ABaseArmourPickup();

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
    class UStaticMeshComponent* ArmourMesh;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
