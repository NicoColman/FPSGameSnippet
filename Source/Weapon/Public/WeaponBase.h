// Zombies

#pragma once

#include "CoreMinimal.h"
#include "ActorInterfaces/ActorMeshInterface.h"
#include "AssetInterfaces/GetDataAssetsInterface.h"
#include "AssetInterfaces/OnAllMeshesLoadedInterface.h"
#include "CoreInterfaces/Public/ActorInterfaces/ActorGetDataAssetInterface.h"
#include "GameFramework/Actor.h"
#include "DataAssets/Public/Structs/FMeshAttachmentInfo.h"
#include "Enums/EWeapons/EPovType.h"
#include "WeaponBase.generated.h"

UCLASS()
class WEAPONS_API AWeaponBase : public AActor, public IGetDataAssetsInterface, public IActorMeshInterface,
		public IActorGetDataAssetInterface, public IOnAllMeshesLoadedInterface

{
	GENERATED_BODY()

public:
	AWeaponBase();
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	/**
	 *
	 * @return SkeletalMeshComponent for the Weapon - IActorMeshInterface
	 */
	virtual USkeletalMeshComponent* GetMeshComponent() const override;

	/**
	 *
	 * @return Data Asset for the Firearm - IGetDataAssetsInterface
	 */
	virtual UPrimaryDataAsset* GetDataAsset() const override;
	
	/**
	 * 
	 * @return Data Asset for the Firearm - GetDataAssetInterface
	 */
	UFUNCTION()
	virtual TMap<FName, TSoftObjectPtr<USkeletalMesh>> GetDataAssetSkeletalMeshes() const override;
	UFUNCTION()
	virtual TMap<FName, FMeshAttachmentInfo> GetDataAssetStaticMeshes() const override;
	UFUNCTION()
	virtual TMap<FName, TSoftObjectPtr<UBlendSpace>> GetDataAssetBlendSpaces() const override;
	UFUNCTION()
	virtual TMap<FName, TSoftObjectPtr<UAnimSequence>> GetDataAssetAnimSequences() const override;
	UFUNCTION()
	virtual TMap<FName, TSoftObjectPtr<UAnimMontage>> GetDataAssetAnimMontages() const override;
	UFUNCTION()
	virtual TMap<FName, TSoftObjectPtr<UAnimMontage>> GetDataAssetWeaponAnimMontages() const override;
	UFUNCTION()
	virtual TMap<FName, TSoftObjectPtr<USoundCue>> GetDataAssetSoundCues() const override;

	virtual EPovType GetDataAssetPovType() const;
	virtual TSubclassOf<UAnimInstance> GetWeaponAnimInstance() const;

	/*
	 * OnAllMeshesLoadedInterface
	 */
	virtual void OnAllSkeletalMeshAssetsLoaded() override;
	virtual void OnAllStaticMeshAssetsLoaded() override;
	
	UFUNCTION()
	virtual void SetFirearmDataAsset(UPrimaryDataAsset* DataAsset);

	UFUNCTION()
	virtual void InitializeDataComponents();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UActorInitializeAnimationComponent* InitializeAnimationComponent;
	
protected:
	virtual void BeginPlay() override;

	virtual void SetCombatComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Components")
	USkeletalMeshComponent* WeaponMeshComponent;
	UPROPERTY()
	UActorComponent* CombatComponent;
private:

	UPROPERTY()
	class UActorInitializeMeshComponent* InitializeMeshComponent;
	UPROPERTY()
	class UActorInitializeSoundComponent* InitializeSoundComponent;

	UPROPERTY()
	class UWeaponBaseCombatComponent* WeaponBaseCombatComponent;
	
	UPROPERTY()
	class UWeaponBaseDataAsset* WeaponDataAsset;

	bool bCanStart;

public:
	// Getters
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshComponent() const { return WeaponMeshComponent; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UActorInitializeAnimationComponent* GetInitializeAnimationComponent() const { return InitializeAnimationComponent; }
	FORCEINLINE UActorInitializeMeshComponent* GetInitializeMeshComponent() const { return InitializeMeshComponent; }
	FORCEINLINE UActorInitializeSoundComponent* GetInitializeSoundComponent() const { return InitializeSoundComponent; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UWeaponBaseCombatComponent* GetWeaponBaseCombatComponent() const { return WeaponBaseCombatComponent; }
	virtual UActorComponent* GetCombatComponent() const;
};
