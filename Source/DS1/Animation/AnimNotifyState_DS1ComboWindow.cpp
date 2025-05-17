// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_DS1ComboWindow.h"
#include "Characters/DS1Character.h"
#include "AbilitySystemComponent.h"
#include "DS1Define.h"
#include "DS1GameplayTags.h"

#include "Abilities/GameplayAbilityTypes.h"

UAnimNotifyState_DS1ComboWindow::UAnimNotifyState_DS1ComboWindow(const FObjectInitializer& ObjectInitializer)
{
	
}

FString UAnimNotifyState_DS1ComboWindow::GetNotifyName_Implementation() const
{
	return TEXT("ComboWindow");
}

void UAnimNotifyState_DS1ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ADS1Character* Owner = Cast<ADS1Character>(MeshComp->GetOwner()))
	{
		FGameplayEventData EventData;
		EventData.EventTag = FGameplayTag::RequestGameplayTag("Notify.ComboWindow.Begin");

		UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();
		if (ASC)
			ASC->HandleGameplayEvent(EventData.EventTag,&EventData);
	}
}

void UAnimNotifyState_DS1ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ADS1Character* Owner = Cast<ADS1Character>(MeshComp->GetOwner()))
	{
		FGameplayEventData EventData;
		EventData.EventTag = FGameplayTag::RequestGameplayTag("Notify.ComboWindow.End");

		UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();
		if (ASC)
			ASC->HandleGameplayEvent(EventData.EventTag,&EventData);
	}
}
