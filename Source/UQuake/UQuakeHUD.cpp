// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UQuake.h"
#include "UQuakeHUD.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"

AUQuakeHUD::AUQuakeHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshiarTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshiarTexObj.Object;
}

void AUQuakeHUD::BeginPlay()
{
    Super::BeginPlay();
    Player = Cast<AUQuakeCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AUQuakeHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );

    // Draw ammo
    if (Player.IsValid())
    {
        FString CurrentAmmoString = FString::FromInt(Player->GetCurrentWeaponAmmo()) + "/" + FString::FromInt(Player->GetCurrentWeaponMaxAmmo());
        DrawText(CurrentAmmoString, FColor::White, 50, 50, Font);
    }
}

