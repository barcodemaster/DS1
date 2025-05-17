// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "DS1GA_Attack.generated.h"

class ADS1Character;

/**
 * 
 */
UCLASS()
class DS1_API UDS1GA_Attack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void DoAttack();
	void ResetCombo();

	UFUNCTION()
	void EnableComboWindow(FGameplayEventData Payload);
	UFUNCTION()
	void DisableComboWindow(FGameplayEventData Payload);
	UFUNCTION()
	void AttackFinished(FGameplayEventData Payload);
	UFUNCTION()
	void MontageEndCallback();

	bool CheckEnoughtSteminaCost();

public:
	UPROPERTY(EditAnywhere, Category = AttackTag)
	FGameplayTag CurrentAttackTag;
	
	bool bLightAttack = false;
	bool bCanComboInput = false;
	bool bSavedComboInput = false;
	bool bAttacking = false;
	int32 ComboCount = 0;
	FTimerHandle ComboResetTimerHandle;	
	UPROPERTY()
	ADS1Character* Character;
};
