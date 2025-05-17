// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DS1GA_Attack.h"
#include "Characters/DS1Character.h"
#include "AbilitySystemComponent.h"
#include "GAS/DS1PlayerAttributeSet.h"
#include "Components/DS1CombatComponent.h"
#include "Components/DS1StateComponent.h"
#include "Components/DS1AttributeComponent.h"
#include "Equipments/DS1Weapon.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "DS1Define.h"
#include "DS1GameplayTags.h"

void UDS1GA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	

	Character = Cast<ADS1Character>(ActorInfo->AvatarActor.Get());
	if (Character)
	{
		UDS1CombatComponent* CombatComponent = Character->GetComponentByClass<UDS1CombatComponent>();
		if (CombatComponent && CombatComponent->GetMainWeapon())
		{
			UDS1StateComponent* StateComponent = Character->GetComponentByClass<UDS1StateComponent>();
			if (StateComponent)
			{
				StateComponent->ToggleMovementInput(false);
			}

			if (CurrentAttackTag == DS1GameplayTags::Character_Attack_LightOrRunning)
			{
				if (Character->IsSprinting())
				{
					CurrentAttackTag = DS1GameplayTags::Character_Attack_Running;
				}
				else
				{
					CurrentAttackTag = DS1GameplayTags::Character_Attack_Light;
				}
			}

			DoAttack();
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
	
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UDS1GA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if (bAttacking == false)
	{
		ComboCount++;
		DoAttack();
		ActorInfo->AvatarActor.Get()->GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	}
	else if (bCanComboInput == true)
	{
		bSavedComboInput = true;
	}
	
}

void UDS1GA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDS1GA_Attack::DoAttack()
{
	UAnimMontage* Montage = nullptr;
	
	if (Character)
	{

		if (!CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
		{
			UDS1StateComponent* StateComponent = Character->GetComponentByClass<UDS1StateComponent>();
			StateComponent->ToggleMovementInput(true);
			ResetCombo();
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
			return;
		}
		
		UDS1AttributeComponent* AttributeComponent = Character->GetComponentByClass<UDS1AttributeComponent>();
		AttributeComponent->ToggleStaminaRegeneration(false);
		AttributeComponent->ToggleStaminaRegeneration(true);

		UDS1CombatComponent* CombatComponent = Character->GetComponentByClass<UDS1CombatComponent>();
		if (CombatComponent)
		{
			CombatComponent->SetLastAttackType(CurrentAttackTag);
			ADS1Weapon* MainWeapon = CombatComponent->GetMainWeapon();
			if (MainWeapon)
			{
				Montage = MainWeapon->GetMontageForTag(CurrentAttackTag, ComboCount);
				if (!Montage)
				{
					ComboCount = 0;
					Montage = MainWeapon->GetMontageForTag(CurrentAttackTag, ComboCount);
				}

				UAbilityTask_WaitGameplayEvent* BeginEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Notify.ComboWindow.Begin")));
				BeginEventTask->EventReceived.AddDynamic(this, &UDS1GA_Attack::EnableComboWindow);
				BeginEventTask->ReadyForActivation();

				UAbilityTask_WaitGameplayEvent* EndEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Notify.ComboWindow.End")));
				EndEventTask->EventReceived.AddDynamic(this, &UDS1GA_Attack::DisableComboWindow);
				EndEventTask->ReadyForActivation();


				UAbilityTask_WaitGameplayEvent* FinishEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Notify.ComboWindow.Finished")));
				FinishEventTask->EventReceived.AddDynamic(this, &UDS1GA_Attack::AttackFinished);
				FinishEventTask->ReadyForActivation();

				UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Attack"), Montage);
				MontageTask->OnCompleted.AddDynamic(this, &UDS1GA_Attack::MontageEndCallback);
				MontageTask->ReadyForActivation();
				bAttacking = true;

			}
		}

		
	}
}


void UDS1GA_Attack::ResetCombo()
{
	ComboCount = 0;
	bCanComboInput = false;
	bSavedComboInput = false;
	bAttacking = false;
	Character->GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
}


void UDS1GA_Attack::EnableComboWindow(FGameplayEventData Payload)
{
	bCanComboInput = true;
	if (Character && Character->GetWorld())
	{
		Character->GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	}
	UE_LOG(LogTemp, Warning, TEXT("Combo Window Opened: Combo Counter = %d"), ComboCount);
}

void UDS1GA_Attack::DisableComboWindow(FGameplayEventData Payload)
{
	bCanComboInput = false;
	
	if (bSavedComboInput)
	{
		bSavedComboInput = false;
		ComboCount++;
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Closed: Advancing to next combo = %d"), ComboCount);
		DoAttack();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Closed: No input received"));
	}
}

void UDS1GA_Attack::AttackFinished(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("AttackFinished"));
	UDS1StateComponent* StateComponent = Character->GetComponentByClass<UDS1StateComponent>();
	if (StateComponent)
	{
		StateComponent->ToggleMovementInput(true);
	}
	float ComboResetDelay = Payload.EventMagnitude;
	// ComboResetDelay 후에 콤보 시퀀스 종료
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ThisClass::ResetCombo, ComboResetDelay, false);
}

void UDS1GA_Attack::MontageEndCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


