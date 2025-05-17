// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/DS1Weapon.h"

#include "DS1GameplayTags.h"
#include "Components/DS1CombatComponent.h"
#include "Components/DS1WeaponCollisionComponent.h"
#include "Data/DS1MontageActionData.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "GAS/DS1WeaponAttributeSet.h"

ADS1Weapon::ADS1Weapon()
{
	WeaponCollision = CreateDefaultSubobject<UDS1WeaponCollisionComponent>("WeaponCollision");
	WeaponCollision->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);

	StaminaCostMap.Add(DS1GameplayTags::Character_Attack_Light, 7.f);
	StaminaCostMap.Add(DS1GameplayTags::Character_Attack_Running, 12.f);
	StaminaCostMap.Add(DS1GameplayTags::Character_Attack_Special, 15.f);
	StaminaCostMap.Add(DS1GameplayTags::Character_Attack_Heavy, 20.f);

	AttributeSet = CreateDefaultSubobject<UDS1WeaponAttributeSet>(TEXT("AttributeSet"));
}

void ADS1Weapon::BeginPlay()
{
	Super::BeginPlay();

	ApplyEffectToSelf(InitialStatEffect);
}

void ADS1Weapon::EquipItem()
{
	Super::EquipItem();

	CombatComponent = GetOwner()->GetComponentByClass<UDS1CombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;

		AttachToOwner(AttachSocket);

		// 무기의 충돌 트레이스 컴포넌트에 무기 메쉬 컴포넌트를 설정합니다.
		WeaponCollision->SetWeaponMesh(Mesh);

		// 무기를 소유한 OwnerActor를 충돌에서 무시합니다.
		WeaponCollision->AddIgnoredActor(GetOwner());
	}
}

UAnimMontage* ADS1Weapon::GetMontageForTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageActionData->GetMontageForTag(Tag, Index);
}

TSubclassOf<UGameplayEffect> ADS1Weapon::GetStaminaCostEffect(const FGameplayTag& Tag) const
{
	if (const TSubclassOf<UGameplayEffect>* FoundEffect = StaminaCostEffectMap.Find(Tag))
	{
		return *FoundEffect;
	}

	return nullptr;
}

float ADS1Weapon::GetStaminaCost(const FGameplayTag& Tag) const
{
	if (const float* Cost = StaminaCostMap.Find(Tag))
		return *Cost;

	return 0;
}

TSubclassOf<UGameplayEffect> ADS1Weapon::GetDamageEffectClass() const
{
	if (AActor* OwnerActor = GetOwner())
	{
		FGameplayTag LastAttackTag = CombatComponent->GetLastAttackType();

		if (const TSubclassOf<UGameplayEffect>* EffectClass = DamageEffectMap.Find(LastAttackTag))
		{
			return *EffectClass;
		}
	}

	return nullptr;
}

/** WeaponCollisionComponent에서 Trace로 Hit을 했으면 BroadCast를하고 이함수가 호출된다.*/
void ADS1Weapon::OnHitActor(const FHitResult& Hit)
{
	if (AActor* TargetActor = Hit.GetActor())
	{		
		TSubclassOf<UGameplayEffect> ApplyEffectClass = GetDamageEffectClass();
			
		ApplyEffectToTarget(ApplyEffectClass, TargetActor);

		FVector DamageDirection = GetOwner()->GetActorForwardVector();

		UGameplayStatics::ApplyPointDamage(
			TargetActor,
			10.f,
			DamageDirection,
			Hit,
			GetOwner()->GetInstigatorController(),
			this,
			nullptr);
	}
	
}

void ADS1Weapon::ApplyEffectToTarget(TSubclassOf<UGameplayEffect>& EffectClass, AActor* TargetActor)
{
	if (EffectClass && TargetActor)
	{
		UAbilitySystemComponent* TargetASC = TargetActor->FindComponentByClass<UAbilitySystemComponent>();
		if (TargetASC == nullptr)
			return;
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);
		if (SpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
		}
	}
}

void ADS1Weapon::ApplyEffectToSelf(TSubclassOf<UGameplayEffect>& EffectClass)
{

	if (EffectClass)
	{
		UE_LOG(LogTemp, Log, TEXT("Applying Effect: %s"), *EffectClass->GetName());

		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			if (ActiveHandle.WasSuccessfullyApplied())
			{
				UE_LOG(LogTemp, Log, TEXT("Effect successfully applied"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Effect failed to apply"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid SpecHandle"));
		}
	}

}


