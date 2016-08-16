// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "UQuakeCharacter.h"
#include "UQuakeWeapon.h"


// Sets default values
AUQuakeWeapon::AUQuakeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
}

// Called when the game starts or when spawned
void AUQuakeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUQuakeWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    if (remainingReloadTime > 0)
    {
        remainingReloadTime -= DeltaTime;
    }
}

void AUQuakeWeapon::Fire(class AUQuakeCharacter* player)
{
}

bool AUQuakeWeapon::CanFire(class AUQuakeCharacter* player)
{
    if (remainingReloadTime > 0 || player->GetAmmo(ammoType) <= 0)
    {
        return false;
    }

    return true;
}

