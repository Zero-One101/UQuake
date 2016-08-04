// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UQuake.h"
#include "UQuakeGameMode.h"
#include "UQuakeHUD.h"
#include "UQuakeCharacter.h"

AUQuakeGameMode::AUQuakeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUQuakeHUD::StaticClass();
}
