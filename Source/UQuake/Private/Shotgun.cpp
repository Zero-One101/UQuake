// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "Engine.h"
#include "UQuakeCharacter.h"
#include "Shotgun.h"

void AShotgun::Fire(class AUQuakeCharacter* player)
{
    if (CanFire(player))
    {
        FireSpread(player->GetControlRotation(), WeaponMesh->GetSocketLocation("Muzzle"));
        ConsumeAmmo(player);

        if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }

        remainingReloadTime = reloadTime;
    }
}
