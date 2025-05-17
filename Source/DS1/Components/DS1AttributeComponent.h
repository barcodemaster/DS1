// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS1Define.h"
#include "Components/ActorComponent.h"
#include "DS1AttributeComponent.generated.h"


class UGameplayEffect;
class UAbilitySystemComponent;
class UDS1PlayerAttributeSet;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS1_API UDS1AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="Stamina")
	float BaseStamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRegenRate = 0.2f;

	/** 스태미나 재충전 타이머 핸들 */
	FTimerHandle StaminaRegenTimerHandle;

	/** 스테미나 재충전 Effect */
	UPROPERTY(EditAnywhere, Category = "Stamina")
	TSubclassOf<UGameplayEffect> SteminaRegenEffectClass;

private:
	UPROPERTY()
	UAbilitySystemComponent* OwningActorASC;

	UPROPERTY()
	const UDS1PlayerAttributeSet* OwningActorAttributeSet;

public:	
	UDS1AttributeComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetBaseStamina() const { return BaseStamina; };
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; };

	/** 스테미나 비율 계산 */
	FORCEINLINE float GetStaminaRatio() const { return BaseStamina / MaxStamina; };

	void Initailize();
	
	/** 스테미너 재충전/중지 토글 */
	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 2.f);

private:
	/** 스태미나 재충전 처리 핸들링 함수 */
	void RegenerateStaminaHandler();
		
};
