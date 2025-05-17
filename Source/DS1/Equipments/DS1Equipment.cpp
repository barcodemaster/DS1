// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/DS1Equipment.h"

#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"

// Sets default values
ADS1Equipment::ADS1Equipment()
{
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

UAbilitySystemComponent* ADS1Equipment::GetAbilitySystemComponent() const
{
	return ASC;
}

// Called when the game starts or when spawned
void ADS1Equipment::BeginPlay()
{
	Super::BeginPlay();
	
	ASC->InitAbilityActorInfo(this, this);
}

// Called every frame
void ADS1Equipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADS1Equipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshAsset)
	{
		Mesh->SetStaticMesh(MeshAsset);
	}
}

void ADS1Equipment::EquipItem()
{
}

void ADS1Equipment::UnequipItem()
{
}

void ADS1Equipment::AttachToOwner(FName SocketName)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
		}
	}
}


