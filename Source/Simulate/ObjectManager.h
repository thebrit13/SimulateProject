// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupObject.h"
#include "Components/SceneComponent.h"
#include "ObjectManager.generated.h"

UCLASS()
class SIMULATE_API AObjectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APickupObject> PickUpObject1;

	UPROPERTY(EditInstanceOnly)
	USceneComponent* SpawnLocationTL;

	UPROPERTY(EditInstanceOnly)
	USceneComponent* SpawnLocationBR;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	const int MAX_SPAWN_COUNT = 10;
	const int MIN_SPAWN_COUNT = 5;

	void CreateObjects();

};
