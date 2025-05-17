// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DS1Character.h"

#include "DS1GameplayTags.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/DS1CombatComponent.h"
#include "Components/DS1AttributeComponent.h"
#include "Components/DS1StateComponent.h"
#include "Equipments/DS1Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/DS1Interact.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/DS1PlayerHUDWidget.h"
#include "GAS/DS1PlayerAbilitySystemComponent.h"
#include "GameModes/DS1PlayerState.h"

ADS1Character::ADS1Character()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	/** 이동, 감속 속도 */
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	CameraBoom->bUsePawnControlRotation = true;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	AttributeComponent = CreateDefaultSubobject<UDS1AttributeComponent>(TEXT("Attribute"));
	StateComponent = CreateDefaultSubobject<UDS1StateComponent>(TEXT("State"));
	CombatComponent = CreateDefaultSubobject<UDS1CombatComponent>(TEXT("Combat"));
	ASC = nullptr;
	AttributeSet = nullptr;

}

void ADS1Character::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UDS1PlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	
}

void ADS1Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ADS1PlayerState* PS = Cast<ADS1PlayerState>(GetPlayerState());
	if (PS)
	{
		AttributeSet = PS->GetAttributeSet();
		ASC = Cast<UDS1PlayerAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		ASC->InitAbilityActorInfo(PS, this);

		for (auto GA : StartAbilities)
		{
			FGameplayAbilitySpec Spec(GA);
			ASC->GiveAbility(Spec);
		}

		for (auto GA : StartInputAbilities)
		{
			FGameplayAbilitySpec Spec(GA.Value);
			Spec.InputID = GA.Key;
			ASC->GiveAbility(Spec);
		}

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
	
	AttributeComponent->Initailize();
	
}

void ADS1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Stamina : %f"), AttributeComponent->GetBaseStamina()));
	//GEngine->AddOnScreenDebugMessage(2, 1.5f, FColor::Cyan, FString::Printf(TEXT("MaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed));

}

void ADS1Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ADS1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		// 인터렉션
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);	

		// 질주
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Triggered, this, &ThisClass::InputPressed,100);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Completed, this, &ThisClass::InputReleased,100);
		// 구르기
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Canceled, this, &ThisClass::InputPressed,101);
		// 전투 활성/비활성
		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ThisClass::InputPressed,102);
		// 자동 전투 활성화
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::InputPressed,103);
		// 기본 공격
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ThisClass::InputPressed,200);
		// 스페셜 공격
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::InputPressed,201);
		// Heavy Attack
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ThisClass::InputPressed,202);
			
	}

}

UAbilitySystemComponent* ADS1Character::GetAbilitySystemComponent() const
{
	return ASC;
}

UDS1PlayerAttributeSet* ADS1Character::GetAttributeSet() const
{
	return AttributeSet;
}

void ADS1Character::InputPressed(int32 Id)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(Id);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void ADS1Character::InputReleased(int32 Id)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(Id);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

bool ADS1Character::IsMoving() const
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		return MovementComp->Velocity.Size2D() > 3.f && MovementComp->GetCurrentAcceleration() != FVector::Zero();
	}

	return false;
}


void ADS1Character::Move(const FInputActionValue& Values)
{
	check(StateComponent);

	// 이동 입력 가능 상태인지 체크.
	if (StateComponent->MovementInputEnabled() == false)
	{
		return;
	}

	FVector2D MovementVector = Values.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotator(0, Rotation.Yaw, 0);

		const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

		// 주어진 월드 방향 벡터(보통 정규화됨)를 따라 'ScaleValue'만큼 스케일된 이동 입력을 추가합니다. 
		// ScaleValue가 0보다 작으면, 이동은 반대 방향으로 이루어집니다.
		// ScaleValue는 아날로그 입력에 사용될 수 있습니다. 
		// 예를 들어, 0.5 값은 정상 값의 절반을 적용하고, -1.0은 방향을 반대로 합니다.
		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
		
	}
}

void ADS1Character::Look(const FInputActionValue& Values)
{
	FVector2D LookDirection = Values.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(LookDirection.Y);
	}
}


void ADS1Character::Interact()
{
	FHitResult OutHit;
	const FVector Start = GetActorLocation();
	const FVector End = Start;
	constexpr float Radius = 100.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACTION));

	TArray<AActor*> ActorsToIgnore;

	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		Start,
		End,
		Radius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true);

	if (bHit)
	{
		// 감지한 Actor와 Interaction
		if (AActor* HitActor = OutHit.GetActor())
		{
			if (IDS1Interact* Interaction = Cast<IDS1Interact>(HitActor))
			{
				Interaction->Interact(this);
			}
		}
	}
}



