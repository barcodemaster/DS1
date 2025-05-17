// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DS1CombatComponent.h"

#include "Characters/DS1Character.h"
#include "Equipments/DS1Weapon.h"
#include "Items/DS1PickupItem.h"

UDS1CombatComponent::UDS1CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UDS1CombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UDS1CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UDS1CombatComponent::SetWeapon(ADS1Weapon* NewWeapon)
{
    // 이미 무기를 가지고 있으면 PickupItem으로 만들어서 떨군다.
    if (::IsValid(MainWeapon))
    {
        if (ADS1Character* OwnerCharacter = Cast<ADS1Character>(GetOwner()))
        {
            ADS1PickupItem* PickupItem = GetWorld()->SpawnActorDeferred<ADS1PickupItem>(ADS1PickupItem::StaticClass(), OwnerCharacter->GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
            PickupItem->SetEquipmentClass(MainWeapon->GetClass());
            PickupItem->FinishSpawning(GetOwner()->GetActorTransform());

            MainWeapon->Destroy();
        }
    }

	MainWeapon = NewWeapon;
}

