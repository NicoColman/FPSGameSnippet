// Zombies


#include "Actors/ActorInitializeMeshComponent.h"

#include "AssetInterfaces/OnAllMeshesLoadedInterface.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"


UActorInitializeMeshComponent::UActorInitializeMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ActorSkeletalMeshComponent = nullptr;
	ActorSkeletalMesh = nullptr;
	TotalAssetsToLoad = 0;
	LoadedAssetsCount = 0;
	PovType = EPovType::EPT_FirstPerson;
}

void UActorInitializeMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	AsyncLoadSkeletalMesh();
}

void UActorInitializeMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UActorInitializeMeshComponent::InitializeAsyncLoading()
{
	AsyncLoadSkeletalMesh();
}

void UActorInitializeMeshComponent::SetSkeletalMeshComponent(USkeletalMeshComponent* NewSkeletalMeshComponent)
{
	ActorSkeletalMeshComponent = NewSkeletalMeshComponent;
}

void UActorInitializeMeshComponent::SetEPovType(EPovType NewEPovType)
{
	PovType = NewEPovType;
}

void UActorInitializeMeshComponent::SetDataAssetSkeletalMeshes(const TMap<FName, TSoftObjectPtr<USkeletalMesh>>& NewMeshes)
{
	SkeletalMeshes = NewMeshes;
}

USkeletalMesh* UActorInitializeMeshComponent::GetSkeletalMesh(FName SkeletalName)
{
	const TSoftObjectPtr<USkeletalMesh>* AssetPtr = SkeletalMeshes.Find(SkeletalName);
	if (!AssetPtr || !AssetPtr->IsValid()) return nullptr;
	return AssetPtr->Get();
}

void UActorInitializeMeshComponent::SetSkeletalMesh()
{
	ActorSkeletalMesh = GetSkeletalMesh(FName(TEXT("WeaponSkeletalMesh")));
	if (ActorSkeletalMesh)
	{
		ActorSkeletalMeshComponent->SetSkeletalMesh(ActorSkeletalMesh);
		if (PovType == EPovType::EPT_ThirdPerson)
		{
			ActorSkeletalMeshComponent->SetOwnerNoSee(true);
		}
		AsyncLoadStaticMesh();
	}
}

void UActorInitializeMeshComponent::SetDataAssetStaticMeshes(const TMap<FName, FMeshAttachmentInfo>& NewMeshes)
{
	StaticMeshes = NewMeshes;
}

UStaticMeshComponent* UActorInitializeMeshComponent::GetStaticMeshes(FName MeshName)
{
	if (UStaticMeshComponent* const* MeshComponentPtr = MeshComponentsMap.Find(MeshName))
	{
		return *MeshComponentPtr;
	}
	return nullptr;
}

void UActorInitializeMeshComponent::SetStaticMeshes()
{
	if (!ActorSkeletalMeshComponent) return;

	// First loop: Create and register all mesh components
	for (const auto& Pair : StaticMeshes)
	{
		FName SocketName = Pair.Key;
		const FMeshAttachmentInfo& MeshInfo = Pair.Value;
		TSoftObjectPtr<UStaticMesh> MeshAsset = MeshInfo.Mesh;
        
		if (!MeshAsset.IsValid()) continue;

		UStaticMesh* StaticMesh = MeshAsset.Get();
		if (!StaticMesh) continue;
        
		UStaticMeshComponent* NewMeshComponent = CreateStaticMeshComponent(StaticMesh);
		NewMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (PovType == EPovType::EPT_ThirdPerson)
		{
			NewMeshComponent->SetOwnerNoSee(true);
		}
		if (!NewMeshComponent) continue;

		AttachMeshComponentToSocket(NewMeshComponent, ActorSkeletalMeshComponent, SocketName);

		MeshComponentsMap.Add(SocketName, NewMeshComponent);
	}

	// Second loop: Handle attachments to other mesh components
	for (const auto& Pair : StaticMeshes)
	{
		FName SocketName = Pair.Key;
		const FMeshAttachmentInfo& MeshInfo = Pair.Value;

		UStaticMeshComponent* NewMeshComponent = MeshComponentsMap.FindRef(SocketName);
		if (!NewMeshComponent) continue;

		if (!MeshInfo.SocketName.IsNone())
		{
			UStaticMeshComponent* TargetSocketComponent = MeshComponentsMap.FindRef(MeshInfo.SocketName);
			if (TargetSocketComponent)
			{
				ReattachMeshComponentToSocket(NewMeshComponent, TargetSocketComponent, SocketName);
			}
		}
	}
}

void UActorInitializeMeshComponent::AsyncLoadSkeletalMesh()
{
	if (!SkeletalMeshes.Num()) return;
	
	LoadedSkeletalMeshPaths.Empty(); 

	for (const auto& MeshEntry : SkeletalMeshes)
	{
		TotalAssetsToLoad++;
		const TSoftObjectPtr<USkeletalMesh>& MeshPtr = MeshEntry.Value;
		FSoftObjectPath AssetPath = MeshPtr.ToSoftObjectPath();
		LoadedSkeletalMeshPaths.Add(AssetPath); // Store the path for later unloading

		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableManager.RequestAsyncLoad(AssetPath, FStreamableDelegate::CreateUObject(this,
			&UActorInitializeMeshComponent::OnSkeletonAssetLoaded));
	}
}

