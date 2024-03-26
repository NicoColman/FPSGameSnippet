// Zombies

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MeshManagerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMeshManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COREINTERFACES_API IMeshManagerInterface
{
	GENERATED_BODY()

public:
	/**
	 * @details Initialize the async loading for the actor
	 */
	virtual void InitializeAsyncLoading() = 0;
	/**
	 * @details Set the skeletal mesh from the DataAsset so that it can be used in the AsyncLoadSkeletalMesh function
	 *	this makes it so that the UPrimaryDataAsset can be changed and the skeletal mesh will be updated in child classes
	 *	
	 * @param NewMeshes - The skeletal mesh TMap from the DataAsset
	 */
	virtual void SetDataAssetSkeletalMeshes(const TMap<FName, TSoftObjectPtr<USkeletalMesh>>& NewMeshes) = 0;
	/**
	 * @details Get the skeletal mesh from the AsyncLoadSkeletalMesh function
	 *
	 * @param SkeletalName - The name of the skeletal mesh to get
	 *	
	 * @return USkeletalMesh* - The skeletal mesh from the AsyncLoadSkeletalMesh function
	 */
	virtual USkeletalMesh* GetSkeletalMesh(FName SkeletalName) = 0;
	/**
	 * @details Set the skeletal mesh for the weapon after the AsyncLoadSkeletalMesh function has been called and completed
	 */
	virtual void SetSkeletalMesh() = 0;
    
	/**
	 * @details Set the StaticMesh from the DataAsset so that it can be used in the AsyncLoadStaticMesh function,
	 *  this makes it so that the UPrimaryDataAsset can be changed and the StaticMesh will be updated in child classes
	 *	
	 * @param  NewMeshes - The StaticMesh from the DataAsset. The FName is the socket name,
	 *	the FMeshAttachmentInfo contains a FName and a TSoftObjectPtr<UStaticMesh>, if the FName is not None, we will
	 *	search for the previous StaticMeshComponent with the same name and attach the new StaticMeshComponent to it. If
	 *	it is None we will attach the new StaticMeshComponent to the WeaponMeshComponent.
	 */
	virtual void SetDataAssetStaticMeshes(const TMap<FName, struct FMeshAttachmentInfo>& NewMeshes) = 0;
	/**
	 * @details Get the StaticMesh from the AsyncLoadStaticMesh function
	 *	
	 * @return UStaticMeshComponent* - The StaticMesh from the AsyncLoadStaticMesh function
	 */
	virtual UStaticMeshComponent* GetStaticMeshes(FName MeshName) = 0;
	/**
	 * @details Set the extra StaticMeshes for the weapon after the AsyncLoadStaticMesh function has been called and completed.
	 *	We are using two for loops because first we are creating the StaticMeshComponent and then we are attaching it
	 *	to the corresponding socket following the instruction from the getter function. With two for loops we can be sure
	 *	that the StaticMeshComponent is created before we try to attach it to the socket, in case we try to attach a not
	 *	yet created StaticMeshComponent to a socket.
	 */
	virtual void SetStaticMeshes() = 0;
	/**
	 * @details Load the skeletal mesh for the actor asynchronously
	 */
	virtual void AsyncLoadSkeletalMesh() = 0;
	/**
	 * @details Load the StaticMesh for the actor asynchronously
	 */
	virtual void AsyncLoadStaticMesh() = 0;
	/**
	 * @details Called when the asset has been loaded
	 */
	virtual void OnSkeletonAssetLoaded() = 0;
	/**
	 * @details Called when the asset has been loaded
	 */
	virtual void OnStaticMeshAssetLoaded() = 0;
	/**
	 * @details Called when all assets have been loaded
	 */
	virtual void OnAllSkeletonAssetsLoaded() = 0;
	/**
	 * @details Called when all assets have been loaded
	 */
	virtual void OnAllStaticMeshAssetsLoaded() = 0;
	/**
	 * @details Called in SetStaticMeshes to create the StaticMeshComponent
	 *
	 * @param StaticMesh - The StaticMesh from the DataAsset
	 */
	virtual UStaticMeshComponent* CreateStaticMeshComponent(UStaticMesh* StaticMesh) = 0;
	/**
	 * @details Called in SetStaticMeshes to attach the StaticMeshComponent to a SceneComponent to attach it to the
	 * socket name
	 *
	 * @param MeshComponent - The StaticMeshComponent to attach
	 * @param Component - The SceneComponent to attach the StaticMeshComponent to
	 * @param SocketName - The socket name to attach the SceneComponent to
	 */
	virtual void AttachMeshComponentToSocket(UStaticMeshComponent* MeshComponent, USceneComponent* Component,
		FName SocketName) = 0;
	/**
	 * @details Called in SetStaticMeshes to reattach the StaticMeshComponent to a new parent component in case it has
	 * to be attached to a later created StaticMeshComponent
	 *
	 * @param MeshComponent - The StaticMeshComponent to reattach
	 * @param NewParentComponent - The SceneComponent to attach the StaticMeshComponent to
	 * @param SocketName - The socket name to attach the SceneComponent to
	 */
	virtual void ReattachMeshComponentToSocket(UStaticMeshComponent* MeshComponent, UStaticMeshComponent* NewParentComponent,
		FName SocketName) = 0;
};
