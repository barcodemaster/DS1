// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DS1Define.h"
#include "DS1PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZeroStemina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FDelegateOnAttributeChanged, EDS1AttributeType, float, float);

/**
 * 
 */
UCLASS()
class DS1_API UDS1PlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	public:
	UDS1PlayerAttributeSet();

	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, Stemina);
	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, MaxStemina);
	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, MaxDamage);
	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, AttackPower);
	ATTRIBUTE_ACCESSORS(UDS1PlayerAttributeSet, MaxAttackPower);

public:
	void BroadcastAttributeChanged(EDS1AttributeType AttributeType);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stemina;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStemina;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPower;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackPower;

public:
	FOnZeroStemina OnZeroStemina;
	FDelegateOnAttributeChanged OnAttributeChanged;
	FOnDeath OnDeath;
};
