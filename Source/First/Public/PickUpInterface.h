// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickUpInterface : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class FIRST_API IPickUpInterface 
{
    GENERATED_BODY()
       // 将接口函数添加到此类。这是将会被继承以实现此接口的类
public:

    //UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = PickUpInterface)
    //void AddItem(class AItem* Item);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = PickUpInterface)
    void AddItemToInventory(class AItem* Item);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = PickUpInterface)
    void SpawnItemFromActor(TSubclassOf<class AItem> Item, FTransform SpawnTransform);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = PickUpInterface)
    void SetOverlappingItem(class AItem* Item);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = PickUpInterface)
    void RemoveOverlappingItem(class AItem* Item);

};
