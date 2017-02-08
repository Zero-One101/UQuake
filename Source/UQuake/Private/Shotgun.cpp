// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "Engine.h"
#include "UQuakeCharacter.h"
#include "Shotgun.h"

void AShotgun::Fire(class AUQuakeCharacter* player)
{
    if (CanFire(player))
    {
        FireSpread(player->GetControlRotation());

        // TODO: Extract this out as ConsumeAmmo method. Also, why the hell did I hardcode the type here? I have a variable for that
        player->SetAmmo(EAmmoType::EShell, player->GetAmmo(EAmmoType::EShell) - 1);

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
    auto Start = WeaponMesh->GetSocketLocation("Muzzle");
    FRotationMatrix RotMatrix(Rotation);

    for (int32 i = 0; i < 6; i++)
    {
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
