// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickups/BasePickup.h"
#include "BaseWeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class UQUAKE_API ABaseWeaponPickup : public ABasePickup
{
	GENERATED_BODY()
	
public:
    ABaseWeaponPickup();

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon)
    TSubclassOf<class AUQuakeWeapon> Weapon;

    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    class USkeletalMeshComponent* WeaponMesh;

    void ABaseWeaponPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
