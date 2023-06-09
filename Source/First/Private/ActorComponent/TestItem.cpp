// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/TestItem.h"

ATestItem::ATestItem()
{

}

void ATestItem::BeginPlay()
{
	Super::BeginPlay();
}

void ATestItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = GetActorLocation();
	FVector GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
	FVector Direction = (GlobalTargetLocation - Location).GetSafeNormal();
	Location += Speed * DeltaTime * Direction;
	SetActorLocation(Location);
}
