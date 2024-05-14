// Fill out your copyright notice in the Description page of Project Settings.


#include "DronesWeapon.h"

#include "Kismet/KismetMathLibrary.h"
#include "Paradigm_IQ/Core/Weapons/Projectile.h"


void ADronesWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	if(bSetUpDrones)
	{
		SpawnedDrones.Empty();
		for(int i = 0; i < TriggerAmount; i++)
		{
			FName DroneName = FName(*FString("DroneMesh").Append(FString::SanitizeFloat(i)));
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
		int JetNameTracker = 0;
		for (UStaticMeshComponent* SpawnedDrone : SpawnedDrones)
		{
			int counter = 0;
			TArray<FName>DroneSocketNames = SpawnedDrone->GetAllSocketNames();
			for (const FName DroneSocketName : DroneSocketNames)
			{
				counter++;
				FName SocketName = FName(*FString("JetSocket").Append(FString::FromInt(counter)));
				//UE_LOGFMT(LogTemp, Warning, "Socket: {0}", SocketName);
				if (SpawnedDrone->DoesSocketExist(SocketName))
				{
					if (UNiagaraComponent* NiagaraComponent = NewObject<UNiagaraComponent>(this, UNiagaraComponent::StaticClass(), FName(*FString("Jet").Append(FString::FromInt(JetNameTracker)))))
					{
						NiagaraComponent->RegisterComponent();
						NiagaraComponent->CreationMethod = EComponentCreationMethod::Instance;
						//UE_LOGFMT(LogTemp, Warning, "NiagraSpawned: {0}", FName(*FString("Jet") + SocketName.ToString()));
						if(NiagaraComponent)
						{
							NiagaraComponent->SetAsset(JetNiagara);
							NiagaraComponent->AttachToComponent(SpawnedDrone, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
							NiagaraComponent->Activate();
							SpawnedJets.Add(NiagaraComponent);
						}
					}
				}
				JetNameTracker++;
			}
		}
		bSetUpDrones = false;
	}

	ApplyDamage(ActorsHit);
	
	ActorsHit.Empty();
	FRotator DroneRot = FRotator(0.f, 360 / TriggerAmount, 0.f);
	if(bIsExpanding)
	{
		for (int i = 0; i < TriggerAmount; i++)
		{
			FRotator NewLaserRot = FRotator(0.f, (DroneRot.Yaw * (i + 1)), 0.f) + SweepTracker;
			FVector StartLocation = this->GetActorLocation();
			if (RadialDistance < (AffectRadius * 5))
			{
				RadialDistance += DeltaTime * 100;
				FVector EndLocation = StartLocation + NewLaserRot.Vector() * RadialDistance;
				TArray<FHitResult> ActiveActorsHit = SphereTrace(EndLocation, EndLocation , AffectRadius);
				if(SpawnedDrones.IsValidIndex(i))
				{
					SpawnedDrones[i]->SetWorldLocation(EndLocation);
				}
				ActorsHit.Append(ActiveActorsHit);
				for (UStaticMeshComponent* SpawnedDrone : SpawnedDrones)
				{
					FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(SpawnedDrone->GetComponentLocation(), this->GetActorLocation());
					LookAtRotation.Yaw += 180.f;
					LookAtRotation.Normalize();

					constexpr float LerpSpeed = 1.0f;
					const FRotator LerpedRotation = FMath::Lerp(SpawnedDrone->GetRelativeRotation(), LookAtRotation, DeltaTime * LerpSpeed);
					SpawnedDrone->SetRelativeRotation(LerpedRotation);
				}
			}
			else
			{
				FVector EndLocation = StartLocation + NewLaserRot.Vector() * RadialDistance;
				TArray<FHitResult> ActiveActorsHit = SphereTrace(EndLocation, EndLocation, AffectRadius);
				if (SpawnedDrones.IsValidIndex(i))
				{
					SpawnedDrones[i]->SetWorldLocation(EndLocation);
				}
				ActorsHit.Append(ActiveActorsHit);
				for (UStaticMeshComponent* SpawnedDrone : SpawnedDrones)
				{
					FVector DroneLocation = SpawnedDrone->GetComponentLocation();
					FVector VectorToDrone = DroneLocation - this->GetActorLocation();
					FVector TangentVector(-VectorToDrone.Y, VectorToDrone.X, 0.0f);
					TangentVector.Normalize();
					FRotator Rotation = TangentVector.Rotation();
	
					constexpr float LerpSpeed = 1.0f;
					const FRotator LerpedRotation = FMath::Lerp(SpawnedDrone->GetRelativeRotation(), Rotation, DeltaTime * LerpSpeed);
					SpawnedDrone->SetRelativeRotation(LerpedRotation);
				}
			}
			//UE_LOGFMT(LogTemp, Warning, "LaserRot: {0}", FireRate);
		}
		
	}
	else
	{
		if (RadialDistance > 0)
		{
			for (int i = 0; i < TriggerAmount; i++)
			{
				FRotator NewLaserRot = FRotator(0.f, (DroneRot.Yaw * (i + 1)), 0.f) + SweepTracker;
				FVector StartLocation = this->GetActorLocation();
				RadialDistance -= DeltaTime * 100;
				FVector EndLocation = StartLocation + NewLaserRot.Vector() * RadialDistance;
				TArray<FHitResult> ActiveActorsHit = SphereTrace(EndLocation, EndLocation, AffectRadius);
				if (SpawnedDrones.IsValidIndex(i))
				{
					SpawnedDrones[i]->SetWorldLocation(EndLocation);
				}
				ActorsHit.Append(ActiveActorsHit);
				for (UStaticMeshComponent* SpawnedDrone : SpawnedDrones)
				{
					FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(SpawnedDrone->GetComponentLocation(), this->GetActorLocation());
					LookAtRotation.Normalize();

					constexpr float LerpSpeed = 1.0f;
					const FRotator LerpedRotation = FMath::Lerp(SpawnedDrone->GetRelativeRotation(), LookAtRotation, DeltaTime * LerpSpeed);
					SpawnedDrone->SetRelativeRotation(LerpedRotation);
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
		for (const auto DroneMesh : SpawnedDrones)
		{
			const FRotator DroneRotation = DroneMesh->GetRelativeRotation() + FRotator(0.f, 25.f, 0.f);
			const FVector SpawnLocation = DroneMesh->GetComponentLocation() + DroneRotation.Vector();
			const FTransform BulletSpawnLocation = FTransform(DroneRotation, SpawnLocation, FVector(1.f, 1.f, 1.f));
			if (Projectile)
			{
				AProjectile* ProjectileSpawn = GetWorld()->SpawnActorDeferred<AProjectile>(Projectile, BulletSpawnLocation);
				ProjectileSpawn->SetDamage(Damage);
				ProjectileSpawn->SetAffectRadius(AffectRadius / 4);
				ProjectileSpawn->SetSpecialUpgrade1(bSpecialUpgrade1);
				ProjectileSpawn->SetSpecialUpgrade2(bSpecialUpgrade2);
				ProjectileSpawn->SetSpecialUpgrade3(bSpecialUpgrade3);
				ProjectileSpawn->SetTriggerAmount(TriggerAmount);
				ProjectileSpawn->SetPlayerCharacter(PlayerCharacter);
				ProjectileSpawn->FinishSpawning(BulletSpawnLocation);

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
	for (const auto DroneMesh : SpawnedDrones)
	{
		const FVector StartLocation = DroneMesh->GetComponentLocation();
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
		if (bStartSweep)
		{
			LaserSweepTracker.Yaw += DeltaTime * (RotationSpeed / 2);
			LaserSweepTracker.Normalize();
		}
	}
}