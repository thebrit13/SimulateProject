// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "PickupObject.h"
#include "Kismet/KismetMathLibrary.h"
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

	APersonCharacter* GetRelevantEnemy();

	void PickupObject(APickupObject* pickupObj);

	UFUNCTION(BlueprintImplementableEvent)
	void MoveToLocation(const FVector& loc);

	UFUNCTION(BlueprintImplementableEvent)
	void MoveToRandomLocation();

	UFUNCTION(BlueprintImplementableEvent)
	void StopMovement();

	UFUNCTION(BlueprintImplementableEvent)
	void LookAtAndTryAndHitEnemy(const FVector& loc);

	UFUNCTION(BlueprintImplementableEvent)
	void Death();

	void AttackEnemy(APersonCharacter* enemy);

	bool HasPickUpTarget = false;
	bool HasEnemyTarget = false;
	bool IsDead = false;

	//UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	//bool IsTrackingEnemy = false;

	UPROPERTY(BlueprintReadOnly)
	APersonCharacter* CurrentEnemy = nullptr;

private:
	TQueue<APickupObject*> _ActiveSeenPickupObjects;
	TQueue<APersonCharacter*> _ActiveSeenEnemies;

	TFunction<void(APersonCharacter*)> _TaskCallback;

	bool IsObjectWithinVisionCone(FTransform selfTrans, FVector objLocation);

	bool IsEnemyWithinKillCone(FTransform selfTrans, FVector objLocation);

	const int VISION_CONE_SIZE = 90;
	const int KILL_CONE_SIZE = 5;

	void EnemyKilled();
};
