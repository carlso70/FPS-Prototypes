// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Sprint.h"
#include "SprintGameMode.h"
#include "SprintHUD.h"
#include "SprintCharacter.h"
#include "Blueprint/UserWidget.h"

ASprintGameMode::ASprintGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASprintHUD::StaticClass();
}

void ASprintGameMode::BeginPlay()
{
	Super::BeginPlay();

	ASprintCharacter* MyCharacter = Cast<ASprintCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	
	if (PlayerHUDClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}