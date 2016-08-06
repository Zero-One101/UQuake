// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "DoubleShotgun.h"

void ADoubleShotgun::Fire(class AUQuakeCharacter* player)
{
    if (CanFire())
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        remainingReloadTime = reloadTime;
    }
}
