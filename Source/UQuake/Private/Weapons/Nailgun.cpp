// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "Nailgun.h"

void ANailgun::Fire(class AUQuakeCharacter* player)
{
    if (CanFire())
    {
        auto SpawnRotation = player->GetControlRotation();
        FVector SpawnLocation;

        if (LeftFire)
        {
            SpawnLocation = WeaponMesh->GetSocketLocation("MuzzleLeft");
            LeftFire = false;
        }
        else
        {
            SpawnLocation = WeaponMesh->GetSocketLocation("MuzzleRight");
            LeftFire = true;
        }

        GetWorld()->SpawnActor<AUQuakeProjectile>(Projectile, SpawnLocation, SpawnRotation);
        currentAmmo--;

        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

        remainingReloadTime = reloadTime;
    }
}
