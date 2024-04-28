// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paradigm_IQ/Core/Data/DataTables/DataTables.h"
#include "Logging/StructuredLog.h"
#include "Engine/DamageEvents.h"
#include "Paradigm_IQ/Core/Weapons/Projectile.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "Weapons.generated.h"

class AWeaponUpgradeManager;

UCLASS()
class PARADIGM_IQ_API AWeapons : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapons();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|DataTables", meta = (ToolTip = "The Data Table that holds the data of the Weapons."))
	TSoftObjectPtr<UDataTable> WeaponsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables|Stats", meta = (ToolTip = "The name to the row in the data table of this Weapon."))
	FName WeaponName;

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
	float FireRateTracker = 0.f;

	UPROPERTY()
	float BulletDelayTracker = 0.f;

	UPROPERTY()
	AActor* PlayerCharacter = nullptr;

	UPROPERTY()
	AWeaponUpgradeManager* UpgradeManagerRef = nullptr;

	UPROPERTY()
	FWeaponUpgrades WeaponUpgrade;


	UFUNCTION()
	virtual void WeaponTriggered(const float DeltaTime);

	UFUNCTION()
	TArray<AActor*> FindClosestEnemies(const float DistanceCheck);

	UFUNCTION()
	AActor* FindClosestEnemy();

	UFUNCTION()
	virtual TArray<FHitResult> LineTrace(const FVector ActorStartLocation, const FVector ActorEndLocation);

	UFUNCTION()
	virtual TArray<FHitResult> SphereTrace(const FVector ActorStartLocation, const FVector ActorEndLocation, const float TraceRadius);

	UFUNCTION()
	void ApplyDamage(const FHitResult ActorHit);

	UFUNCTION()
	TSubclassOf<AProjectile> SpawnProjectile(FTransform Transform);

	UFUNCTION()
	virtual void SpecialUpgrade1();

	UFUNCTION()
	virtual void SpecialUpgrade2();

	UFUNCTION()
	virtual void SpecialUpgrade3();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void UpgradeWeapon(FWeaponUpgrades WeaponUpgrades);

	FORCEINLINE AActor*						GetPlayerCharacter()			const { return PlayerCharacter; }
	FORCEINLINE TSoftObjectPtr<UDataTable>	GetWeaponsDataTable()			const { return WeaponsDataTable; }
};
