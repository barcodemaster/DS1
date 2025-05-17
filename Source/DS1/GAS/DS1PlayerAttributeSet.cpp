// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DS1PlayerAttributeSet.h"

#include "DS1Define.h"
#include "GameplayEffectExtension.h"

UDS1PlayerAttributeSet::UDS1PlayerAttributeSet()
	: Stemina(100.0f)
	, MaxStemina(100.0f)
{
}

void UDS1PlayerAttributeSet::BroadcastAttributeChanged(EDS1AttributeType AttributeType)
{
	if (OnAttributeChanged.IsBound())
	{
		switch (AttributeType)
		{
			case EDS1AttributeType::Stamina:
				OnAttributeChanged.Broadcast(EDS1AttributeType::Stamina, GetStemina(),GetMaxStemina());
				break;
			case EDS1AttributeType::Health:
				OnAttributeChanged.Broadcast(EDS1AttributeType::Health, GetHealth(),GetMaxHealth());
				break;
		}
	}
}

void UDS1PlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetSteminaAttribute())
	{
		if (NewValue < 0.0f)
		{
			NewValue = 0.0f;
			if (OnZeroStemina.IsBound())
				OnZeroStemina.Broadcast();
		}
		else if (NewValue >= GetMaxStemina())
			NewValue = GetMaxStemina();
		
	}
	
}

bool UDS1PlayerAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	return true;
	
}

void UDS1PlayerAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
		
	if (Data.EvaluatedData.Attribute == GetSteminaAttribute())
	{
		BroadcastAttributeChanged(EDS1AttributeType::Stamina);
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float PervHealth = GetHealth();
		float ActualDamage = GetDamage();
		float CurrentHealth = FMath::Clamp(GetHealth() - GetDamage(), 0.0f, GetMaxHealth());
		SetHealth(CurrentHealth);
		SetDamage(0.0f);
		BroadcastAttributeChanged(EDS1AttributeType::Health);
		if (CurrentHealth == 0.0f)
			OnDeath.Broadcast();
		GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Damaged : %f"), ActualDamage));
	}
}
