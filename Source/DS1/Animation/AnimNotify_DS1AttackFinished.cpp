// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_DS1AttackFinished.h"
#include "Characters/DS1Character.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "DS1GameplayTags.h"


UAnimNotify_DS1AttackFinished::UAnimNotify_DS1AttackFinished()
{
}

void UAnimNotify_DS1AttackFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ADS1Character* Owner = Cast<ADS1Character>(MeshComp->GetOwner()))
	{
		FGameplayEventData EventData;
		EventData.EventTag = FGameplayTag::RequestGameplayTag("Notify.ComboWindow.Finished");
		EventData.EventMagnitude = ComboResetDelay;
		
		UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();
		if (ASC)
			ASC->HandleGameplayEvent(EventData.EventTag,&EventData);
	}
}
