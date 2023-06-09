// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRST_API IInventoryInterface
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual	void AddActor(class AItem* Actor) = 0;
};
