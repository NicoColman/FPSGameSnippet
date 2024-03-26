// Zombies

#pragma once

#include "CoreMinimal.h"
#include "AssetInterfaces/MeshManagerInterface.h"
#include "Components/ActorComponent.h"
#include "DataAssets/Public/Structs/FMeshAttachmentInfo.h"
#include "Enums/EWeapons/EPovType.h"
#include "ActorInitializeMeshComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COREACTORCOMPONENTS_API UActorInitializeMeshComponent : public UActorComponent, public IMeshManagerInterface
{
	GENERATED_BODY()

public:
	UActorInitializeMeshComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	 * Mesh Manager Interface
	 */
	virtual void InitializeAsyncLoading() override;
	
	void SetSkeletalMeshComponent(USkeletalMeshComponent* NewSkeletalMeshComponent);
	void SetEPovType(EPovType NewEPovType);
	
	virtual void SetDataAssetSkeletalMeshes(const  TMap<FName, TSoftObjectPtr<USkeletalMesh>>& NewMeshes) override;
	virtual USkeletalMesh* GetSkeletalMesh(FName SkeletalName) override;
	virtual void SetSkeletalMesh() override;

	virtual void SetDataAssetStaticMeshes(const TMap<FName, struct FMeshAttachmentInfo>& NewMeshes) override;
	virtual UStaticMeshComponent* GetStaticMeshes(FName MeshName) override;
	virtual void SetStaticMeshes() override;

	virtual void AsyncLoadSkeletalMesh() override;
	virtual void AsyncLoadStaticMesh()  override;

	virtual void OnSkeletonAssetLoaded() override;
	virtual void OnStaticMeshAssetLoaded() override;

	virtual void OnAllSkeletonAssetsLoaded() override;
	virtual void OnAllStaticMeshAssetsLoaded() override;

	virtual UStaticMeshComponent* CreateStaticMeshComponent(UStaticMesh* StaticMesh) override;
	virtual void AttachMeshComponentToSocket(UStaticMeshComponent* MeshComponent, USceneComponent* Component,
		FName SocketName) override;
	virtual void ReattachMeshComponentToSocket(UStaticMeshComponent* MeshComponent, UStaticMeshComponent* NewParentComponent,
		FName SocketName) override;

	void HideShowMeshes(bool bHide);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	USkeletalMeshComponent* ActorSkeletalMeshComponent;
	UPROPERTY()
	USkeletalMesh *ActorSkeletalMesh;

	UPROPERTY()
	TMap<FName, TSoftObjectPtr<USkeletalMesh>> SkeletalMeshes;
	TMap<FName, FMeshAttachmentInfo> StaticMeshes;
	
	EPovType PovType;

	TArray<FSoftObjectPath> LoadedSkeletalMeshPaths;
	TArray<FSoftObjectPath> LoadedStaticMeshPaths;
	
	UPROPERTY()
	TMap<FName, UStaticMeshComponent*> MeshComponentsMap;
	UPROPERTY()
	TMap<FName, USceneComponent*> SceneComponentsMap;

	int TotalAssetsToLoad;
	int LoadedAssetsCount;


public:
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() const { return ActorSkeletalMeshComponent; }
};
