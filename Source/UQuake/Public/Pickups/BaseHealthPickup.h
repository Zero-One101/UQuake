// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickups/BasePickup.h"
#include "BaseHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class UQUAKE_API ABaseHealthPickup : public ABasePickup
{
	GENERATED_BODY()
	
public:
    ABaseHealthPickup();

protected:
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = Mesh)
    class UStaticMeshComponent* HealthMesh;
};
