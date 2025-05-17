// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DS1Enemy.generated.h"

class UDS1AttributeComponent;
class UDS1StateComponent;
class UAbilitySystemComponent;
class UDS1PlayerAttributeSet;
class UGameplayEffect;
class USoundCue;

UCLASS()
class DS1_API ADS1Enemy : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	UDS1AttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UDS1StateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere)
	UAbilitySystemComponent* ASC;

	UPROPERTY(VisibleAnywhere)
	UDS1PlayerAttributeSet* AttributeSet;

protected:
	UPROPERTY(EditAnywhere, Category = Effect)
	TSubclassOf<UGameplayEffect> InitStatEffectClass;


	// Effect Section
protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactParticle;

	// Montage Section
protected:
	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimFront;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimBack;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimLeft;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimRight;
public:
	ADS1Enemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION()
	virtual void OnDeath();

protected:
	void ImpactEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker);
	UAnimMontage* GetHitReactAnimation(const AActor* Attacker) const;
};
