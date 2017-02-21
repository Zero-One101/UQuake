// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "UQuakeCharacter.h"
#include "BaseWeaponPickup.h"

ABaseWeaponPickup::ABaseWeaponPickup()
{
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(BoxCollision);
}

void ABaseWeaponPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA(AUQuakeCharacter::StaticClass()))
    {
        AUQuakeCharacter* player = Cast<AUQuakeCharacter>(OtherActor);
        if (player)
        {
            if (player->PickupWeapon(Weapon))
            {
                if (PickupSound)
                {
                    UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
                }

                Destroy();
            }
        }
    }
}
