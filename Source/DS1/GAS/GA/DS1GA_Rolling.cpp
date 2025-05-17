// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DS1GA_Rolling.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/DS1Character.h"
#include "Components/DS1AttributeComponent.h"

UDS1GA_Rolling::UDS1GA_Rolling()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDS1GA_Rolling::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle,ActorInfo,ActivationInfo))
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);

	ADS1Character* Character = Cast<ADS1Character>(ActorInfo->AvatarActor.Get());
	if (Character)
		Character->GetAttributeComponent()->ToggleStaminaRegeneration(false);
	
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Rolling"), RollingMontage);
	Task->OnCompleted.AddDynamic(this, &UDS1GA_Rolling::MontageEndCallback);
	Task->ReadyForActivation();

	
}

void UDS1GA_Rolling::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ADS1Character* Character = Cast<ADS1Character>(ActorInfo->AvatarActor.Get());
	if (Character)
		Character->GetAttributeComponent()->ToggleStaminaRegeneration(true);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDS1GA_Rolling::MontageEndCallback()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
}
