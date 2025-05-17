// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DS1Character.generated.h"

class UDS1PlayerAttributeSet;
class UDS1CombatComponent;
class UDS1StateComponent;
class UDS1PlayerHUDWidget;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UDS1AttributeComponent;
class UAbilitySystemComponent;
class UDS1PlayerAbilitySystemComponent;
class UGameplayAbility;

UCLASS()
class DS1_API ADS1Character : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

// GAS
private:
	UPROPERTY()
	UDS1PlayerAbilitySystemComponent* ASC;

	UPROPERTY()
	UDS1PlayerAttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<int32,TSubclassOf<UGameplayAbility>> StartInputAbilities;
	
// Input Section
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintRollingAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	/** Heavy Attack */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* HeavyAttackAction;

private:
	/** 캐릭터의 각종 스탯 관리 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UDS1AttributeComponent* AttributeComponent;

	/** 캐릭터의 상태 관리 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UDS1StateComponent* StateComponent;

	/** 무기, 전투 관리 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UDS1CombatComponent* CombatComponent;

	/** 전투 활성화/비활성화 토글 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleCombatAction;

// UI Section
protected:
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY()
	UDS1PlayerHUDWidget* PlayerHUDWidget;

protected:
	/** 질주 속도 */
	UPROPERTY(EditAnywhere, Category="Sprinting")
	float SprintingSpeed = 750.f;

	/** 일반 속도 */
	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float NormalSpeed = 500.f;

	UPROPERTY(VisibleAnywhere, Category = "Sprinting")
	bool bSprinting = false;

protected:
	UPROPERTY(EditAnywhere, Category="Montage")
	UAnimMontage* RollingMontage;

public:
	ADS1Character();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UDS1PlayerAttributeSet* GetAttributeSet() const;

public:
	FORCEINLINE UDS1StateComponent* GetStateComponent() const { return StateComponent; };
	FORCEINLINE UDS1AttributeComponent* GetAttributeComponent() const { return AttributeComponent; };

	/** Sprinting 관련*/
	FORCEINLINE bool IsSprinting() const { return bSprinting; };
	FORCEINLINE void SetSprinting(const bool _bSprinting) { bSprinting = _bSprinting; };
	
	

protected:
	/** GAS시스템에서 Input으로 실행 */
	void InputPressed(int32 Id);
	void InputReleased(int32 Id);
	/** 캐릭터가 이동중인지 체크 */
	bool IsMoving() const;

	/** 이동 */
	void Move(const FInputActionValue& Values);
	/** 카메라 방향 */
	void Look(const FInputActionValue& Values);
	/** 인터렉션 */
	void Interact();
	
};
