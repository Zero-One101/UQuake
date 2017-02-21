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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Fonts")
    UFont* SmallFont;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Fonts")
    UFont* LargeFont;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Fonts")
    FColor FontColour;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Position")
    FVector2D ArmourTextLocation;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Position")
    FVector2D HealthTextLocation;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Font | Position")
    FVector2D AmmoTextLocation;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

    TWeakObjectPtr<AUQuakeCharacter> Player;
};

