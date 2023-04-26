// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SimulateGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATE_API ASimulateGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void SetWinner(FString playerID);

	void AddToAliveList(FString playerID);

	bool RemoveFromAliveList(FString playerID);

	void StartNewGame() { _GameID++; };

	void SetGameOverCallback(TFunction<void(FString)> callback) { _GameOverCallback = callback; };

private:
	UPROPERTY()
		TMap<FString, FString> _WinnerList;

	TArray<FString> _AliveList;

	int _GameID = 1;

	TFunction<void(FString)> _GameOverCallback;
};