// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PersonCharacter.h"
#include "TaskManager.h"
#include "Components/SceneComponent.h"
#include "PersonManager.generated.h"

UCLASS()
class SIMULATE_API APersonManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APersonManager();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APersonCharacter> Person1;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SpawnPointTL;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SpawnPointBR;

	UPROPERTY(EditInstanceOnly)
	ATaskManager* TaskManager;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<APersonCharacter*> CreatedPeople;

	void SpawnPeople();

	const int MIN_PEOPLE_SPAWN = 10;
	const int MAX_PEOPLE_SPAWN = 20;

};
