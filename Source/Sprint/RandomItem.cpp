// Fill out your copyright notice in the Description page of Project Settings.

#include "Sprint.h"
#include "RandomItem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARandomItem::ARandomItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = MyMesh;
}

// Called when the game starts or when spawned
void ARandomItem::BeginPlay()
{
	Super::BeginPlay();
	
	MyMesh->SetRelativeRotation(UKismetMathLibrary::RandomRotator());
}

// Called every frame
void ARandomItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

/*
FRotator ARandomItem::GetRandomRotator()
{
	FRotator MyRandomRotator = FRotator(0.f, 0.f, 0.f);

	MyRandomRotator.Pitch = FMath::RandRange(0.f, 360.f);
	MyRandomRotator.Yaw = FMath::RandRange(0.f, 360.f);
	MyRandomRotator.Roll = FMath::RandRange(0.f, 360.f);

	return MyRandomRotator;
}
*/