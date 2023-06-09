// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickUpInterface.h"
#include "CharacterTypes.h"
#include "BaseCharacter.generated.h"

class UAnimMontage;
class UInventoryComponent;
class AItem;

UCLASS()
class FIRST_API ABaseCharacter : public ACharacter, public IPickUpInterface
{
	GENERATED_BODY()
	
public:
	
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;

	//virtual  void AddItemToInventory_Implementation(class AItem* Item) override;

	void PlayIdelToCombatMontage(const FName& SectionName);
	void PlayFistsMontag();

	UFUNCTION(BlueprintCallable)
	void SetAttackSelection(int32 Value);
	void ClearAttackSelection();
	void StartComboTimer();//开始连击计时
	void ClearComboTimer();//清除连击计时

	/*组件*/

	//物品栏组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;

private:	
	/*蒙太奇*/
	UPROPERTY(EditAnyWhere, Category = Combat)
	UAnimMontage* IdeleToCombatMontage;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* FistsMontage;

	/*函数*/
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);


	/*自定义变量*/
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> FistsMontageSections;

	UPROPERTY(VisibleAnywhere, Category = Combat)
	int32 AttackSelection;

	FTimerHandle ComboTimer;

	
public:

	
};
