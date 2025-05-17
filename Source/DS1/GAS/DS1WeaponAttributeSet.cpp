// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DS1WeaponAttributeSet.h"

#include "GameplayEffectExtension.h"

UDS1WeaponAttributeSet::UDS1WeaponAttributeSet()
{
}

void UDS1WeaponAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}

bool UDS1WeaponAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return true;
}

void UDS1WeaponAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute())
	{
		float Value = GetAttackPower();
		float Value2 = GetAttackRadius();
		float Value3 = GetMaxAttackPower();
	}
}
