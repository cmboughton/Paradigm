// Fill out your copyright notice in the Description page of Project Settings.


#include "DronesWeapon.h"

#include "Kismet/KismetMathLibrary.h"
#include "Paradigm_IQ/Core/Data/Interfaces/EnemyInterface.h"
#include "Paradigm_IQ/Core/Weapons/Projectile.h"

void ADronesWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

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
				if (DroneMeshRef)
				{
					DroneMesh->SetStaticMesh(DroneMeshRef);
					DroneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
		bSetUpDrones = false;
	}

	ApplyDamage(ActorsHit);
	ActorsHit.Empty();

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
		for (int i = 0; i < TriggerAmount; i++)
		{
			if (RadialDistance < (AffectRadius * 5))
			{
				ExpandDrones(true, i, DeltaTime, 180);
				const TArray<FHitResult> ActiveActorsHit = CheckSphereTrace(i);
				ActorsHit.Append(ActiveActorsHit);
			}
			else
			{
				RotateDronesForward(i, DeltaTime);
				const TArray<FHitResult> ActiveActorsHit = CheckSphereTrace(i);
				ActorsHit.Append(ActiveActorsHit);
			}
			//UE_LOGFMT(LogTemp, Warning, "LaserRot: {0}", FireRate);
		}
		
	}
	else if (bIsExpanding && bSpecialUpgrade4)
	{
		for (int i = 0; i < TriggerAmount; i++)
		{
			if (bIsDroneAlive.IsValidIndex(i))
			{
				if (bIsDroneAlive[i])
				{
					if (RadialDistance < (AffectRadius * 5))
					{
						ExpandDrones(true, i, DeltaTime, 180);
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
					else
					{
						RotateDronesForward(i, DeltaTime);
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
					//UE_LOGFMT(LogTemp, Warning, "LaserRot: {0}", FireRate);
				}
			}
		}
	}
	else
	{
		if (RadialDistance > 0)
		{
			for (int i = 0; i < TriggerAmount; i++)
			{
				if (!bSpecialUpgrade4)
				{
					ExpandDrones(false, i, DeltaTime, 0);
					const TArray<FHitResult> ActiveActorsHit = CheckSphereTrace(i);
					ActorsHit.Append(ActiveActorsHit);
				}
				else
				{
					if (bIsDroneAlive.IsValidIndex(i))
					{
						if (bIsDroneAlive[i])
						{
							ExpandDrones(false, i, DeltaTime, 0);
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
}

// Drones shooting Laser Projectile
void ADronesWeapon::SpecialUpgrade1(const float& DeltaTime)
{
	if (DroneFireRateTracker <= 0)
	{
		DroneFireRateTracker = FireRate;
		for (int i = 0; i < SpawnedDrones.Num(); i++)
		{
			if (bIsDroneAlive[i])
			{
				const FRotator DroneRotation = SpawnedDrones[i]->GetRelativeRotation() + FRotator(0.f, 25.f, 0.f);
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

void ADronesWeapon::SpecialUpgrade4(const int& Index, const FVector& SpawnLocation)
{
	bIsDroneAlive[Index] = false;
	SpawnedDrones[Index]->SetVisibility(false, true);

	const TArray<FHitResult> ActiveActorsHit = SphereTrace(SpawnLocation, SpawnLocation, AffectRadius * 3);
	ApplyDamage(ActiveActorsHit);
}

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

void ADronesWeapon::LerpDroneRotation(UStaticMeshComponent* MeshToLerp, const FRotator& LerpRotation, const float& DeltaTime)
{
	if(MeshToLerp)
	{
		constexpr float LerpSpeed = 5.0f;
		const FRotator LerpedRotation = FMath::Lerp(MeshToLerp->GetRelativeRotation(), LerpRotation, DeltaTime * LerpSpeed);
		MeshToLerp->SetRelativeRotation(LerpedRotation);
	}
}

void ADronesWeapon::SetDroneLocation(UStaticMeshComponent* MeshToLerp, const FVector& NewLocation)
{
	if (MeshToLerp)
	{
		MeshToLerp->SetWorldLocation(NewLocation);
	}
}

void ADronesWeapon::ExpandDrones(const bool& IsExpanding, const int& Index, const float& DeltaTime, const float& YawModifier)
{
	if(IsExpanding)
	{
		RadialDistance += (DeltaTime * 100);
	}
	else
	{
		RadialDistance -= (DeltaTime * 100);
	}
	const FRotator NewLaserRot = FRotator(0.f, (DroneRot.Yaw * (Index + 1)), 0.f) + SweepTracker;
	const FVector EndLocation = this->GetActorLocation() + NewLaserRot.Vector() * RadialDistance;
	SetDroneLocation(SpawnedDrones[Index], EndLocation);

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(SpawnedDrones[Index]->GetComponentLocation(), this->GetActorLocation());
	LookAtRotation.Yaw += YawModifier;
	LookAtRotation.Normalize();
	LerpDroneRotation(SpawnedDrones[Index], LookAtRotation, DeltaTime);
}

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

TArray<FHitResult> ADronesWeapon::CheckSphereTrace(const int& Index)
{
	const FRotator NewLaserRot = FRotator(0.f, (DroneRot.Yaw * (Index + 1)), 0.f) + SweepTracker;
	const FVector EndLocation = this->GetActorLocation() + NewLaserRot.Vector() * RadialDistance;
	const TArray<FHitResult> ActiveActorsHit = SphereTrace(EndLocation, EndLocation, AffectRadius);
	return ActiveActorsHit;
}
