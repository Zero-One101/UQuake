// Fill out your copyright notice in the Description page of Project Settings.

#include "UQuake.h"
#include "UQuakeCharacter.h"
#include "LightningGun.h"

ALightningGun::ALightningGun()
{
    HeldAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Held Audio Component"));
    HeldAudioComponent->bAutoActivate = false;
    //HeldAudioComponent->AttachParent = RootComponent;

    LightningParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Lightning Particle"));
    LightningParticle->AttachToComponent(WeaponMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
    LightningParticle->AutoAttachParent = WeaponMesh;
    LightningParticle->AutoAttachSocketName = TEXT("Muzzle");
    LightningParticle->bAutoManageAttachment = true;
}

void ALightningGun::BeginPlay()
{
    Super::BeginPlay();
    LightningParticle->DeactivateSystem();
    if (HeldSound)
    {
        HeldAudioComponent->SetSound(HeldSound);
    }
}

void ALightningGun::Fire(class AUQuakeCharacter* Player)
{
    if (CanFire(Player))
    {
        if (!IsFiring && IsValid(FireSound))
        {
            ServerPlayFireSound();
            HeldAudioComponent->Play();
            IsFiring = true;
        }
        LightningParticle->ActivateSystem(true);
        ConsumeAmmo(Player);

        RemainingReloadTime = ReloadTime;
    }
    else if (IsFiring && Player->GetAmmo(AmmoType) <= 0)
    {
        EndFire();
    }
}

void ALightningGun::EndFire()
{
    LightningParticle->DeactivateSystem();
    IsFiring = false;
    HeldAudioComponent->Stop();
}