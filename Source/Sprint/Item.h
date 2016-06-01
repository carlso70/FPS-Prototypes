// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SprintCharacter.h"
#include "Item.generated.h"


UCLASS()
class SPRINT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Trigger box
	UPROPERTY(EditAnywhere)
	UShapeComponent* TBox;
	
	//Mesh associated with the trigger box
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SM_TBox;
	
	ASprintCharacter* MyPlayerController;

	//Inventory Item name
	UPROPERTY(EditAnywhere, Category = "Item Name")
	FString ItemName = FString(TEXT(""));

	// Pickup Item
	void Pickup();

	// Used to get the player requesting to pickup the item
	void GetPlayer(AActor* Player);

	bool bItemIsWithinRange = false;

	/* Toggles item glow */
	void ToggleGlow(bool IsGlowing);

	UFUNCTION()
	void TriggerEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TriggerExit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
