// Fill out your copyright notice in the Description page of Project Settings.

#include "TaskManager.h"

// Sets default values
ATaskManager::ATaskManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATaskManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATaskManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_CurrentTime += DeltaTime;
	if (_CurrentTime >= UPDATE_TIME)
	{
		UpdateTasks();
		_CurrentTime = 0;
	}
}

void ATaskManager::RegisterPerson(APersonCharacter* person)
{
	FPersonTaskObject* personTask = new FPersonTaskObject();
	personTask->Person = person;
	person->RegisterTaskCallback([this](APersonCharacter* personCallback){
		TaskCallback(personCallback);
		});
	_RegisteredPeople.Add(personTask);
}

void ATaskManager::TaskCallback(APersonCharacter* person)
{
	for (FPersonTaskObject* personLoop : _RegisteredPeople)
	{
		if (personLoop->Person == person)
		{
			delete personLoop->CurrentTask;
			personLoop->CurrentTask = nullptr;
		}
	}
}

void ATaskManager::UpdateTasks()
{
	//UE_LOG(LogTemp, Warning, TEXT("Updating tasks"));
	//Do they see anything
	//Do they hear anything
	//If yes, clear all tasks and move towards what they see/hear

	for (FPersonTaskObject* personObject:_RegisteredPeople)
	{
		if (!personObject->CurrentTask)
		{
			if (personObject->TaskQueue.IsEmpty())
			{
				//See Pickup
				//Move to it, then wait
				APickupObject* pickup = personObject->Person->GetRelevantPickup();
				if (pickup)
				{
					FTaskObject* newTask = new FTaskObject(TaskType::MOVING, pickup->GetActorLocation());
					FTaskObject* newTask2 = new FTaskObject(TaskType::PICKUP, pickup,.25f);
					personObject->TaskQueue.Enqueue(newTask);
					personObject->TaskQueue.Enqueue(newTask2);
				}
				//Other tasks
				//last will be random walk
				else
				{
					FTaskObject* newTask2 = new FTaskObject(TaskType::MOVING_RANDOM,5.0f);
					personObject->TaskQueue.Enqueue(newTask2);
				}
			}

			//Nothing to do, just return for now
			if (personObject->TaskQueue.IsEmpty())
			{
				continue;
			}

			//Execute new task
			//Might need to handle deleting current task if we have tasks to override everything
			personObject->TaskQueue.Dequeue(personObject->CurrentTask);

			switch (personObject->CurrentTask->ObjectTaskType)
			{
				case NONE:
					break;
				case WAIT:
				case PICKUP:
					personObject->CurrentTask->FinishTime = FDateTime::Now() + FTimespan(0, 0, personObject->CurrentTask->TaskTime);
					break;
				case MOVING:
					personObject->Person->MoveToLocation(personObject->CurrentTask->Destination);
					break;
				case MOVING_RANDOM:
					personObject->CurrentTask->FinishTime = FDateTime::Now() + FTimespan(0, 0, personObject->CurrentTask->TaskTime);
					personObject->Person->MoveToRandomLocation();
					break;
				default:
					break;
			}
		}
		else
		{
			//Check for wait tasks
			if (personObject->CurrentTask->ObjectTaskType == TaskType::WAIT)
			{
				if (FDateTime::Now() >= personObject->CurrentTask->FinishTime)
				{
					UE_LOG(LogTemp, Warning, TEXT("Wait task is complete"));
					TaskCallback(personObject->Person);
				}
			}
			else if (personObject->CurrentTask->ObjectTaskType == TaskType::PICKUP)
			{
				if (FDateTime::Now() >= personObject->CurrentTask->FinishTime)
				{
					UE_LOG(LogTemp, Warning, TEXT("Pickup task is complete"));
					personObject->Person->PickupObject(personObject->CurrentTask->PickupObject);
					TaskCallback(personObject->Person);
				}
				else if (!personObject->CurrentTask->PickupObject)
				{
					UE_LOG(LogTemp, Warning, TEXT("Pickup task is cancelled"));
					personObject->Person->PickupObject(personObject->CurrentTask->PickupObject);
					TaskCallback(personObject->Person);
				}
			}
			else if (personObject->CurrentTask->ObjectTaskType == TaskType::MOVING_RANDOM)
			{
				if (personObject->Person->HasPickUpTarget ||
					FDateTime::Now() >= personObject->CurrentTask->FinishTime)
				{
					UE_LOG(LogTemp, Warning, TEXT("Random move is complete"));
					personObject->Person->StopMovement();
					TaskCallback(personObject->Person);
				}
			}
		}

		//switch (personTask.CurrentStatus)
		//{
		//	NONE:
		//		break;
		//	WALKING:
		//		break;
		//	default:
		//		break;
		//}
	}
}

