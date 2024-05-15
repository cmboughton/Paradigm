// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Logging/StructuredLog.h"
#include "Paradigm_IQ/Core/Data/Structs/Structs.h"
#include "Weapons.generated.h"

class APlayerCharacter;
class AProjectile;
class AWeaponUpgradeManager;

UCLASS()
class PARADIGM_IQ_API AWeapons : public AActor
{
	GENERATED_BODY()

protected:
#pragma region Protected Functions

	AWeapons();

	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaTime) override;

	UFUNCTION()
	virtual void WeaponTriggered(const float DeltaTime);

	UFUNCTION()
	TArray<AActor*> FindClosestEnemies(const float& DistanceCheck, const FVector& Origin) const;

	UFUNCTION()
	virtual AActor* FindClosestEnemy(const FVector& Origin) const;

	UFUNCTION()
	virtual TArray<FHitResult> LineTrace(const FVector& ActorStartLocation, const FVector& ActorEndLocation);

	UFUNCTION()
	virtual TArray<FHitResult> SphereTrace(const FVector& ActorStartLocation, const FVector& ActorEndLocation, const float& TraceRadius);

	UFUNCTION()
	virtual void ApplyDamage(const TArray<FHitResult>& AllActorsHit);

	UFUNCTION()
	TSubclassOf<AProjectile> SpawnProjectile(const FTransform& Transform) const;

	UFUNCTION()
	static FVector GetRandomPointNearOrigin(const FVector& Origin, const float MinDistance, const float MaxDistance);

	UFUNCTION()
	float UpdateDamage();

#pragma endregion

#pragma region Protected Variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Weapons."))
	TSoftObjectPtr<UDataTable> WeaponsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The name to the row in the data table of this Weapon."))
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "Enable to create a small delay between each bullet shot."))
	bool bShouldUseBulletDelay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "The explosion radius of the grenade."))
	float AffectRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "Total Number of projectiles to spawn per shot."), meta = (ClampMin = 1), meta = (ClampMax = 50))
	int TriggerAmount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Stats", meta = (ToolTip = "Blueprint ref to the base bullet being shot by auto shooter."))
	TSubclassOf<class AProjectile> Projectile = nullptr;

	UPROPERTY(meta = (ClampMin = 0.1f))
	float FireRate = 0.f;

	UPROPERTY()
	float Damage = 0.f;

	UPROPERTY()
	float DamageModifier = 1.f;

	UPROPERTY()
	float FireRateTracker = 0.f;

	UPROPERTY()
	float BulletDelayTracker = 0.f;

	UPROPERTY()
	AActor* PlayerCharacter = nullptr;

	UPROPERTY()
	AWeaponUpgradeManager* UpgradeManagerRef = nullptr;

	UPROPERTY()
	FWeaponUpgrades WeaponUpgrade;

	UPROPERTY()
	bool bSpecialUpgrade1 = false;

	UPROPERTY()
	bool bSpecialUpgrade2 = false;

	UPROPERTY()
	bool bSpecialUpgrade3 = false;

	UPROPERTY()
	bool bSpecialUpgrade4 = false;

	UPROPERTY()
	bool bSpecialUpgrade5 = false;

	UPROPERTY()
	int SpecialUpgradeTracker = 0;

	UPROPERTY()
	int WeaponLevel = 0;

#pragma endregion

public:	
#pragma region Public Functions

	UFUNCTION()
	void UpgradeWeapon(const FWeaponUpgrades& WeaponUpgrades);


	UFUNCTION(BlueprintImplementableEvent)
	void WeaponTriggeredBP();

#pragma endregion

#pragma region Getters and Setters

	FORCEINLINE AActor*						GetPlayerCharacter()											const { return PlayerCharacter; }
	FORCEINLINE TSoftObjectPtr<UDataTable>	GetWeaponsDataTable()											const { return WeaponsDataTable; }
	FORCEINLINE int							GetSpecialUpgradeTracker()										const { return SpecialUpgradeTracker; }
	FORCEINLINE int							GetWeaponLevel()												const { return  WeaponLevel; }

	FORCEINLINE void						SetPlayerCharacter	(AActor* PlayerCharRef)							  { PlayerCharacter = PlayerCharRef; }
	FORCEINLINE void						SetWeaponLevel		(const int& WeaponLevelRef)						  { WeaponLevel = WeaponLevelRef; }

#pragma endregion
};
