// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SIMULATE_API PersonData
{
public:
	PersonData(FString personID, FString name);
	~PersonData();

	void AddToInventory(FString id, int amt);
	bool RemoveFromInventory(FString id, int amt);

	FString PersonID;

private:
	UPROPERTY();
	TMap<FString, int> _Inventory;

	FString _Name;
};
