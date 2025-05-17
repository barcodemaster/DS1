// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DS1GA_Sprint.generated.h"

/**
 * 스페이스바를 누르고있으면 800의 속력이 된다. Effect는 Infinite로 0.2초마다 1씩깍이게 했다.
 * 시작할때 Player한테서 AttributeComponent를 가져와서 Stemina를 회복못하게 하고 끝날때 회복하게만든다.
 * 스페이스바를 떼면 GA가 끝나야해서 InputReleased를 구현하였다.
 */
UCLASS()
class DS1_API UDS1GA_Sprint : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UDS1GA_Sprint();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	virtual void ZeroSteminaCallback();
};
