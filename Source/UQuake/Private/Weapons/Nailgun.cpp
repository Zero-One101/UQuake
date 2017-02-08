// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "UQuakeCharacter.h"
#include "Nailgun.h"

void ANailgun::Fire(class AUQuakeCharacter* player)
{
    if (CanFire(player))
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
        ConsumeAmmo(player);

        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

        remainingReloadTime = reloadTime;
    }
}
