// Zombies

#pragma once

#include "CoreMinimal.h"
#include "FactoryInterfaces/WeaponFactoryInterface.h"
#include "GameFramework/Actor.h"
#include "WeaponFactory.generated.h"

UCLASS()
class FACTORYMANAGER_API AWeaponFactory : public AActor, public IWeaponFactoryInterface
{
	GENERATED_BODY()
	
public:	
	AWeaponFactory();
	virtual void Tick(float DeltaTime) override;


	static AActor* CreateWeapon(EWeaponTypes WeaponType, UPrimaryDataAsset* DataAsset, UWorld* World, AActor* Actor);

protected:
	virtual void BeginPlay() override;
	
private:
	static class AWeaponProxy* CreateWeaponProxy(UWorld* World, UPrimaryDataAsset* DataAsset, AActor* Actor);
	static class AWeaponFirearm* CreateFirearm(UWorld* World, UPrimaryDataAsset* DataAsset);
	static AActor* CreateMelee(UPrimaryDataAsset* DataAsset);

};
