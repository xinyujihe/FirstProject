// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryUI.h"
#include "HUD/InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "ActorComponent/Item.h"

void UInventoryUI::UiUpdate(TArray<TSubclassOf<class AItem>> InventoryArray)
{
	if (InventorySlot)
	{
		GridPanel->ClearChildren();
		UInventorySlot* SlotUI;
		for (auto& ItemActor : InventoryArray)
		{
			SlotUI = CreateWidget<UInventorySlot>(this, InventorySlot);
			SlotUI->UpdateSlot(ItemActor);
			if (Length)
			{
				GridPanel->AddChildToUniformGrid(SlotUI, GridPanel->GetChildrenCount() / Length, GridPanel->GetChildrenCount() % Length);
			}
		}
	}
	
}
