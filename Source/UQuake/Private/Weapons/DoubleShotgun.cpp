// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "Engine.h"
#include "DoubleShotgun.h"

void ADoubleShotgun::Fire(class AUQuakeCharacter* player)
{
    if (CanFire())
    {
        FireSpread(player->GetControlRotation());
        remainingReloadTime = reloadTime;
    }
}

void ADoubleShotgun::FireSpread(FRotator Rotation)
{
    FCollisionQueryParams TraceParams = FCollisionQueryParams(false);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = false;

    FHitResult Hit(ForceInit);
    auto Start = WeaponMesh->GetSocketLocation("MuzzleLeft");
    FRotationMatrix RotMatrix(Rotation);

    for (auto i = 0; i < 7; i++)
    {
        auto Spread = FVector(0, FMath::RandRange(-MaxSpread, MaxSpread), FMath::RandRange(-MaxSpread, MaxSpread));
        Spread = RotMatrix.TransformVector(Spread);
        auto End = Rotation.Vector() * range + Start + Spread;
        GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

        if (Hit.bBlockingHit)
        {
            DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Green, true, -1, 0, 1);
            DrawDebugSolidBox(GetWorld(), FVector(Hit.Location), FVector(5, 5, 5), FRotationMatrix::MakeFromX(Hit.ImpactNormal).ToQuat(), FColor::Green, true, -1, 0);
        }
        else
        {
            DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, -1, 0, 1);
        }
    }

    currentAmmo--;

    if (currentAmmo <= 0)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SingleBarrelFire, GetActorLocation());
        return;
    }

    Start = WeaponMesh->GetSocketLocation("MuzzleRight");

    // TODO: Reduce code redundancy here

    for (auto i = 0; i < 7; i++)
    {
        auto Spread = FVector(0, FMath::RandRange(-MaxSpread, MaxSpread), FMath::RandRange(-MaxSpread, MaxSpread));
        Spread = RotMatrix.TransformVector(Spread);
        auto End = Rotation.Vector() * range + Start + Spread;
        GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

        if (Hit.bBlockingHit)
        {
            DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Green, true, -1, 0, 1);
            DrawDebugSolidBox(GetWorld(), FVector(Hit.Location), FVector(5, 5, 5), FRotationMatrix::MakeFromX(Hit.ImpactNormal).ToQuat(), FColor::Green, true, -1, 0);
        }
        else
        {
            DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, -1, 0, 1);
        }
    }

    currentAmmo--;

    UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
}
