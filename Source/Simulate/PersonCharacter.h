// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "PickupObject.h"
#include "PersonCharacter.generated.h"

UCLASS()
class SIMULATE_API APersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APersonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(EditDefaultsOnly)
	//float VisionDistance = 100;

	UFUNCTION(BlueprintCallable)
	void SomethingSeenCallback(AActor* seenActor);

	UFUNCTION(BlueprintCallable)
	void SomethingSeenExitCallback(AActor* seenActor);

	UFUNCTION(BlueprintCallable)
	void MoveCallback();

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* VisionCollider;
	
	//UCapsuleComponent* HearingCollider;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void RegisterTaskCallback(TFunction<void(APersonCharacter*)> callback);

	APickupObject* GetRelevantPickup();

	void PickupObject(APickupObject* pickupObj);

	UFUNCTION(BlueprintImplementableEvent)
	void MoveToLocation(const FVector& loc);

	UFUNCTION(BlueprintImplementableEvent)
	void MoveToRandomLocation();

	UFUNCTION(BlueprintImplementableEvent)
	void StopMovement();

	bool HasPickUpTarget = false;

private:
	TQueue<APickupObject*> _ActiveSeenObjects;

	TFunction<void(APersonCharacter*)> _TaskCallback;
};
