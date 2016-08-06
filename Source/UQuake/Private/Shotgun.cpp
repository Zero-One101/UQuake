// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "Engine.h"
#include "Shotgun.h"

void AShotgun::Tick(float DeltaSeconds)
{
    if (remainingReloadTime > 0)
    {
        remainingReloadTime -= DeltaSeconds;
    }
}

void AShotgun::Fire(class AUQuakeCharacter* player)
{
    if (CanFire())
    {
        UE_LOG(LogTemp, Log, TEXT("Shotgun Fire"));

        FireSpread(player->GetControlRotation());

        if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }

        remainingReloadTime = reloadTime;
    }
}

void AShotgun::FireSpread(FRotator Rotation)
{
    FCollisionQueryParams TraceParams = FCollisionQueryParams(false);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = false;

    FHitResult Hit(ForceInit);
    //auto Start = GetActorLocation();
    auto Start = WeaponMesh->GetSocketLocation("Muzzle");
    FRotationMatrix RotMatrix(Rotation);

    for (int32 i = 0; i < 6; i++)
    {
        UE_LOG(LogTemp, Log, TEXT("BANG!"));

        FVector Spread = FVector(0, FMath::RandRange(-maxSpread, maxSpread), FMath::RandRange(-maxSpread, maxSpread));
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
}
