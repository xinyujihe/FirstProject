// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorComponent/Item.h"
#include "PickUpInterface.h"
#include "InventoryComponent.generated.h"

#define TRACE_Inventory ECC_GameTraceChannel1

class UInventoryUI;
class AItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRST_API UInventoryComponent : public UActorComponent, public IPickUpInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddItemToInventory_Implementation(class AItem* Item) override;
	void SpawnItemFromActor_Implementation(TSubclassOf<class AItem> Item, FTransform SpawnTransform) override;
	void SetOverlappingItem_Implementation(class AItem* Item) override;
	void RemoveOverlappingItem_Implementation(class AItem* Item) override;
	
private:

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UInventoryUI> InventoryUIClass;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> PickUpUIClass;

	UPROPERTY(EditDefaultsOnly, Category = Sets)
	float TraceLength = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = Sets)
	float TraceRadius;
	UPROPERTY(EditDefaultsOnly, Category = Sets)
	bool bShowBoxDebug;
	UPROPERTY(VisibleAnywhere, Category = Sets)
	TArray<AActor*> ActorsToIgnore;//模板类数组，数组内添加的Actor(AActor*)会被检测函数忽略

	UPROPERTY(VisibleAnywhere, Category = Sets)
	bool BController = true;//是否使用背包UI的控制器

	UPROPERTY(EditDefaultsOnly, Category = Sets)
	APlayerController* Controller;

	FHitResult BoxHit;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AItem>>InventoryArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryUI* InventoryHUD;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UUserWidget* PickUpUI;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AItem*>CanPickItem;
	
public:	
	void ShowPickUpUI();
	void HiddenPickUpUI();
	void PickUpActorByKey();
	UFUNCTION(BlueprintCallable)
	void ShowInventoryHUD();
	//初始化控制器
	void InitializeController(APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable)
	void HiddenInventoryHUD();
	void ShpereTrace();


	FORCEINLINE void SetInventoryArray (AItem* AddtoItem) { InventoryArray.Add(AddtoItem->GetClass());}
	FORCEINLINE TArray<TSubclassOf<AItem>> GetInventoryArray() { return InventoryArray; }
};
