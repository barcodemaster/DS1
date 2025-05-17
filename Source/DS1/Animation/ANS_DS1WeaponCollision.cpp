// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS_DS1WeaponCollision.h"
#include "ANS_DS1WeaponCollision.h"
#include "Components/DS1CombatComponent.h"
#include "Equipments/DS1Weapon.h"
#include "Components/DS1WeaponCollisionComponent.h"

UANS_DS1WeaponCollision::UANS_DS1WeaponCollision(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UANS_DS1WeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (const UDS1CombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UDS1CombatComponent>())
		{
			const ADS1Weapon* Weapon = CombatComponent->GetMainWeapon();
			if (::IsValid(Weapon))
			{
				Weapon->GetCollision()->TurnOnCollision();
			}
		}
	}
}

void UANS_DS1WeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (const UDS1CombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UDS1CombatComponent>())
		{
			const ADS1Weapon* Weapon = CombatComponent->GetMainWeapon();
			if (::IsValid(Weapon))
			{
				Weapon->GetCollision()->TurnOffCollision();
			}
		}
	}
}
