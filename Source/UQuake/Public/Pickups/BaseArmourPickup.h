// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickups/BasePickup.h"
#include "BaseArmourPickup.generated.h"

/**
 * 
 */
UENUM()
enum class EArmourType : uint8
{
    ENone           UMETA(DisplayName = "None"),
    EGreen          UMETA(DisplayName = "Green"),
    EYellow         UMETA(DisplayName = "Yellow"),
    ERed            UMETA(DisplayName = "Red")
};

UCLASS()
class UQUAKE_API ABaseArmourPickup : public ABasePickup
{
	GENERATED_BODY()

public:
    ABaseArmourPickup();

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
    class UStaticMeshComponent* ArmourMesh;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Armour")
    int32 ArmourValue;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Armour")
    EArmourType ArmourType;

    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
