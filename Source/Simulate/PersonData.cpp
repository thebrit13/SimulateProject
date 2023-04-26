// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonData.h"

PersonData::PersonData(FString personID,FString name)
{
	this->PersonID = personID;
	this->_Name = name;
}

PersonData::~PersonData()
{
}

void PersonData::AddToInventory(FString id, int amt)
{
	if (_Inventory.Contains(id))
	{
		_Inventory[id] += amt;
	}
	else
	{
		_Inventory.Add(id, amt);
	}
	
}

bool PersonData::RemoveFromInventory(FString id, int amt)
{
	if (_Inventory.Contains(id))
	{
		_Inventory[id] -= amt;
		return true;
	}
	else
	{
		return false;
	}
}
