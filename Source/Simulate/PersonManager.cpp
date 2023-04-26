// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonManager.h"

// Sets default values
APersonManager::APersonManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("PersonManagerParent")));

	SpawnPointTL = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPointTL"));
	SpawnPointTL->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SpawnPointBR = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPointBR"));
	SpawnPointBR->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APersonManager::BeginPlay()
{
	Super::BeginPlay();
	_GameStateRef = (ASimulateGameStateBase*)GetWorld()->GetGameState();

	_GameStateRef->SetGameOverCallback(([this](FString winnerID) {
			SpawnPeople(false);
		}));
	_GameStateRef->StartNewGame();
	
	//CreatedPeople.Add(GetWorld()->SpawnActor<APersonCharacter>(Person1, SpawnPoints[0]->GetComponentLocation(), FRotator::ZeroRotator));
	//CreatedPeople.Add(GetWorld()->SpawnActor<APersonCharacter>(Person1, SpawnPoints[1]->GetComponentLocation(), FRotator::ZeroRotator));
	SpawnPeople(true);

}

// Called every frame
void APersonManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APersonManager::SpawnPeople(bool firstGame)
{
	FVector topLeftExtent = SpawnPointTL->GetComponentLocation();
	FVector bottomRightExtent = SpawnPointBR->GetComponentLocation();

	for (int i = 0; i < FMath::RandRange(MIN_PEOPLE_SPAWN, MAX_PEOPLE_SPAWN); i++)
	{
		FVector spawnLocation = FVector(FMath::RandRange(topLeftExtent.X, bottomRightExtent.X), FMath::RandRange(topLeftExtent.Y, bottomRightExtent.Y), 0);
		APersonCharacter* tempChar = GetWorld()->SpawnActor<APersonCharacter>(Person1, spawnLocation, FRotator::ZeroRotator);

		if (!tempChar)
		{
			continue;
		}

		tempChar->PersonData = new PersonData(FString::Printf(TEXT("PERSON_%d"), _RunningIDCount), FString::Printf(TEXT("NAME_PERSON_%d"), _RunningIDCount));

		CreatedPeople.Add(tempChar);
		TaskManager->RegisterPerson(tempChar);
		_GameStateRef->AddToAliveList(tempChar->PersonData->PersonID);

		_RunningIDCount++;
	}
}

