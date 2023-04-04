// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PickupObject.generated.h"


UCLASS()
class SIMULATE_API APickupObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ModelParent;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CollisionParent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
