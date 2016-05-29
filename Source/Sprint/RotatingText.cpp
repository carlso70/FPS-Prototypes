// Fill out your copyright notice in the Description page of Project Settings.

#include "Sprint.h"
#include "RotatingText.h"
#include "SprintCharacter.h"

// Sets default values
ARotatingText::ARotatingText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("RotatingText"));
	MyText->SetWorldSize(70.f);
	MyText->SetTextRenderColor(FColor::Red);
	MyText->SetHorizontalAlignment(EHTA_Center);

	MyText->SetText("Testing");
}

// Called when the game starts or when spawned
void ARotatingText::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotatingText::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	MakeTextFacePlayer();
}

void ARotatingText::MakeTextFacePlayer()
{
	ASprintCharacter* Character = Cast<ASprintCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	FRotator NewRoation = Character->GetActorRotation();
	//add 180 because by default its Yaw is backwards 
	NewRoation.Yaw += 180.f;

	MyText->SetRelativeRotation(NewRoation);
}