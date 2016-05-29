// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SprintCharacter.h"
#include "GameFramework/Actor.h"
#include "Fire.generated.h"

UCLASS()
class SPRINT_API AFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFire();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Trigger box
	UPROPERTY(EditAnywhere)
	UShapeComponent* TB_Fire;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SM_Fire;
	
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* PS_Fire;

	ASprintCharacter* CurrentPlayerController;

	UPROPERTY(EditAnywhere)
	FString Helptext = FString(TEXT("Press F to activate fire"));

	int32 ResetTime = 3; 

	bool bPlayerIsWithinRange = false;
	bool bFireIsLit = false;

	// This will be used to initialize the CurrentPlayerController
	void GetPlayer(AActor* Player);
	
	// Light the fire
	void Light();

	// Handles TimerHandle
	void AdvanceTimer();
	void TimerHasFinished();
	FTimerHandle CountdownTimerHandle;

	UFUNCTION()
	void TriggerEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void TriggerExit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

