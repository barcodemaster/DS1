// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DS1GA_ToggleCombat.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/DS1Character.h"
#include "Components/DS1CombatComponent.h"
#include "Equipments/DS1Weapon.h"
#include "DS1GameplayTags.h"


void UDS1GA_ToggleCombat::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ADS1Character* Character = Cast<ADS1Character>(ActorInfo->AvatarActor.Get());
	if (Character)
	{
		UDS1CombatComponent* CombatComponent = Character->GetComponentByClass<UDS1CombatComponent>();
		if (CombatComponent)
		{
			ADS1Weapon* MainWeapon = CombatComponent->GetMainWeapon();
			if (MainWeapon)
			{
				UAnimMontage* ToggleCombatMontage;
				if (CombatComponent->IsCombatEnabled())
					ToggleCombatMontage = MainWeapon->GetMontageForTag(DS1GameplayTags::Character_Action_Unequip);
				else
					ToggleCombatMontage = MainWeapon->GetMontageForTag(DS1GameplayTags::Character_Action_Equip);
				
				UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ToggleCombat"), ToggleCombatMontage);
				Task->OnCompleted.AddDynamic(this, &UDS1GA_ToggleCombat::MontageEndCallback);
				Task->ReadyForActivation();
			}
		}		
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UDS1GA_ToggleCombat::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDS1GA_ToggleCombat::MontageEndCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
