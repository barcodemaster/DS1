// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "DS1Equipment.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS()
class DS1_API ADS1Equipment : public AActor , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Equipment | Mesh")
	UStaticMesh* MeshAsset;

	UPROPERTY(VisibleAnywhere, Category = "Equipment | Mesh")
	UStaticMeshComponent* Mesh;

protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;
	
public:	
	ADS1Equipment();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void EquipItem();

	virtual void UnequipItem();

	/** 이 함수는 Weapon클래스에서 Equip함수에서 어느소켓에 붙을건지할때 사용된다 */
	virtual void AttachToOwner(FName SocketName);

	
};


