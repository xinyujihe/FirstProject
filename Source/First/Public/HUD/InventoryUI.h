// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

/**
 * 
 */
class UUniformGridSlot;
class UUniformGridPanel;
class UInventorySlot;

UCLASS()
class FIRST_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), VisibleAnywhere)
	UUniformGridPanel* GridPanel;

	
public:

	UFUNCTION(BlueprintCallable)
	void UiUpdate(TArray<TSubclassOf<class AItem>> InventoryArray);

	
private:
	UPROPERTY(EditAnywhere, Category = Categoryname)
	TSubclassOf<UInventorySlot> InventorySlot;

	//背包每行存储的数量
	UPROPERTY(EditAnywhere, Category = Categoryname)
	int32 Length = 6;

	
};
