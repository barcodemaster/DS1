// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DS1GA_Rolling.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class DS1_API UDS1GA_Rolling : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UDS1GA_Rolling();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	
protected:
	UFUNCTION()
	virtual void MontageEndCallback();
	
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* RollingMontage;
};
