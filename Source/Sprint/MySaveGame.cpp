// Fill out your copyright notice in the Description page of Project Settings.

#include "Sprint.h"
#include "MySaveGame.h"


UMySaveGame::UMySaveGame()
{
	//This all inherits from the SaveGame class already defined by UE4
	PlayerLocation = FVector(0.f, 0.f, 0.f);

}

