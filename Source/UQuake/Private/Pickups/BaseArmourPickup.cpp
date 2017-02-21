// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "BaseArmourPickup.h"

ABaseArmourPickup::ABaseArmourPickup()
{
    ArmourMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmourMesh"));
    ArmourMesh->SetupAttachment(BoxCollision);
}

void ABaseArmourPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
    Destroy();
}