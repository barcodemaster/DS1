// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DS1PlayerState.h"
#include "GAS/DS1PlayerAbilitySystemComponent.h"
#include "GAS/DS1PlayerAttributeSet.h"

ADS1PlayerState::ADS1PlayerState()
{
	ASC = CreateDefaultSubobject<UDS1PlayerAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UDS1PlayerAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ADS1PlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
