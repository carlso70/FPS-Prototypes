// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Sprint.h"
#include "SprintCharacter.h"
#include "SprintProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "MySaveGame.h"
#include "Target.h"
//Added Engine.h to print runtime messages 
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ASprintCharacter

ASprintCharacter::ASprintCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// Changed this to EAttachLocation::KeepWorldPosition, due to it originally not allowing us to rotate the gun
	FP_Gun->AttachTo(Mesh1P, TEXT("GripPoint"), EAttachLocation::KeepWorldPosition, true);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->AttachTo(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	CurrentStamina = InitialStamina;

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ASprintCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FP_Gun->AttachTo(Mesh1P, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true); //Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
}

void ASprintCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentStamina > 0)
	{
		UpdateCurrentStamina(-DeltaTime * RateOfStaminaDecay * InitialStamina);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Your stamina is %f"), GetCurrentStamina()));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASprintCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Raycast", IE_Pressed, this, &ASprintCharacter::PerformRaycast);

	InputComponent->BindAction("Interact", IE_Pressed, this, &ASprintCharacter::BeginInteract);
	InputComponent->BindAction("Interact", IE_Released, this, &ASprintCharacter::EndInteract);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &ASprintCharacter::BeginSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ASprintCharacter::EndSprint);

	InputComponent->BindAction("Pickup", IE_Pressed, this, &ASprintCharacter::BeginPickup);
	InputComponent->BindAction("Pickup", IE_Released, this, &ASprintCharacter::EndPickup);
	InputComponent->BindAction("ShowInventory", IE_Pressed, this, &ASprintCharacter::ShowInventory);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Save", IE_Pressed, this, &ASprintCharacter::SaveGame);
	InputComponent->BindAction("Load", IE_Pressed, this, &ASprintCharacter::LoadGame);

	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASprintCharacter::TouchStarted);
	if (EnableTouchscreenMovement(InputComponent) == false)
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &ASprintCharacter::OnFire);
	}

	InputComponent->BindAxis("MoveForward", this, &ASprintCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASprintCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ASprintCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ASprintCharacter::LookUpAtRate);
}

void ASprintCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			//World->SpawnActor<ASprintProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

	//Raycast code
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = ForwardVector * 2000.f + StartTrace;
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	if (GetWorld()->LineTraceSingle(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, 5.f);

		ATarget* TestTarget = Cast<ATarget>(HitResult->Actor.Get());
		if (TestTarget && !TestTarget->IsPendingKill())
		{ 
			TestTarget->DamageTarget(BulletDamage);
		}
	}	
}

void ASprintCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ASprintCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

void ASprintCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void ASprintCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		if (bIsSprinting)
			Value *= SprintRate;

		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value / 2.5);
	}
}

void ASprintCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		if (bIsSprinting)
			Value *= SprintRate;

		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value / 2.5);
	}
}

void ASprintCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASprintCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ASprintCharacter::EnableTouchscreenMovement(class UInputComponent* InputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASprintCharacter::BeginTouch);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &ASprintCharacter::EndTouch);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ASprintCharacter::TouchUpdate);
	}
	return bResult;
}

void ASprintCharacter::BeginSprint()
{
	// Speed handled in the MoveRight and MoveForward methods 
	bIsSprinting = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("YOU ARE SPRINTING Mofo"));
}

void ASprintCharacter::EndSprint()
{
	bIsSprinting = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NOT SPRINTING"));
}

void ASprintCharacter::BeginPickup()
{
	/*
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Begin Pickup"));
	*/

	bIsPickingUp = true;
}

void ASprintCharacter::EndPickup()
{
	/*
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("End Pickup"));
	*/

	bIsPickingUp = false;
}

void ASprintCharacter::BeginInteract()
{
	bIsInteracting = true;
}

void ASprintCharacter::EndInteract()
{
	bIsInteracting = false;
}


void ASprintCharacter::ShowInventory()
{
	
	for (auto& Item : Inventory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%s"), *Item));
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Inventory:"));
}

float ASprintCharacter::GetInitialStamina()
{
	return InitialStamina;
}

float ASprintCharacter::GetCurrentStamina()
{
	return CurrentStamina;
}

void ASprintCharacter::UpdateCurrentStamina(float Stamina)
{
	CurrentStamina += Stamina;
}

void ASprintCharacter::SaveGame()
{
	/* NOTE WE ARE ONLY SAVING THE LOCATION, WE COULD SAVE ANYTHING WE WANT LIKE THE DIRECTION WE ARE FACING OR GOLD OR WHATEVER*/

	// Create an instance of our save game class
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	//Set the save game instance location to the players current location
	SaveGameInstance->PlayerLocation = this->GetActorLocation();
	// Save the savegameinstance
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);
	//Log a message to show we saved the game
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Save"));
}

void ASprintCharacter::LoadGame()
{
	// Create an instance of our save game class
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	// Load the saved game into our savegameinstance variable
	SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));
	// Set the players position from the saved game file
	this->SetActorLocation(SaveGameInstance->PlayerLocation);
	// log a message to show we have loaded the game
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Load Game"));
}

void ASprintCharacter::PerformRaycast()
{
	
	FHitResult* HitResult = new FHitResult();

	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	//Raycast will travel 5000 units 
	
	FVector EndTrace = ((ForwardVector * 5000) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		// if we pass false the lines will not stay forever
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitResult->Actor->GetName()));
	}
}