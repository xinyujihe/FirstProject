// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FirstCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ActorComponent/InventoryComponent.h"


// Sets default values
AFirstCharacter::AFirstCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
//如果打开Yaw会导致摄像机无法转到人物的前面，只能固定在人物后背，无法看到面部。
//如果打开Pitch会导致人物随着鼠标的移动前后摇晃。

	GetCharacterMovement()->bOrientRotationToMovement = true;//将旋转定向到运动。朝哪个方向前进，人物面朝哪边
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);//旋转速率。

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Arm->SetupAttachment(GetRootComponent());//附加到根组件
	Arm->TargetArmLength = 300.f;//弹簧臂长度
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	Camera->SetupAttachment(Arm);//附加到弹簧臂
	
}

// Called when the game starts or when spawned
void AFirstCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		InventoryComponent->InitializeController(PlayerController);
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}
}

void AFirstCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}


void AFirstCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//创建一个全新的旋转矩阵,X是世界坐标当中前进的方向
	//AddMovementInput(ForwardDirection, MovementVector.Y);
	//左右
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//AddMovementInput(RightDirection, MovementVector.X);
	MoveInputValue = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;
}

void AFirstCharacter::ClearMove(const FInputActionValue& Value)
{
	MoveInputValue = FVector(0);
}

void AFirstCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(LookAxisValue.Y);
}

void AFirstCharacter::SpeedUp(const FInputActionValue& Value)
{
	bSpeedUp ? bSpeedUp = false : bSpeedUp = true;
}

void AFirstCharacter::LightAttack(const FInputActionValue& Value)
{
	if (CharacterState < ECharacterState::ECS_Unequipped)
	{
		PlayIdelToCombatMontage(FName("IdleToFists"));
		CharacterState = ECharacterState::ECS_Unequipped;
		return;
	}
	if (FistsAndUnoccupied())
	{
		ClearComboTimer();
		PlayFistsMontag();
		ActionState = EActionState::EAS_Attacking;
		StartComboTimer();
	}
}

bool AFirstCharacter::FistsAndUnoccupied()
{
	return CharacterState == ECharacterState::ECS_Unequipped && ActionState == EActionState::EAS_Unoccupied;
}

void AFirstCharacter::BackToIdle(const FInputActionValue& Value)
{
	if (FistsAndUnoccupied())
	{
		PlayIdelToCombatMontage(FName("FistsToIdle"));
		CharacterState = ECharacterState::ECS_StandMove;
	}
}

void AFirstCharacter::PickUpActor(const FInputActionValue& Value)
{
	InventoryComponent->PickUpActorByKey();
}

// Called every frame
void AFirstCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AFirstCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AFirstCharacter::ClearMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirstCharacter::Look);
		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Started, this, &AFirstCharacter::SpeedUp);
		EnhancedInputComponent->BindAction(LeftKeyAttackAction, ETriggerEvent::Triggered, this, &AFirstCharacter::LightAttack);
		EnhancedInputComponent->BindAction(BackToIdleAction, ETriggerEvent::Triggered, this, &AFirstCharacter::BackToIdle);
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Triggered, this, &AFirstCharacter::PickUpActor);
	}
	

}



