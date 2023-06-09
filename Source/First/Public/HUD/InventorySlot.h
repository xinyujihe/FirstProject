// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UButton;
class UImage;
class AItem;

UCLASS()
class FIRST_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
private:

	

protected:
	UPROPERTY(meta = (BindWidget), BluePrintReadOnly)
	UButton* Button;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(BluePrintReadOnly)
	TSubclassOf<AItem>HeldItem;
public:

	void UpdateSlot(TSubclassOf<AItem>ItemtoUpdate);
};
