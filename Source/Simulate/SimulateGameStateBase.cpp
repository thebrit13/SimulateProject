// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulateGameStateBase.h"

void ASimulateGameStateBase::SetWinner(FString personID)
{
	_WinnerList.Add(FString::Printf(TEXT("GAME_%d"), _GameID), personID);
	if (_GameOverCallback)
	{
		_GameOverCallback(personID);
	}
}

void ASimulateGameStateBase::AddToAliveList(FString playerID)
{
	_AliveList.Add(playerID);
}

bool ASimulateGameStateBase::RemoveFromAliveList(FString personID)
{
	_AliveList.Remove(personID);
	if (_AliveList.Num() == 1)
	{
		return true;
	}
	return false;
}
