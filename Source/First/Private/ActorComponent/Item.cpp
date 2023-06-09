// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/Item.h"
#include "Components/WidgetComponent.h"
#include"Components/BoxComponent.h"
#include "PickUpInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ActorComponent/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponent/InventoryComponent.h"

AItem::AItem()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(GetRootComponent());

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(GetRootComponent());
}


void AItem::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBoxOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AItem::OnBoxEndOverlap);
}

FItemDetails AItem::GetItemDetails()
{
	FItemDetails SelfDetails;

	SelfDetails.ItemName = this->ItemName;
	SelfDetails.ItemDiscription = this->ItemDescription;
	SelfDetails.ItemIcon = this->ItemIcon;
	SelfDetails.Mesh = MeshComponent->GetStaticMesh();

	return SelfDetails;
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		if (OtherActor->GetComponentByClass(UInventoryComponent::StaticClass()))
		{
			OverLappingComponent = Cast<UInventoryComponent>(OtherActor->GetComponentByClass(UInventoryComponent::StaticClass()));
			if (ComponentHasInterface(OtherActor))
			{
				IPickUpInterface::Execute_SetOverlappingItem(OtherActor->GetComponentByClass(UInventoryComponent::StaticClass()), this);
				OverLappingComponent->ShowPickUpUI();
				GEngine->AddOnScreenDebugMessage(63, 0.5f, FColor::Cyan, TEXT("OtherActor->GetComponentByClass(UPickUpInterface::StaticClass())"));
			}
		}
		/*else
		{
			IPickUpInterface::Execute_AddItemToInventory(OtherActor, this);
			GEngine->AddOnScreenDebugMessage(9, 5.0f, FColor::Cyan, TEXT("BluePrint"));
		}*/
	
}

void AItem::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetComponentByClass(UInventoryComponent::StaticClass()))
	{
		if (ComponentHasInterface(OtherActor))
		{
			IPickUpInterface::Execute_RemoveOverlappingItem(OtherActor->GetComponentByClass(UInventoryComponent::StaticClass()),this);
			OverLappingComponent->HiddenPickUpUI();
		}
	}
}

bool AItem::ComponentHasInterface(AActor* OtherActor)
{
	return UKismetSystemLibrary::DoesImplementInterface(OtherActor->GetComponentByClass(UInventoryComponent::StaticClass()), UPickUpInterface::StaticClass());
}
