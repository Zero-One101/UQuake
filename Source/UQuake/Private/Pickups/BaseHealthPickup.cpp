// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "UQuakeCharacter.h"
#include "BaseHealthPickup.h"

ABaseHealthPickup::ABaseHealthPickup()
{
    HealthMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthMesh"));
    HealthMesh->SetupAttachment(BoxCollision);
}

void ABaseHealthPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA(AUQuakeCharacter::StaticClass()))
    {
        AUQuakeCharacter* Player = Cast<AUQuakeCharacter>(OtherActor);
        if (Player)
        {
            if (Player->PickupHealth(HealthValue, CanExceedMax))
            {
                UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
                Destroy();
            }
        }
    }
}