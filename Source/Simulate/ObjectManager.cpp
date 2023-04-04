// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectManager.h"

// Sets default values
AObjectManager::AObjectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("ObjectManagerParent")));

	SpawnLocationTL = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLocationTL"));
	SpawnLocationTL->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SpawnLocationBR = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLocationBR"));
	SpawnLocationBR->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AObjectManager::BeginPlay()
{
	Super::BeginPlay();
	
	CreateObjects();
}

// Called every frame
void AObjectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//At some point use a raycast to determine where to place
void AObjectManager::CreateObjects()
{
	FVector topLeftExtent = SpawnLocationTL->GetComponentLocation();
	FVector bottomRightExtent = SpawnLocationBR->GetComponentLocation();

	for (int i = 0; i < FMath::RandRange(MIN_SPAWN_COUNT, MAX_SPAWN_COUNT); i++)
	{
		FVector spawnLocation = FVector(FMath::RandRange(topLeftExtent.X, bottomRightExtent.X), FMath::RandRange(topLeftExtent.Y, bottomRightExtent.Y), 0);
		GetWorld()->SpawnActor<APickupObject>(PickUpObject1, spawnLocation, FRotator::ZeroRotator);
	}
}

