// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PickupObject.generated.h"

const FString PICKUP_FOOD = "Food";
const FString PICKUP_WATER = "Water";
const FString PICKUP_AMMO = "Ammo";

UCLASS()
class SIMULATE_API APickupObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupObject();

	void Setup(FString ID, FString pType);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString ID;
	FString PickUpType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ModelParent;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CollisionParent;

};
