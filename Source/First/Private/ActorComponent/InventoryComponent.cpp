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
		//ETraceTypeQuery::ECC_GameTraceChannel1,//跟踪类型查询，一般为可见性追踪。这个参数并不重要。可以自己自定义查询类型
		UEngineTypes::ConvertToTraceType(TRACE_Inventory),
		false,//如果为 true，则针对复杂碰撞进行测试，如果为 false，则针对简化碰撞进行测试。
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,//绘制Debug检测
		BoxHit,
		true,//是否忽略自己
		FLinearColor::Green,
		FLinearColor::Red
	);
	if (BoxHit.GetActor())
	{

	}
}

