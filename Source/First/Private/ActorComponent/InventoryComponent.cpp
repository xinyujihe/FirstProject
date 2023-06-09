// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/InventoryUI.h"
#include "Kismet/KismetArrayLibrary.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//ShpereTrace();
}

void UInventoryComponent::AddItemToInventory_Implementation(AItem* Item)
{
	InventoryArray.Add(Item->GetClass());
	if (InventoryHUD)
	{
		InventoryHUD->UiUpdate(InventoryArray);
	}
}

void UInventoryComponent::SpawnItemFromActor_Implementation(TSubclassOf<class AItem> Item, FTransform SpawnTransform)
{
	GetWorld()->SpawnActor<AItem>(Item, SpawnTransform);
	int32 Idenx = InventoryArray.Find(Item);
	if (Idenx >=0)
	{
		InventoryArray.RemoveAt(Idenx);
		FString TStr = FString::FromInt(Idenx);
		GEngine->AddOnScreenDebugMessage(7, 2.0f, FColor::Green, FString::Printf(TEXT("Idenx is: %d"), Idenx));
	}
	InventoryHUD->UiUpdate(InventoryArray);
}

void UInventoryComponent::SetOverlappingItem_Implementation(AItem* Item)
{
	CanPickItem.AddUnique(Item);
	OverlappingItem = Item;
}

void UInventoryComponent::RemoveOverlappingItem_Implementation(AItem* Item)
{
	CanPickItem.Remove(Item);
	OverlappingItem = nullptr;
}

void UInventoryComponent::ShowPickUpUI()
{
	if (Controller && PickUpUIClass)
	{
		bool IsLocalPlayer = Controller->IsLocalPlayerController();
		if (IsLocalPlayer)
		{
			if (!PickUpUI)
			{
				PickUpUI = CreateWidget<UUserWidget>(Controller, PickUpUIClass);
			}
			PickUpUI->AddToViewport();
		}
	}
}

void UInventoryComponent::HiddenPickUpUI()
{
	if (PickUpUI)
	{
		PickUpUI->RemoveFromParent();
	}
}

void UInventoryComponent::PickUpActorByKey()
{
	if (OverlappingItem)
	{
		AddItemToInventory_Implementation(OverlappingItem);
		UGameplayStatics::SpawnSoundAtLocation(this, OverlappingItem->GetSound(), OverlappingItem->GetActorLocation());
		CanPickItem.Remove(OverlappingItem);
		OverlappingItem->Destroy();
		if (CanPickItem.Num()&& OverlappingItem ==nullptr)
		{
			const int32 num = FMath::RandRange(0, CanPickItem.Num() - 1);
			OverlappingItem = CanPickItem[num];
		}
	}
	
}

void UInventoryComponent::ShowInventoryHUD()
{
	if (Controller && InventoryUIClass)
	{
		bool IsLocalPlayer = Controller->IsLocalPlayerController();
		if (IsLocalPlayer)
		{
			if (!InventoryHUD)
			{
				InventoryHUD = CreateWidget<UInventoryUI>(Controller, InventoryUIClass);
			}
			InventoryHUD->UiUpdate(InventoryArray);
			InventoryHUD->AddToViewport();
			if (BController)
			{
				Controller->SetInputMode(FInputModeGameAndUI());
				Controller->bShowMouseCursor = true;
			}
		}
	}

}

void UInventoryComponent::InitializeController(APlayerController* PlayerController)
{
	if (!Controller)
	{
		Controller = PlayerController;
	}
}

void UInventoryComponent::HiddenInventoryHUD()
{
	if (InventoryHUD)
	{
		InventoryHUD->RemoveFromParent();
		Controller->SetInputMode(FInputModeGameOnly());
		Controller->bShowMouseCursor = false;
	}
}

void UInventoryComponent::ShpereTrace()
{
	const FVector Start = GetOwner()->GetActorLocation();
	const FVector End = Start + GetOwner()->GetActorForwardVector() * TraceLength;
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : ActorsToIgnore)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		TraceRadius,
		//ETraceTypeQuery::ECC_GameTraceChannel1,//�������Ͳ�ѯ��һ��Ϊ�ɼ���׷�١��������������Ҫ�������Լ��Զ����ѯ����
		UEngineTypes::ConvertToTraceType(TRACE_Inventory),
		false,//���Ϊ true������Ը�����ײ���в��ԣ����Ϊ false������Լ���ײ���в��ԡ�
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,//����Debug���
		BoxHit,
		true,//�Ƿ�����Լ�
		FLinearColor::Green,
		FLinearColor::Red
	);
	if (BoxHit.GetActor())
	{

	}
}

