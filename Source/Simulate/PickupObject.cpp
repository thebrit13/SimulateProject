// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupObject.h"

// Sets default values
APickupObject::APickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ModelParent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshParent"));
	SetRootComponent(ModelParent);

	CollisionParent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionParent"));
	CollisionParent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APickupObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

