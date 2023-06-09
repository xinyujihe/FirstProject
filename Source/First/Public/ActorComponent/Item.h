// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UWidgetComponent;
class UBoxComponent;
class UUserWidget;
class USoundBase;
class UInventoryComponent;
struct FItemDetails
{
	FText ItemName = FText::FromString("None");
	FText ItemDiscription = FText::FromString("None");;
	UTexture2D* ItemIcon = nullptr;
	UStaticMesh* Mesh = nullptr;
};

UCLASS()

class FIRST_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AItem();
	virtual void Tick(float DeltaTime) override;
protected:
	
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool DoesInterface(AActor* OtherActor);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool ComponentHasInterface(AActor* OtherActor);
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	FText ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	FText ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	UTexture2D* ItemIcon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* WidgetComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* Box;


private:
	UPROPERTY(EditAnywhere, Category = Variable)
	USoundBase* PickUpSpound;

	UPROPERTY(VisibleAnywhere, Category = Variable)
	UInventoryComponent* OverLappingComponent;
public:

	FItemDetails GetItemDetails();
	FORCEINLINE USoundBase* GetSound() { return PickUpSpound; };
};
