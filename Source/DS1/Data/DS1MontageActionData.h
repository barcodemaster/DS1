// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DS1MontageActionData.generated.h"



USTRUCT(BlueprintType)
struct FDS1MontageGroup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> Animations;
};

/**
 * 특정 무기에 부여되는 몽타주 데이터. GameplayTag에 해당하는 몽타주를 Map으로 key value로 저장
 * GameplayTag에 해당하는 몽타주가 여러개일수있음 FDS1MontageGroup이라는 구조체에 TArray로 저장되어있음
 */
UCLASS()
class DS1_API UDS1MontageActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="Montage Groups")
	TMap<FGameplayTag, FDS1MontageGroup> MontageGroupMap;

public:
	UAnimMontage* GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index);

};