void UActorInitializeMeshComponent::AsyncLoadStaticMesh()
{
	if (!StaticMeshes.Num())
	{
		return;
	}
	
	LoadedStaticMeshPaths.Empty(); 

	for (const auto& MeshEntry : StaticMeshes)
	{
		TotalAssetsToLoad++;
		const TSoftObjectPtr<UStaticMesh>& StaticMeshPtr = MeshEntry.Value.Mesh;
		FSoftObjectPath AssetPath = StaticMeshPtr.ToSoftObjectPath();
		LoadedStaticMeshPaths.Add(AssetPath); // Store the path for later unloading

		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableManager.RequestAsyncLoad(AssetPath, FStreamableDelegate::CreateUObject(this,
			&UActorInitializeMeshComponent::OnStaticMeshAssetLoaded));
	}
}

void UActorInitializeMeshComponent::OnSkeletonAssetLoaded()
{
	LoadedAssetsCount++;
	if (LoadedAssetsCount == TotalAssetsToLoad)
	{
		OnAllSkeletonAssetsLoaded();
	}
}

void UActorInitializeMeshComponent::OnStaticMeshAssetLoaded()
{
	LoadedAssetsCount++;
	if (LoadedAssetsCount == TotalAssetsToLoad)
	{
		OnAllStaticMeshAssetsLoaded();
	}
}

void UActorInitializeMeshComponent::OnAllSkeletonAssetsLoaded()
{
	SetSkeletalMesh();
	AActor* Owner = GetOwner();
	if (Owner)
	{
		IOnAllMeshesLoadedInterface* MeshInterface = Cast<IOnAllMeshesLoadedInterface>(Owner);
		if (MeshInterface)
		{
			MeshInterface->OnAllSkeletalMeshAssetsLoaded();
		}
		TArray<UActorComponent*> Components = Owner->GetComponentsByTag(UActorComponent::StaticClass(), TEXT("OnAllMeshesLoadedInterface"));
		for (UActorComponent* Component : Components)
		{
			IOnAllMeshesLoadedInterface* ActorComponentMeshInterface = Cast<IOnAllMeshesLoadedInterface>(Component);
			if (MeshInterface)
			{
				ActorComponentMeshInterface->OnAllSkeletalMeshAssetsLoaded();
			}
		}
	}
}

void UActorInitializeMeshComponent::OnAllStaticMeshAssetsLoaded()
{
	SetStaticMeshes();
	AActor* Owner = GetOwner();
	if (Owner)
	{
		IOnAllMeshesLoadedInterface* MeshInterface = Cast<IOnAllMeshesLoadedInterface>(Owner);
		if (MeshInterface)
		{
			MeshInterface->OnAllStaticMeshAssetsLoaded();
		}
		TArray<UActorComponent*> Components = Owner->GetComponentsByTag(UActorComponent::StaticClass(), TEXT("OnAllMeshesLoadedInterface"));
		for (UActorComponent* Component : Components)
		{
			IOnAllMeshesLoadedInterface* ActorComponentMeshInterface = Cast<IOnAllMeshesLoadedInterface>(Component);
			if (MeshInterface)
			{
				ActorComponentMeshInterface->OnAllStaticMeshAssetsLoaded();
			}
		}
	}
}

UStaticMeshComponent* UActorInitializeMeshComponent::CreateStaticMeshComponent(UStaticMesh* StaticMesh)
{
	UStaticMeshComponent* NewMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), NAME_None);
	if (NewMeshComponent)
	{
		NewMeshComponent->SetStaticMesh(StaticMesh);
		NewMeshComponent->RegisterComponent();
	}
	return NewMeshComponent;
}

void UActorInitializeMeshComponent::AttachMeshComponentToSocket(UStaticMeshComponent* MeshComponent,
	USceneComponent* Component, FName SocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	MeshComponent->AttachToComponent(Component, AttachmentRules, SocketName);
}

void UActorInitializeMeshComponent::ReattachMeshComponentToSocket(UStaticMeshComponent* MeshComponent,
	UStaticMeshComponent* NewParentComponent, FName SocketName)
{
	MeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	AttachMeshComponentToSocket(MeshComponent, NewParentComponent, SocketName);
}

void UActorInitializeMeshComponent::HideShowMeshes(bool bHide)
{
	if (PovType == EPovType::EPT_ThirdPerson) return;
	ActorSkeletalMeshComponent->SetVisibility(!bHide);
	for (const auto& Pair : MeshComponentsMap)
	{
		UStaticMeshComponent* MeshComponent = Pair.Value;
		if (!MeshComponent) return;
		if (bHide)
		{
			MeshComponent->SetVisibility(false);
		}
		else
		{
			MeshComponent->SetVisibility(true);
		}
	}
}

