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
			//for now
			if (personLoop->CurrentTask && personLoop->CurrentTask->ObjectTaskType == TaskType::ATTACK)
			{
				personLoop->TaskQueue.Empty();
			}

			delete personLoop->CurrentTask;
			personLoop->CurrentTask = nullptr;
		}
	}
}

void ATaskManager::RemovePerson(APersonCharacter* person)
{
	FPersonTaskObject* fto = nullptr;
	for (FPersonTaskObject* ftoIn : _RegisteredPeople)
	{
		if (ftoIn->Person == person)
		{
			fto = ftoIn;
			break;
		}
	}
	if (fto)
	{
		_RegisteredPeople.Remove(fto);
	}
	
}

void ATaskManager::UpdateTasks()
{
	//UE_LOG(LogTemp, Warning, TEXT("Updating tasks"));
	//Do they see anything
	//Do they hear anything
	//If yes, clear all tasks and move towards what they see/hear

	for (FPersonTaskObject* personObject : _RegisteredPeople)
	{
		if (personObject->Person && personObject->Person->IsDead)
		{
			//UE_LOG(LogTemp, Warning, TEXT("SKIP"));
			continue;
		}

		if (!personObject->CurrentTask || (
			personObject->Person->HasEnemyTarget && personObject->CurrentTask && personObject->CurrentTask->ObjectTaskType != TaskType::ATTACK))
		{
			//if enemy, clear queue and attack!
			if (personObject->Person->HasEnemyTarget)
			{
				personObject->TaskQueue.Empty();
			}

			if (personObject->TaskQueue.IsEmpty())
			{
				//See enemy
				//Stop moving, track enemy, and fire
				if (personObject->Person->HasEnemyTarget)
				{
					APersonCharacter* enemy = personObject->Person->GetRelevantEnemy();
					if (enemy)
					{
						FTaskObject* newTask = new FTaskObject(TaskType::ATTACK,enemy);
						personObject->TaskQueue.Enqueue(newTask);
					}
				}
				//See Pickup
				//Move to it, then wait
				else if (personObject->Person->HasPickUpTarget)
				{
					APickupObject* pickup = personObject->Person->GetRelevantPickup();
					if (pickup)
					{
						FTaskObject* newTask = new FTaskObject(TaskType::MOVING, pickup->GetActorLocation());
						FTaskObject* newTask2 = new FTaskObject(TaskType::PICKUP, pickup, 1.0f);
						personObject->TaskQueue.Enqueue(newTask);
						personObject->TaskQueue.Enqueue(newTask2);
					}
				}
				//Other tasks
			}

			//Nothing to do, just walk around
			if (personObject->TaskQueue.IsEmpty())
			{
				FTaskObject* newTask2 = new FTaskObject(TaskType::MOVING_RANDOM, 5.0f);
				personObject->TaskQueue.Enqueue(newTask2);
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
				//Small note, currently only accepts ints for time in seconds
				personObject->CurrentTask->FinishTime = FDateTime::Now() + FTimespan(0, 0, personObject->CurrentTask->TaskTime);
				break;
			case MOVING:
				personObject->Person->MoveToLocation(personObject->CurrentTask->Destination);
				break;
			case MOVING_RANDOM:
				personObject->CurrentTask->FinishTime = FDateTime::Now() + FTimespan(0, 0, personObject->CurrentTask->TaskTime);
				personObject->Person->MoveToRandomLocation();
				break;
			case ATTACK:
				personObject->Person->AttackEnemy(personObject->CurrentTask->EnemyObject);
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
					//UE_LOG(LogTemp, Warning, TEXT("Wait task is complete"));
					TaskCallback(personObject->Person);
				}
			}
			else if (personObject->CurrentTask->ObjectTaskType == TaskType::ATTACK)
			{
				if (!personObject->CurrentTask->EnemyObject || (personObject->CurrentTask->EnemyObject && personObject->CurrentTask->EnemyObject->IsDead))
				{
					//UE_LOG(LogTemp, Warning, TEXT("Pickup task is cancelled"));
					//personObject->Person->PickupObject(personObject->CurrentTask->PickupObject);
					TaskCallback(personObject->Person);
				}
			}
			else if (personObject->CurrentTask->ObjectTaskType == TaskType::PICKUP)
			{
				if (FDateTime::Now() >= personObject->CurrentTask->FinishTime)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Pickup task is complete"));
					personObject->Person->PickupObject(personObject->CurrentTask->PickupObject);
					TaskCallback(personObject->Person);
				}
				else if (!personObject->CurrentTask->PickupObject)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Pickup task is cancelled"));
					personObject->Person->PickupObject(personObject->CurrentTask->PickupObject);
					TaskCallback(personObject->Person);
				}
			}
			else if (personObject->CurrentTask->ObjectTaskType == TaskType::MOVING_RANDOM)
			{
				if (personObject->Person->HasPickUpTarget || personObject->Person->HasEnemyTarget ||
					FDateTime::Now() >= personObject->CurrentTask->FinishTime)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Random move is complete"));
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

