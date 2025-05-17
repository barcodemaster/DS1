// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/DS1Equipment.h"
#include "GameplayTagContainer.h"
#include "DS1Weapon.generated.h"

class UDS1MontageActionData;
class UDS1CombatComponent;
class UGameplayEffect;
class UDS1WeaponCollisionComponent;
class UDS1WeaponAttributeSet;
/**
 * 무기는 자신만의 Montage Data를 들고있는다. 나중에 캐릭터에서 무기별 Montage를 재생시키는 방식으로 사용된다.
 */
UCLASS()
class DS1_API ADS1Weapon : public ADS1Equipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName EquipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName UnequipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Animation")
	UDS1MontageActionData* MontageActionData;

protected:
	UPROPERTY(VisibleAnywhere)
	UDS1WeaponCollisionComponent* WeaponCollision;

protected:
	UPROPERTY()
	UDS1CombatComponent* CombatComponent;

	UPROPERTY()
	UDS1WeaponAttributeSet* AttributeSet;


protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InitialStatEffect;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> StaminaCostEffectMap;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> StaminaCostMap;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> DamageEffectMap;

public:
	ADS1Weapon();

protected:
	virtual void BeginPlay() override;


public:
	/** 이 함수는 떨궈진 무기를 장착하는 역할을한다. 이 때 손에장착할지 등에장착할지 고른다. */
	virtual void EquipItem() override;

	UAnimMontage* GetMontageForTag(const FGameplayTag& Tag, const int32 Index = 0) const;
	
	TSubclassOf<UGameplayEffect> GetStaminaCostEffect(const FGameplayTag& Tag) const;
	float GetStaminaCost(const FGameplayTag& Tag) const;

	TSubclassOf<UGameplayEffect> GetDamageEffectClass() const;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; };
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; };
	FORCEINLINE UDS1WeaponCollisionComponent* GetCollision() const { return WeaponCollision; };

public:
	/** 무기의 Collision에 검출된 Actor에 Damage를 전달 */
	void OnHitActor(const FHitResult& Hit);

	virtual void ApplyEffectToTarget(TSubclassOf<UGameplayEffect>& EffectClass, AActor* TargetActor);
	virtual void ApplyEffectToSelf(TSubclassOf<UGameplayEffect>& EffectClass);
};

