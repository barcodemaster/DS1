// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "DS1PlayerState.generated.h"

class UDS1PlayerAbilitySystemComponent;
class UDS1PlayerAttributeSet;

/**
 * 
 */
UCLASS()
class DS1_API ADS1PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ADS1PlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	FORCEINLINE UDS1PlayerAttributeSet* GetAttributeSet() const { return AttributeSet; }

private:
	UPROPERTY(EditAnywhere , Category = GAS, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDS1PlayerAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UDS1PlayerAttributeSet> AttributeSet;
};
