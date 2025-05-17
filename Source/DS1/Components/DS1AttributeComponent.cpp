// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DS1AttributeComponent.h"
#include "Characters/DS1Character.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GAS/DS1PlayerAttributeSet.h"

UDS1AttributeComponent::UDS1AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UDS1AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UDS1AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UDS1AttributeComponent::Initailize()
{
	ADS1Character* OwningCharacter = Cast<ADS1Character>(GetOwner());
	OwningActorASC = OwningCharacter->GetAbilitySystemComponent();
	OwningActorAttributeSet = OwningActorASC->GetSet<UDS1PlayerAttributeSet>();
}


void UDS1AttributeComponent::ToggleStaminaRegeneration(bool bEnabled, float StartDelay)
{
	if (bEnabled)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &ThisClass::RegenerateStaminaHandler, 0.1f, true, StartDelay);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}



void UDS1AttributeComponent::RegenerateStaminaHandler()
{
	/** 여기는 GAS 이용한 코드 */
	if (OwningActorAttributeSet->GetStemina() >= OwningActorAttributeSet->GetMaxStemina())
	{
		ToggleStaminaRegeneration(false);
		return;
	}
	
	if (OwningActorASC )
	{
		FGameplayEffectContextHandle EffectContext = OwningActorASC->MakeEffectContext();
		EffectContext.AddSourceObject(this); // 필요시

		FGameplayEffectSpecHandle SpecHandle = OwningActorASC->MakeOutgoingSpec(
			SteminaRegenEffectClass,         // UGameplayEffect* 또는 TSubclassOf<UGameplayEffect>
			1.0f,                            // Level
			EffectContext
		);

		if (SpecHandle.IsValid())
		{
			OwningActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}	
}


