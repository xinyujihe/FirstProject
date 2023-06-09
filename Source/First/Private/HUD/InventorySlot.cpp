// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventorySlot.h"
#include "Components/Image.h"
#include "ActorComponent/Item.h"

void UInventorySlot::UpdateSlot(TSubclassOf<AItem>ItemtoUpdate)
{
	HeldItem = ItemtoUpdate;
	AItem* DefaultActor = Cast<AItem>(ItemtoUpdate->GetDefaultObject(true));
	ItemImage->SetBrushFromTexture(DefaultActor->GetItemDetails().ItemIcon);
}
