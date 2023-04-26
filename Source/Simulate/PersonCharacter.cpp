// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonCharacter.h"

// Sets default values
APersonCharacter::APersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisionCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("VisionCollider"));
	VisionCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

APersonCharacter::~APersonCharacter()
{
	delete PersonData;
}

// Called when the game starts or when spawned
void APersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APersonCharacter::SomethingSeenCallback(AActor* seenActor)
{
	if (!IsObjectWithinVisionCone(GetActorTransform(), seenActor->GetActorLocation()))
	{
		return;
	}

	if (seenActor->IsA(APickupObject::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("PICK SEEN"));
		_ActiveSeenPickupObjects.Enqueue(Cast<APickupObject>(seenActor));
		HasPickUpTarget = true;
	}
	else if (seenActor->IsA(APersonCharacter::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("ENEMY SEEN"));
		_ActiveSeenEnemies.Enqueue(Cast<APersonCharacter>(seenActor));
		HasEnemyTarget = true;
	}
}

void APersonCharacter::SomethingSeenExitCallback(AActor* seenActor)
{
	if (seenActor->IsA(APickupObject::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("PICK EXIT"));
		//_ActiveSeenObjects.Pop();
	}
}

void APersonCharacter::MoveCallback()
{
	//UE_LOG(LogTemp, Warning, TEXT("Completed move"));
	if (_TaskCallback)
	{
		_TaskCallback(this);
	}
}

// Called every frame
void APersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentEnemy)
	{
		if (IsEnemyWithinKillCone(GetActorTransform(), CurrentEnemy->GetActorLocation()))
		{
			EnemyKilled();
		}
	}
}

// Called to bind functionality to input
void APersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APersonCharacter::RegisterTaskCallback(TFunction<void(APersonCharacter*)> callback)
{
	_TaskCallback = callback;
}

APickupObject* APersonCharacter::GetRelevantPickup()
{
	APickupObject* returnObj = nullptr;
	if (!_ActiveSeenPickupObjects.IsEmpty())
	{
		_ActiveSeenPickupObjects.Dequeue(returnObj);
	}
	if (!returnObj)
	{
		HasPickUpTarget = false;
	}
	return returnObj;
}

APersonCharacter* APersonCharacter::GetRelevantEnemy()
{
	APersonCharacter* returnObj = nullptr;
	if (!_ActiveSeenEnemies.IsEmpty())
	{
		_ActiveSeenEnemies.Dequeue(returnObj);
	}
	return returnObj;
}

void APersonCharacter::PickupObject(APickupObject* pickupObj)
{
	if (pickupObj)
	{
		PersonData->AddToInventory(pickupObj->PickUpType, 1);
		GetWorld()->DestroyActor(pickupObj);
	}
	HasPickUpTarget = false;
	if (_TaskCallback)
	{
		_TaskCallback(this);
	}
}


//Myabe change to track enemy
//Probably need to look at enemy
//Make sure they can hit them
//Then destroy them
void APersonCharacter::AttackEnemy(APersonCharacter* enemy)
{	
	StopMovement();
	if (enemy)
	{
		CurrentEnemy = enemy;
	}
}

bool APersonCharacter::IsObjectWithinVisionCone(FTransform selfTrans, FVector objLocation)
{
	FRotator rotator = UKismetMathLibrary::FindRelativeLookAtRotation(selfTrans, objLocation);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), rotator.Yaw);
	if (FMath::Abs(rotator.Yaw) < VISION_CONE_SIZE)
	{
		return true;
	}
	return false;
}

bool APersonCharacter::IsEnemyWithinKillCone(FTransform selfLocation, FVector objLocation)
{
	FRotator rotator = UKismetMathLibrary::FindRelativeLookAtRotation(selfLocation, objLocation);
	//UE_LOG(LogTemp, Warning, TEXT("Check %f"),rotator.Yaw);
	if (FMath::Abs(rotator.Yaw) < KILL_CONE_SIZE)
	{
		return true;
	}
	return false;
}

void APersonCharacter::EnemyKilled()
{
	if (IsDead)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("KILL"));
	HasEnemyTarget = false;
	bool HasWon = false;

	ASimulateGameStateBase* gameStateRef = (ASimulateGameStateBase*)GetWorld()->GetGameState();

	if (CurrentEnemy)
	{
		HasWon = (gameStateRef->RemoveFromAliveList(CurrentEnemy->PersonData->PersonID));
		CurrentEnemy->Death();
		CurrentEnemy->StopMovement();
		CurrentEnemy->IsDead = true;
		CurrentEnemy = nullptr;
		PersonData->AddToInventory(TEXT("KILL"), 1);
	}

	if (HasWon)
	{
		StopMovement();
		gameStateRef->SetWinner(PersonData->PersonID);
		UE_LOG(LogTemp, Warning, TEXT("WINNER"));
	}

	//let task manager handle it?
	//if (_TaskCallback)
	//{
	//	_TaskCallback(this);
	//}
}

