// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "SprintGameMode.generated.h"

UCLASS(minimalapi)
class ASprintGameMode : public AGameMode
{
	GENERATED_BODY()

		virtual void BeginPlay() override; //override the gameplay from the base class
public:
	ASprintGameMode();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> PlayerHUDClass;

	// we use uproperty here just to enable garbage collection
	UPROPERTY()
	class UUserWidget* CurrentWidget;

};



