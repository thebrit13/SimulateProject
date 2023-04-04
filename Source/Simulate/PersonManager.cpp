// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonManager.h"

// Sets default values
APersonManager::APersonManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("PersonManagerParent")));

	USceneComponent* sp1 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint1"));
	sp1->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnPoints.Add(sp1);

	USceneComponent* sp2 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint2"));
	sp2->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnPoints.Add(sp2);
}

// Called when the game starts or when spawned
void APersonManager::BeginPlay()
{
	Super::BeginPlay();
	
	CreatedPeople.Add(GetWorld()->SpawnActor<APersonCharacter>(Person1, SpawnPoints[0]->GetComponentLocation(), FRotator::ZeroRotator));
	CreatedPeople.Add(GetWorld()->SpawnActor<APersonCharacter>(Person1, SpawnPoints[1]->GetComponentLocation(), FRotator::ZeroRotator));

	for (int i = 0; i < CreatedPeople.Num(); i++)
	{
		TaskManager->RegisterPerson(CreatedPeople[i]);
	}
}

// Called every frame
void APersonManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

