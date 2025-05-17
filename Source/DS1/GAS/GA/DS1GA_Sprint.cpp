// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DS1GA_Sprint.h"
#include "Characters/DS1Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "GAS/DS1PlayerAttributeSet.h"
#include "Components/DS1AttributeComponent.h"

UDS1GA_Sprint::UDS1GA_Sprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDS1GA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);

	ADS1Character* Character = Cast<ADS1Character>(ActorInfo->AvatarActor.Get());	
	if (Character)
	{
		Character->SetSprinting(true);
		Character->GetAttributeComponent()->ToggleStaminaRegeneration(false);
		Character->GetCharacterMovement()->MaxWalkSpeed = 750.f;
		
		UDS1PlayerAttributeSet* AttributeSet = Character->GetAttributeSet();
		AttributeSet->OnZeroStemina.AddDynamic(this,&UDS1GA_Sprint::ZeroSteminaCallback);
	}
}

void UDS1GA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UDS1GA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ADS1Character* Character = Cast<ADS1Character>(ActorInfo->AvatarActor.Get());
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 500.f;
		Character->GetAttributeComponent()->ToggleStaminaRegeneration(true);
		Character->SetSprinting(false);
	}
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	
	// Cost로 사용한 GE 클래스
	TSubclassOf<UGameplayEffect> CostGEClass = CostGameplayEffectClass;

	if (CostGEClass)
	{
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		// ASC의 모든 ActiveGE 중에서 찾아서 제거
		const FActiveGameplayEffectsContainer& ActiveEffects = ASC->GetActiveGameplayEffects();
		for (auto It = ActiveEffects.CreateConstIterator(); It; ++It)
		{
			const FActiveGameplayEffect& ActiveGE = *It;
			if (ActiveGE.Spec.Def && ActiveGE.Spec.Def->GetClass() == CostGEClass)
			{
				HandlesToRemove.Add(ActiveGE.Handle);
			}
		}

		for (FActiveGameplayEffectHandle _Handle : HandlesToRemove)
		{
			ASC->RemoveActiveGameplayEffect(_Handle);
		}
	}
	
	UDS1PlayerAttributeSet* AttributeSet = Character->GetAttributeSet();
	AttributeSet->OnZeroStemina.RemoveDynamic(this,&UDS1GA_Sprint::ZeroSteminaCallback);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDS1GA_Sprint::ZeroSteminaCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
