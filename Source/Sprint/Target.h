// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Target.generated.h"

UCLASS()
class SPRINT_API ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATarget();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/* This is the mesh to the target */
	UPROPERTY(EditAnywhere, Category = "Destruction")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Destruction")
	float ImpulseStrength;

	UPROPERTY(EditAnywhere, Category = "Destruction Particles")
	UParticleSystemComponent* Smoke;

	UPROPERTY(EditAnywhere, Category = "Destruction")
	UDestructibleComponent* DestructComp;

	/* this is the function that is called when the targets health is below 0*/
	void DestroyTarget();

	/* This is the value from 0-100 and represents the health of the target */
	UPROPERTY(EditAnywhere, Category = "Health")
	float Health;

	/*
	This function takes a value and damages the target with that value
	@param Damage this is the amount to damage the target by
	*/
	void DamageTarget(float Damage);
	
	void PrintDebugs();

	void Delete();
};
