// Fill out your copyright notice in the Description page of Project Settings.

#include "Sprint.h"
#include "Target.h"
#include "Engine/DestructibleMesh.h"


// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 100;

	DestructComp = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Boom"));
	RootComponent = DestructComp;

	Smoke = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke"));
	Smoke->AttachTo(DestructComp);
	Smoke->SetVisibility(false);
	
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATarget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
}

void ATarget::DestroyTarget()
{
	FVector Location = GetActorLocation();
	//DestructComp->ApplyRadiusDamage(1.f, Location, 1.f, 100.f, true);
	FVector playloc = UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation();
	DestructComp->ApplyDamage(Damage, Location, playloc, ImpulseStrength);

	Smoke->SetVisibility(true);

	FTimerHandle Handle;
	// need to do destroy in delete method because SetTimer only allows void functions in params
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ATarget::Delete, 10.f, true);
}

void ATarget::PrintDebugs()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Testing testing sfasdfsdfsafdasdfsfad"));
}

void ATarget::Delete()
{
	Destroy();
}

void ATarget::DamageTarget(float Damage)
{
	Health -= Damage;
	if (Health <= 0)
	{
		DestroyTarget();
	}
}