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
//�����Yaw�ᵼ��������޷�ת�������ǰ�棬ֻ�̶ܹ�������󱳣��޷������沿��
//�����Pitch�ᵼ���������������ƶ�ǰ��ҡ�Ρ�

	GetCharacterMovement()->bOrientRotationToMovement = true;//����ת�����˶������ĸ�����ǰ���������泯�ı�
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);//��ת���ʡ�

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Arm->SetupAttachment(GetRootComponent());//���ӵ������
	Arm->TargetArmLength = 300.f;//���ɱ۳���
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	Camera->SetupAttachment(Arm);//���ӵ����ɱ�
	
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
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//����һ��ȫ�µ���ת����,X���������굱��ǰ���ķ���
	//AddMovementInput(ForwardDirection, MovementVector.Y);
	//����
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



