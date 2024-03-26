// Zombies

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enums/EWeapons/EPovType.h"
#include "Structs/FMeshAttachmentInfo.h"
#include "WeaponBaseDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DATAASSETS_API UWeaponBaseDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayPriority = 1))
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayPriority = 1))
	EPovType PovType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Settings")
	float WeaponFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Settings")
	bool bWeaponAutomatic;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Settings")
	float WeaponDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	TMap<FName, TSoftObjectPtr<USkeletalMesh>> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	TMap<FName, FMeshAttachmentInfo> WeaponStaticMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TMap<FName, TSoftObjectPtr<UBlendSpace>> BlendSpaces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TMap<FName,TSoftObjectPtr<UAnimSequence>> AnimSequences;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TMap<FName, TSoftObjectPtr<UAnimMontage>> AnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TMap<FName,TSoftObjectPtr<UAnimMontage>> WeaponAnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TMap<FName, TSoftObjectPtr<class USoundCue>> SoundCues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UPrimaryDataAsset* WeaponProxyDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TSubclassOf<UAnimInstance> WeaponAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	TMap<FName, UTexture2D*> Crosshairs;
};
