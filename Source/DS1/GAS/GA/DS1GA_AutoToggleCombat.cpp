// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DS1GA_AutoToggleCombat.h"
#include "Characters/DS1Character.h"
#include "Components/DS1CombatComponent.h"
#include "Equipments/DS1Weapon.h"
#include "DS1Define.h"
#include "DS1GameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UDS1GA_AutoToggleCombat::UDS1GA_AutoToggleCombat()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDS1GA_AutoToggleCombat::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ADS1Character* Character = Cast<ADS1Character>(ActorInfo->AvatarActor.Get());
	if (Character)
	{		
		UDS1CombatComponent* CombatComponent = Character->GetComponentByClass<UDS1CombatComponent>();
		if (CombatComponent && !CombatComponent->IsCombatEnabled())
		{
			CombatComponent->SetCombatEnabled(true);
			ADS1Weapon* MainWeapon = CombatComponent->GetMainWeapon();
			if (MainWeapon)
			{
				UAnimMontage* Montage = MainWeapon->GetMontageForTag(DS1GameplayTags::Character_Action_Equip);
				if (Montage)
				{
					UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,FName("AutoToggleCombat"),Montage);
					Task->OnCompleted.AddDynamic(this, &UDS1GA_AutoToggleCombat::MontageEndCallback);
					Task->ReadyForActivation();
				}
				else
				{
					EndAbility(Handle,ActorInfo,ActivationInfo,true, false);
				}
			}
			else
			{
				EndAbility(Handle,ActorInfo,ActivationInfo,true, false);
			}
		}
		else
			EndAbility(Handle,ActorInfo,ActivationInfo,true, false);
	}
	else
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,true, false);
	}
}

void UDS1GA_AutoToggleCombat::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDS1GA_AutoToggleCombat::MontageEndCallback()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
}
