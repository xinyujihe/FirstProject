// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "ActorComponent/InventoryComponent.h"
#include "ActorComponent/Item.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

//void ABaseCharacter::AddItemToInventory_Implementation(AItem* Item)
//{
//	GEngine->AddOnScreenDebugMessage(4, 30.0f, FColor::Cyan, TEXT(" in C++ ::ReactToTriggert_Implementation"));
//	InventoryComponent->SetInventoryArray(Item);
//}



void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::PlayIdelToCombatMontage(const FName& SectionName)
{
	 PlayMontageSection(IdeleToCombatMontage, SectionName);
}

void ABaseCharacter::PlayFistsMontag()
{
	PlayRandomMontageSection(FistsMontage, FistsMontageSections);
}

void ABaseCharacter::SetAttackSelection(int32 Value = 0)
{
	AttackSelection = Value;
}

void ABaseCharacter::ClearAttackSelection()
{
	AttackSelection = 0;
}

void ABaseCharacter::StartComboTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ComboTimer, this, &ABaseCharacter::ClearAttackSelection, 1, false, 1.25f);
}

void ABaseCharacter::ClearComboTimer()
{
	GetWorldTimerManager().PauseTimer(ComboTimer);
	GetWorldTimerManager().ClearTimer(ComboTimer);
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	//const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	const int32 Selection = FMath::Clamp(AttackSelection, 0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}



void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FString TStr = FString::FromInt(AttackSelection);
	GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, TStr);
}


void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

