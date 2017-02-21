// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "Engine.h"
#include "UQuakeCharacter.h"
#include "DoubleShotgun.h"

void ADoubleShotgun::Fire(class AUQuakeCharacter* player)
{
    if (CanFire(player))
    {
        FireSpread(player->GetControlRotation(), WeaponMesh->GetSocketLocation("MuzzleLeft"));
        ConsumeAmmo(player);
        RemainingReloadTime = ReloadTime;

        if (player->GetAmmo(AmmoType) <= 0)
        {
            ServerPlayAltFireSound(SingleBarrelFire);
            return;
        }

        FireSpread(player->GetControlRotation(), WeaponMesh->GetSocketLocation("MuzzleRight"));
        ConsumeAmmo(player);
        ServerPlayFireSound();
    }
}