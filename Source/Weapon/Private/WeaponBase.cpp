// Zombies


#include "WeaponTypes/WeaponBase.h"
#include "CoreActorComponents/Public/Actors/ActorInitializeMeshComponent.h"
#include "CoreActorComponents/Public/Actors/InitializeActorAsset/ActorInitializeAnimationComponent.h"
#include "CoreActorComponents/Public/Actors/InitializeActorAsset/ActorInitializeSoundComponent.h"
#include "Weapons/Public/Components/WeaponBase/WeaponBaseCombatComponent.h"
#include "DataAssets/Public/Weapons/WeaponBaseDataAsset.h"
#include "DataAssets/Public/Structs/FMeshAttachmentInfo.h"
#include "DrawDebugHelpers.h"
#include "GameEventManager/GameEventManager.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	RootComponent = WeaponMeshComponent;

	InitializeMeshComponent = CreateDefaultSubobject<UActorInitializeMeshComponent>(TEXT("InitializeMeshComponent"));
	InitializeMeshComponent->SetIsReplicated(true);
	InitializeAnimationComponent = CreateDefaultSubobject<UActorInitializeAnimationComponent>(TEXT("InitializeAnimationComponent"));
	InitializeAnimationComponent->SetIsReplicated(true);
	InitializeSoundComponent = CreateDefaultSubobject<UActorInitializeSoundComponent>(TEXT("InitializeSoundComponent"));
	InitializeSoundComponent->SetIsReplicated(true);
	
	Tags.AddUnique("Weapon");
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitializeMeshComponent->SetSkeletalMeshComponent(WeaponMeshComponent);
	SetCombatComponent();
}

void AWeaponBase::SetCombatComponent()
{
	CombatComponent = NewObject<UWeaponBaseCombatComponent>(this, TEXT("WeaponBaseCombatComponent"));
	CombatComponent->SetIsReplicated(true);
	CombatComponent->RegisterComponent();
	CombatComponent->ComponentTags.AddUnique("WeaponCombatComponent");
	CombatComponent->ComponentTags.AddUnique("OnAllMeshesLoadedInterface");
}

UActorComponent* AWeaponBase::GetCombatComponent() const
{
	return CombatComponent;
}

USkeletalMeshComponent* AWeaponBase::GetMeshComponent() const
{
	return WeaponMeshComponent;
}

UPrimaryDataAsset* AWeaponBase::GetDataAsset() const
{
	if (WeaponDataAsset && WeaponDataAsset->WeaponProxyDataAsset)
	{
		return WeaponDataAsset->WeaponProxyDataAsset;
	}
	return nullptr;
}

TMap<FName, TSoftObjectPtr<USkeletalMesh>> AWeaponBase::GetDataAssetSkeletalMeshes() const
{
	if (WeaponDataAsset)
	{
		return WeaponDataAsset->WeaponMesh;
	}
	return TMap<FName, TSoftObjectPtr<USkeletalMesh>>();
}

TMap<FName, FMeshAttachmentInfo> AWeaponBase::GetDataAssetStaticMeshes() const
{
	if (WeaponDataAsset)
	{
		return WeaponDataAsset->WeaponStaticMeshes;
	}
	return TMap<FName, FMeshAttachmentInfo>();
}

TMap<FName, TSoftObjectPtr<UBlendSpace>> AWeaponBase::GetDataAssetBlendSpaces() const
{
	return WeaponDataAsset->BlendSpaces;
}

TMap<FName, TSoftObjectPtr<UAnimSequence>> AWeaponBase::GetDataAssetAnimSequences() const
{
	return WeaponDataAsset->AnimSequences;
}

TMap<FName, TSoftObjectPtr<UAnimMontage>> AWeaponBase::GetDataAssetAnimMontages() const
{
	return WeaponDataAsset->AnimMontages;
}

TMap<FName, TSoftObjectPtr<UAnimMontage>> AWeaponBase::GetDataAssetWeaponAnimMontages() const
{
	return WeaponDataAsset->WeaponAnimMontages;
}

TMap<FName, TSoftObjectPtr<USoundCue>> AWeaponBase::GetDataAssetSoundCues() const
{
	return WeaponDataAsset->SoundCues;
}

EPovType AWeaponBase::GetDataAssetPovType() const
{
	return WeaponDataAsset->PovType;
}

TSubclassOf<UAnimInstance> AWeaponBase::GetWeaponAnimInstance() const
{
	return WeaponDataAsset->WeaponAnimInstance;
}

void AWeaponBase::OnAllSkeletalMeshAssetsLoaded()
{
	if (GetWeaponAnimInstance())
	{
		WeaponMeshComponent->SetAnimInstanceClass(GetWeaponAnimInstance());
	}
}

void AWeaponBase::OnAllStaticMeshAssetsLoaded()
{
	
}

void AWeaponBase::SetFirearmDataAsset(UPrimaryDataAsset* DataAsset)
{
	if (DataAsset)
	{
		WeaponDataAsset = Cast<UWeaponBaseDataAsset>(DataAsset);
	}
}

void AWeaponBase::InitializeDataComponents()
{
	if (InitializeMeshComponent)
	{
		InitializeMeshComponent->SetDataAssetSkeletalMeshes(GetDataAssetSkeletalMeshes());
		InitializeMeshComponent->SetDataAssetStaticMeshes(GetDataAssetStaticMeshes());
		InitializeMeshComponent->SetEPovType(GetDataAssetPovType());
		InitializeMeshComponent->InitializeAsyncLoading();
	}
	if (InitializeAnimationComponent)
	{
		InitializeAnimationComponent->SetDataAssetAnimSequences(GetDataAssetAnimSequences());
		InitializeAnimationComponent->SetDataAssetAnimMontages(GetDataAssetAnimMontages());
		InitializeAnimationComponent->SetDataAssetBlendSpaces(GetDataAssetBlendSpaces());
		InitializeAnimationComponent->InitializeAsyncLoading();
	}
	if (InitializeSoundComponent)
	{
		InitializeSoundComponent->SetDataAssetSoundCues(GetDataAssetSoundCues());
		InitializeSoundComponent->InitializeAsyncLoading();
	}
}
