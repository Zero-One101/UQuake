// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "Engine.h"
#include "BaseShotgun.h"

void ABaseShotgun::FireSpread(FRotator Rotation, FVector MuzzleLocation)
{
    FCollisionQueryParams TraceParams = FCollisionQueryParams(false);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = false;

    FHitResult Hit(ForceInit);
    FRotationMatrix RotMatrix(Rotation);

    for (int32 i = 0; i < shotsPerSpread; i++)
    {
        FVector Spread = FVector(0, FMath::RandRange(-maxSpread, maxSpread), FMath::RandRange(-maxSpread, maxSpread));
        Spread = RotMatrix.TransformVector(Spread);
        auto End = Rotation.Vector() * range + MuzzleLocation + Spread;
        GetWorld()->LineTraceSingleByChannel(Hit, MuzzleLocation, End, ECC_Visibility, TraceParams);

        if (Hit.bBlockingHit)
        {
            DrawDebugLine(GetWorld(), MuzzleLocation, Hit.Location, FColor::Green, true, -1, 0, 1);
            DrawDebugSolidBox(GetWorld(), FVector(Hit.Location), FVector(5, 5, 5), FRotationMatrix::MakeFromX(Hit.ImpactNormal).ToQuat(), FColor::Green, true, -1, 0);
        }
        else
        {
            DrawDebugLine(GetWorld(), MuzzleLocation, End, FColor::Red, true, -1, 0, 1);
        }
    }
}
