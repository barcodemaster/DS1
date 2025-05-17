// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DS1PlayerHUDWidget.h"

#include "DS1StatBarWidget.h"
#include "Characters/DS1Character.h"
#include "GAS/DS1PlayerAttributeSet.h"


UDS1PlayerHUDWidget::UDS1PlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UDS1PlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ADS1Character* OwningPawn = Cast<ADS1Character>(GetOwningPlayerPawn()))
	{
		if (UDS1PlayerAttributeSet* AttributeSet = OwningPawn->GetAttributeSet())
		{
			AttributeSet->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChanged);
			AttributeSet->BroadcastAttributeChanged(EDS1AttributeType::Stamina);
		}
	}
}

void UDS1PlayerHUDWidget::OnAttributeChanged(EDS1AttributeType AttributeType, float InValue, float MaxValue)
{
	
	switch (AttributeType)
	{
	case EDS1AttributeType::Stamina:
		{
			StaminaBarWidget->SetRatio(InValue/MaxValue);
			break;
		}
	case EDS1AttributeType::Health:
		break;
	}
}
