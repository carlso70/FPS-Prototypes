// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "SprintCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class ASprintCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;
public:
	ASprintCharacter();

	// we override it in order to drain stamina every second 
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float SprintRate;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float RateOfStaminaDecay;

	/* Amount of damage per hit the player does */
	UPROPERTY(EditAnywhere, Category = "Damage")
	float BulletDamage;

	// Player Xp
	int32 PlayerXp = 0;
	
	// Item Inventory 
	TArray<FString> Inventory;

	// Is picking up item
	bool bIsPickingUp = false;

	bool bIsInteracting;

	virtual void BeginPlay();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ASprintProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);


	/* Handles player sprinting begin and end*/
	void BeginSprint();

	void EndSprint();

	bool bIsSprinting;

	// Custom input functions
	void BeginPickup();
	void EndPickup();
	void ShowInventory();
	void BeginInteract();
	void EndInteract();



	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

private:
	
	// Stamina levels for our character
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float InitialStamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float CurrentStamina;

	// Used to control game saves
	void SaveGame();
	void LoadGame();

	void PerformRaycast();

public:
	
	//Accessor function for initial stamina, make these pure so blueprints can get access to these
	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetInitialStamina();

	//Accessor function for current stamina
	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetCurrentStamina();

	/*
	* updates the players current stamina 
	* @param Stamina This is the amount to change the players stamina by, it can be either positive or negative
	*/
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateCurrentStamina(float Stamina);

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

