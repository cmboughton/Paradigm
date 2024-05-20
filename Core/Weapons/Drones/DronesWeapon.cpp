// Fill out your copyright notice in the Description page of Project Settings.


#include "DronesWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Paradigm_IQ/Core/Character/BaseCharacter.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "Paradigm_IQ/Core/Weapons/Projectile.h"

void ADronesWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	//Spawns Drone meshes and their jets(niagara)
	if (bSetUpDrones || SpawnedDrones.Num() < TriggerAmount)
	{
		SpawnedDrones.Empty();
		bIsDroneAlive.Empty();
		for (int i = 0; i < TriggerAmount; i++)
		{
			bIsDroneAlive.Add(true);
			const FName DroneName = FName(*FString("DroneMesh").Append(FString::SanitizeFloat(i)));
			if (UStaticMeshComponent* DroneMesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), DroneName))
			{
				DroneMesh->RegisterComponent();
				DroneMesh->CreationMethod = EComponentCreationMethod::Instance;
				DroneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				if(bSpecialUpgrade5)
				{
					if (HealingDroneMeshRef)
					{
						DroneMesh->SetStaticMesh(HealingDroneMeshRef);
					}
				}
				else
				{
					if (DroneMeshRef)
					{
						DroneMesh->SetStaticMesh(DroneMeshRef);
					}
				}
				SpawnedDrones.Add(DroneMesh);
			}
		}
		SpawnedJets.Empty();
		for (int i = 0; i < SpawnedDrones.Num(); i++)
		{
			if (SpawnedDrones.IsValidIndex(i))
			{
				TArray<FName>DroneSocketNames = SpawnedDrones[i]->GetAllSocketNames();
				if (!DroneSocketNames.IsEmpty())
				{
					for (int j = 0; j < DroneSocketNames.Num(); j++)
					{
						const FName SocketName = FName(*FString("JetSocket").Append(FString::FromInt(j + 1)));
						UE_LOGFMT(LogTemp, Warning, "Socket: {0}", SocketName);
						if (SpawnedDrones[i]->DoesSocketExist(SocketName))
						{
							if (UNiagaraComponent* NiagaraComponent = NewObject<UNiagaraComponent>(this, UNiagaraComponent::StaticClass(), FName(*FString("Jet").Append(FString::FromInt(i)).Append(" ").Append(FString::FromInt(j)))))
							{
								NiagaraComponent->RegisterComponent();
								NiagaraComponent->CreationMethod = EComponentCreationMethod::Instance;
								//UE_LOGFMT(LogTemp, Warning, "NiagraSpawned: {0}", FName(*FString("Jet") + SocketName.ToString()));
								if (NiagaraComponent)
								{
									NiagaraComponent->SetAsset(JetNiagara);
									NiagaraComponent->AttachToComponent(SpawnedDrones[i], FAttachmentTransformRules::KeepRelativeTransform, SocketName);
									NiagaraComponent->Activate();
									SpawnedJets.Add(NiagaraComponent);
								}
							}
						}
					}
				}
			}
		}
		bSetUpDrones = false;
	}

	if(bSpecialUpgrade5)
	{
		DamageModifier = DamageReduction;
	}

	ApplyDamage(ActorsHit);
	ActorsHit.Empty();

	// Resets the Drones if all of them have been destroyed
	bool bDronesActive = false;
	for(const auto IsDroneAlive : bIsDroneAlive)
	{
		if(IsDroneAlive)
		{
			bDronesActive = true;
		}
	}
	if(!bDronesActive)
	{
		ResetDrones();
	}

	DroneRot = FRotator(0.f, 360 / TriggerAmount, 0.f);
	if(bIsExpanding && !bSpecialUpgrade4)
	{
		if(RadialDistance < (AffectRadius * 5))
		{
			RadialDistance += (DeltaTime * 400);
		}
		for (int i = 0; i < TriggerAmount; i++)
		{
			(RadialDistance < (AffectRadius * 5)) ? ExpandDrones(i, DeltaTime, 180) : RotateDronesForward(i, DeltaTime);
			const TArray<FHitResult> ActiveActorsHit = CheckSphereTrace(i);
			ActorsHit.Append(ActiveActorsHit);
		}
		
	}
	else if (bIsExpanding && bSpecialUpgrade4)
	{
		if(RadialDistance < (AffectRadius * 5))
		{
			RadialDistance += (DeltaTime * 400);
		}
		for (int i = 0; i < TriggerAmount; i++)
		{
			if (bIsDroneAlive.IsValidIndex(i))
			{
				if (bIsDroneAlive[i])
				{
					(RadialDistance < (AffectRadius * 5)) ? ExpandDrones(i, DeltaTime, 180) : RotateDronesForward(i, DeltaTime);
					const TArray<FHitResult> ActiveActorsHit = CheckSphereTrace(i);
					if (!ActiveActorsHit.IsEmpty())
					{
						for (auto ActorHit : ActiveActorsHit)
						{
							if (ActorHit.GetActor())
							{
								if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
								{
									SpecialUpgrade4(i, SpawnedDrones[i]->GetComponentLocation());
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if (RadialDistance > 0)
		{
			RadialDistance -= (DeltaTime * 400);
			for (int i = 0; i < TriggerAmount; i++)
			{
				if (!bSpecialUpgrade4)
				{
					ExpandDrones(i, DeltaTime, 0);
					const TArray<FHitResult> ActiveActorsHit = CheckSphereTrace(i);
					ActorsHit.Append(ActiveActorsHit);
				}
				else
				{
					if (bIsDroneAlive.IsValidIndex(i))
					{
						if (bIsDroneAlive[i])
						{
							ExpandDrones(i, DeltaTime, 0);
							const TArray<FHitResult> ActiveActorsHit = CheckSphereTrace(i);
							if (!ActiveActorsHit.IsEmpty())
							{
								for (FHitResult ActorHit : ActiveActorsHit)
								{
									if (ActorHit.GetActor())
									{
										if (ActorHit.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
										{
											SpecialUpgrade4(i, SpawnedDrones[i]->GetComponentLocation());
										}
									}
								}
							}
						}
					}
				}
			}
			if (bSpecialUpgrade2)
			{
				if (LaserLengthTracker <= 0)
				{
					LaserSweepTracker.Yaw = 0.f;
					LaserLengthTracker = 0.f;
					bStartSweep = false;
				}
				else
				{
					for (auto DroneMesh : SpawnedDrones)
					{
						const FVector StartLocation = DroneMesh->GetComponentLocation();
						const FRotator LaserRot = FRotator(0.f, 360 / TriggerAmount, 0.f);
						for (int i = 0; i < TriggerAmount; i++)
						{
							LaserLengthTracker -= DeltaTime * (((1 - FireRate) * 300) + 300);
							const FRotator NewLaserRot = FRotator(0.f, (LaserRot.Yaw * (i + 1)) + 45, 0.f) + LaserSweepTracker;
							const FVector EndLocation = StartLocation + NewLaserRot.Vector() * LaserLengthTracker;
							TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
							ActorsHit.Append(ActiveActorsHit);
						}
					}
				}
			}
		}
		else
		{
			ResetDrones();
		}
		//UE_LOGFMT(LogTemp, Warning, "LaserRot: {0}", FireRate);
	}

	SweepRotation.Yaw = DroneRot.Yaw * 10;

	// Drones staying up forever
	if(bSpecialUpgrade3)
	{
		SweepTracker.Yaw += DeltaTime * RotationSpeed;
		SweepTracker.Normalize();
		if (bSpecialUpgrade1)
		{
			SpecialUpgrade1(DeltaTime);
		}
		if (bSpecialUpgrade2)
		{
			SpecialUpgrade2(DeltaTime);
		}
	}
	else
	{
		if (RadialDistance >= (AffectRadius * 5))
		{
			if (DroneDurationTracker <= DroneDuration)
			{
				SweepTracker.Yaw += DeltaTime * RotationSpeed;
				SweepTracker.Normalize();
				DroneDurationTracker += DeltaTime;
				if (bSpecialUpgrade1)
				{
					SpecialUpgrade1(DeltaTime);
				}
				if (bSpecialUpgrade2)
				{
					SpecialUpgrade2(DeltaTime);
				}
				//UE_LOGFMT(LogTemp, Warning, "LaserRot: {0}", SweepTracker.Yaw);
			}
			else
			{
				bIsExpanding = false;
				//UE_LOGFMT(LogTemp, Warning, "Delay: {0}", LaserEndDelayTracker);
			}
		}
	}

	if(LifeStealImmuneTracker > 0)
	{
		LifeStealImmuneTracker -= DeltaTime;
		//UE_LOGFMT(LogTemp, Warning, "LifeStealImmuneTracker: {0}", LifeStealImmuneTracker);
	}
}

// Drones shooting Laser Projectile
/**
 * @brief This method implements the functionality for the first special upgrade of the drone weapon.
 *
 * The method is responsible for firing projectiles from the drones at a rate defined by `FireRate`. 
 * Each drone fires a projectile if it is alive. The projectile's properties are set according to the 
 * weapon's current upgrade state. The method also handles the cooldown between drone fires.
 *
 * @param DeltaTime The time elapsed since the last frame, used for the fire rate cooldown.
 */
void ADronesWeapon::SpecialUpgrade1(const float& DeltaTime)
{
	if (DroneFireRateTracker <= 0)
	{
		DroneFireRateTracker = FireRate;
		for (int i = 0; i < SpawnedDrones.Num(); i++)
		{
			if (bIsDroneAlive[i])
			{
				const FRotator DroneRotation = SpawnedDrones[i]->GetRelativeRotation() + FRotator(0.f, 10.f, 0.f);
				const FVector SpawnLocation = SpawnedDrones[i]->GetComponentLocation() + DroneRotation.Vector();
				const FTransform BulletSpawnLocation = FTransform(DroneRotation, SpawnLocation, FVector(1.f, 1.f, 1.f));
				if (Projectile)
				{
					AProjectile* ProjectileSpawn = GetWorld()->SpawnActorDeferred<AProjectile>(Projectile, BulletSpawnLocation);
					ProjectileSpawn->SetDamage(Damage);
					ProjectileSpawn->SetAffectRadius(AffectRadius / 4);
					ProjectileSpawn->SetSpecialUpgrade1(bSpecialUpgrade1);
					ProjectileSpawn->SetSpecialUpgrade2(bSpecialUpgrade2);
					ProjectileSpawn->SetSpecialUpgrade3(bSpecialUpgrade3);
					ProjectileSpawn->SetSpecialUpgrade4(bSpecialUpgrade4);
					ProjectileSpawn->SetSpecialUpgrade5(bSpecialUpgrade5);
					ProjectileSpawn->SetTriggerAmount(TriggerAmount);
					ProjectileSpawn->SetPlayerCharacter(PlayerCharacter);
					ProjectileSpawn->FinishSpawning(BulletSpawnLocation);

				}
			}
		}
	}
	else
	{
		DroneFireRateTracker -= DeltaTime;
	}
}

// Drones rotating lasers
/**
 * @brief Performs the second special upgrade for the drone weapon.
 * 
 * This function is responsible for implementing the second special upgrade for the drone weapon. It iterates over all spawned drones that are alive and performs a series of operations including line tracing and actor hit detection. The function also handles the laser sweep tracking if the sweep has started.
 * 
 * @param DeltaTime The time elapsed since the last frame, used for time-dependent calculations.
 */
void ADronesWeapon::SpecialUpgrade2(const float& DeltaTime)
{
	for (int j = 0; j < SpawnedDrones.Num(); j++)
	{
		if (bIsDroneAlive[j])
		{
			const FVector StartLocation = SpawnedDrones[j]->GetComponentLocation();
			const FRotator LaserRot = FRotator(0.f, 360 / TriggerAmount, 0.f);
			for (int i = 0; i < TriggerAmount; i++)
			{
				const FRotator NewLaserRot = FRotator(0.f, (LaserRot.Yaw * (i + 1)), 0.f) + LaserSweepTracker;

				if (LaserLengthTracker < AffectRadius * 2)
				{
					LaserLengthTracker += DeltaTime * (((1 - FireRate) * 300) + 300);
					const FVector EndLocation = StartLocation + NewLaserRot.Vector() * LaserLengthTracker;
					const TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
					ActorsHit.Append(ActiveActorsHit);
				}
				else
				{
					const FVector EndLocation = StartLocation + NewLaserRot.Vector() * LaserLengthTracker;
					const TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
					ActorsHit.Append(ActiveActorsHit);
					bStartSweep = true;
				}
			}
		}
	}
	if (bStartSweep)
	{
		LaserSweepTracker.Yaw += DeltaTime * (RotationSpeed * 2);
		LaserSweepTracker.Normalize();
	}
}

// Drones that explode
/**
 * @brief This function handles the special upgrade 4 for the drone weapon.
 * 
 * @param Index The index of the drone to be upgraded.
 * @param SpawnLocation The location where the drone is spawned.
 * 
 * The function sets the drone at the given index as not alive, hides it, and adjusts the damage modifier based on whether special upgrade 5 is active.
 * It then performs a sphere trace at the spawn location with a radius modified by the drone explosion modifier, and applies damage to the actors hit.
 */
void ADronesWeapon::SpecialUpgrade4(const int& Index, const FVector& SpawnLocation)
{
	bIsDroneAlive[Index] = false;
	SpawnedDrones[Index]->SetVisibility(false, true);
	(bSpecialUpgrade5) ? DamageModifier = DroneExplosionDamageModifier * DamageReduction : DamageModifier = DroneExplosionDamageModifier;
	const TArray<FHitResult> ActiveActorsHit = SphereTrace(SpawnLocation, SpawnLocation, AffectRadius * DroneExplosionModifier);
	ApplyDamage(ActiveActorsHit);
}

/**
 * @brief Resets the state of drones in the weapon system.
 * 
 * This method is responsible for resetting the state of drones in the weapon system. 
 * It sets up the drones, starts their expansion, resets the drone duration tracker, 
 * fire rate tracker, sweep tracker, and radial distance. It also destroys any spawned 
 * drones and deactivates any spawned jets.
 */
void ADronesWeapon::ResetDrones()
{
	bSetUpDrones = true;
	bIsExpanding = true;
	DroneDurationTracker = 0.f;
	FireRateTracker = FireRate;
	SweepTracker.Yaw = 0.f;
	RadialDistance = 0.f;
	for (UStaticMeshComponent* SpawnedDrone : SpawnedDrones)
	{
		if (SpawnedDrone)
		{
			SpawnedDrone->DestroyComponent();
		}
	}
	for (UNiagaraComponent* SpawnedJet : SpawnedJets)
	{
		if (SpawnedJet)
		{
			SpawnedJet->Deactivate();
		}
	}
}

/**
 * @brief Lerps the rotation of a drone mesh towards a target rotation.
 * 
 * This function linearly interpolates the rotation of a given drone mesh towards a target rotation over time. 
 * The speed of the interpolation is determined by a constant lerp speed and the passed delta time.
 *
 * @param MeshToLerp The mesh of the drone whose rotation is to be interpolated.
 * @param LerpRotation The target rotation to interpolate towards.
 * @param DeltaTime The time difference between the current frame and the previous frame.
 */
void ADronesWeapon::LerpDroneRotation(UStaticMeshComponent* MeshToLerp, const FRotator& LerpRotation, const float& DeltaTime)
{
	if(MeshToLerp)
	{
		constexpr float LerpSpeed = 5.0f;
		const FRotator LerpedRotation = FMath::Lerp(MeshToLerp->GetRelativeRotation(), LerpRotation, DeltaTime * LerpSpeed);
		MeshToLerp->SetRelativeRotation(LerpedRotation);
	}
}

/**
 * @brief Sets the location of a drone in the world.
 * 
 * This function updates the world location of a given drone (represented by a UStaticMeshComponent). 
 * If the provided mesh component is not null, its world location is set to the provided new location.
 *
 * @param MeshToLerp The mesh component representing the drone whose location is to be set.
 * @param NewLocation The new location in the world to which the drone should be moved.
 */
void ADronesWeapon::SetDroneLocation(UStaticMeshComponent* MeshToLerp, const FVector& NewLocation)
{
	if (MeshToLerp)
	{
		MeshToLerp->SetWorldLocation(NewLocation);
	}
}

/**
 * @brief Expands the drones in a radial pattern.
 *
 * This function is responsible for expanding the drones in a radial pattern around the actor. 
 * It sets the drone location based on the radial distance and the yaw rotation. 
 * It also adjusts the drone's rotation to face the actor.
 *
 * @param Index The index of the drone to be expanded.
 * @param DeltaTime The time since the last frame update.
 * @param YawModifier The modifier to be added to the yaw rotation of the drone.
 */
void ADronesWeapon::ExpandDrones(const int& Index, const float& DeltaTime, const float& YawModifier)
{
	
	const FRotator NewLaserRot = FRotator(0.f, (DroneRot.Yaw * (Index + 1)), 0.f) + SweepTracker;
	const FVector EndLocation = this->GetActorLocation() + NewLaserRot.Vector() * RadialDistance;
	SetDroneLocation(SpawnedDrones[Index], EndLocation);

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(SpawnedDrones[Index]->GetComponentLocation(), this->GetActorLocation());
	LookAtRotation.Yaw += YawModifier;
	LookAtRotation.Normalize();
	LerpDroneRotation(SpawnedDrones[Index], LookAtRotation, DeltaTime);
}

/**
 * @brief Rotates the drones forward.
 *
 * This function rotates each drone in the forward direction based on its index and the elapsed time.
 * The rotation is calculated based on the drone's yaw and the sweep tracker. The drone's location is then updated
 * and its rotation is interpolated towards the calculated rotation.
 *
 * @param Index The index of the drone to be rotated.
 * @param DeltaTime The time elapsed since the last frame.
 */
void ADronesWeapon::RotateDronesForward(const int& Index, const float& DeltaTime)
{
	const FRotator NewLaserRot = FRotator(0.f, (DroneRot.Yaw * (Index + 1)), 0.f) + SweepTracker;
	const FVector EndLocation = this->GetActorLocation() + NewLaserRot.Vector() * RadialDistance;
	SetDroneLocation(SpawnedDrones[Index], EndLocation);

	const FVector DroneLocation = SpawnedDrones[Index]->GetComponentLocation();
	const FVector VectorToDrone = DroneLocation - this->GetActorLocation();
	FVector TangentVector(-VectorToDrone.Y, VectorToDrone.X, 0.0f);
	TangentVector.Normalize();
	const FRotator Rotation = TangentVector.Rotation();
	LerpDroneRotation(SpawnedDrones[Index], Rotation, DeltaTime);
}

/**
 * @brief Performs a sphere trace in the game world and returns the actors hit.
 * 
 * This function performs a sphere trace from the actor's location in the direction of the drone's rotation. 
 * The radius of the sphere trace is determined by the `AffectRadius` member of the `ADronesWeapon` class.
 * The sphere trace checks for collisions with other actors in the game world.
 * 
 * @param Index The index of the drone performing the sphere trace. This is used to calculate the rotation of the sphere trace.
 * @return Returns an array of `FHitResult` objects. Each `FHitResult` contains information about an actor hit by the sphere trace.
 */
TArray<FHitResult> ADronesWeapon::CheckSphereTrace(const int& Index)
{
	const FRotator NewLaserRot = FRotator(0.f, (DroneRot.Yaw * (Index + 1)), 0.f) + SweepTracker;
	const FVector EndLocation = this->GetActorLocation() + NewLaserRot.Vector() * RadialDistance;
	const TArray<FHitResult> ActiveActorsHit = SphereTrace(EndLocation, EndLocation, AffectRadius);
	return ActiveActorsHit;
}

/**
 * @brief This function handles the damage taken by the drone weapon.
 * 
 * @param DamageAmount The amount of damage to be taken by the drone weapon.
 * @param DamageEvent The event that describes the damage.
 * @param EventInstigator The controller that was responsible for causing this damage.
 * @param DamageCauser The Actor that caused this damage.
 * 
 * @return Returns the amount of damage that was taken.
 * 
 * If the special upgrade 5 is active and the damage causer is an enemy, the player's health is updated based on the damage amount and the life steal percent.
 */
float ADronesWeapon::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bSpecialUpgrade5)
	{
		if (DamageCauser->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
		{
			if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				Player->UpdateHealth(DamageAmount * LifeStealPercent);
			}
		}
	}
	return DamageAmount;
}