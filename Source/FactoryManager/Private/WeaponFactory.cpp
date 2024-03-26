// Zombies


#include "Weapons/WeaponFactory.h"
#include "DataAssets/Public/Enums/EWeapons/EWeaponTypes.h"
#include "WeaponTypes/WeaponFirearm.h"
#include "WeaponTypes/WeaponProxy.h"

// Sets default values
AWeaponFactory::AWeaponFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
}

void AWeaponFactory::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


AActor* AWeaponFactory::CreateWeapon(EWeaponTypes WeaponType, UPrimaryDataAsset* DataAsset, UWorld* World,
								AActor* Actor)
{
	switch (WeaponType)
	{
	case EWeaponTypes::EWT_Proxy:
		return CreateWeaponProxy(World, DataAsset, Actor);
	case EWeaponTypes::EWT_Firearm:
		return CreateFirearm(World, DataAsset);
	case EWeaponTypes::EWT_Melee:
		return CreateMelee(DataAsset);
	default:
		return nullptr;
	}
}

AWeaponProxy* AWeaponFactory::CreateWeaponProxy(UWorld* World, UPrimaryDataAsset* DataAsset, AActor* Actor)
{
	AWeaponProxy* Weapon = World->SpawnActor<AWeaponProxy>(AWeaponProxy::StaticClass(), Actor->GetActorLocation(),
													Actor->GetActorRotation());
	if (DataAsset)
	{
		Weapon->SetDataAsset(DataAsset);
	}
	return Weapon;
}

AWeaponFirearm* AWeaponFactory::CreateFirearm(UWorld* World, UPrimaryDataAsset* DataAsset)
{
	AWeaponFirearm* Weapon = World->SpawnActor<AWeaponFirearm>(AWeaponFirearm::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

	if (DataAsset)
	{
		Weapon->SetFirearmDataAsset(DataAsset);
	}
	return Weapon;
}

AActor* AWeaponFactory::CreateMelee(UPrimaryDataAsset* DataAsset)
{
	return nullptr;
}

