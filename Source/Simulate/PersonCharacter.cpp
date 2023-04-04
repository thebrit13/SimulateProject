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

// Called when the game starts or when spawned
void APersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APersonCharacter::SomethingSeenCallback(AActor* seenActor)
{
	if (seenActor->IsA(APickupObject::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("PICK SEEN"));
		_ActiveSeenObjects.Enqueue(Cast<APickupObject>(seenActor));
		HasPickUpTarget = true;
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
	if (!_ActiveSeenObjects.IsEmpty())
	{
		_ActiveSeenObjects.Dequeue(returnObj);
	}
	return returnObj;
}

void APersonCharacter::PickupObject(APickupObject* pickupObj)
{
	if (pickupObj)
	{
		GetWorld()->DestroyActor(pickupObj);
	}
	HasPickUpTarget = false;
}

