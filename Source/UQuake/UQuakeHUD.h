// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "UQuakeCharacter.h"
#include "UQuakeHUD.generated.h"

UCLASS()
class AUQuakeHUD : public AHUD
{
	GENERATED_BODY()

public:
	AUQuakeHUD();

    virtual void BeginPlay() override;

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

    /** The small, 9pt font */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Fonts")
    UFont* SmallFont;

    /** The large, 48pt font */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Fonts")
    UFont* LargeFont;

    /** The colour the HUD text will be rendered */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Fonts")
    FColor FontColour;

    /** The location of the armour text on screen, in the range 0-1 */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Position")
    FVector2D ArmourTextLocation;

    /** The location of the health text on screen, in the range 0-1 */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Position")
    FVector2D HealthTextLocation;

    /** The location of the ammo text on screen, in the range 0-1 */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Position")
    FVector2D AmmoTextLocation;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

    TWeakObjectPtr<AUQuakeCharacter> Player;
};

