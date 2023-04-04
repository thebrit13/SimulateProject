// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PersonCharacter.h"
#include "Containers/Queue.h"
#include "TaskManager.generated.h"

UENUM()
enum TaskType
{
	NONE,
	MOVING_RANDOM,
	MOVING,
	PICKUP,
	WAIT
};

struct FTaskObject
{
	TaskType ObjectTaskType = NONE;
	FVector Destination = FVector::ZeroVector;
	APickupObject* PickupObject;
	float TaskTime = -1;
	FDateTime FinishTime;

	FTaskObject(TaskType tt, APickupObject* pickupObject,float taskTime)
	{
		ObjectTaskType = tt;
		PickupObject = pickupObject;
		TaskTime = taskTime;
	}

	FTaskObject(TaskType tt, FVector destination)
	{
		ObjectTaskType = tt;
		Destination = destination;
	}

	FTaskObject(TaskType tt, float taskTime)
	{
		ObjectTaskType = tt;
		TaskTime = taskTime;
	}

	FTaskObject(TaskType tt)
	{
		ObjectTaskType = tt;
	}
};

struct FPersonTaskObject
{
	APersonCharacter* Person = nullptr;
	FTaskObject* CurrentTask = nullptr;
	TQueue<FTaskObject*> TaskQueue;
};



UCLASS()
class SIMULATE_API ATaskManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATaskManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RegisterPerson(APersonCharacter* person);

	void TaskCallback(APersonCharacter* person);

private:

	float _CurrentTime = 0;
	const float UPDATE_TIME = .1f;

	TArray<FPersonTaskObject*> _RegisteredPeople;

	void UpdateTasks();

};

