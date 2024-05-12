// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserWeapon.h"

void ALaserWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	if(!ActorsHit.IsEmpty())
	{
		ApplyDamage(ActorsHit);
		ActorsHit.Empty(); 
	}
	if(!bSpecialUpgrade1)
	{
		const FVector StartLocation = this->GetActorLocation();
		const FRotator LaserRot = FRotator(0.f, 360 / TriggerAmount, 0.f);
		if(SweepTracker.Yaw <= LaserRot.Yaw)
		{
			for(int i = 0; i < TriggerAmount; i++)
			{
				const FRotator NewLaserRot = FRotator(0.f, (LaserRot.Yaw * (i + 1)) + 45, 0.f) + SweepTracker;
				
				if(LaserLengthTracker < AffectRadius)
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
		if(bStartSweep)
		{
			if(SweepTracker.Yaw <= LaserRot.Yaw)
			{
				SweepTracker.Yaw += DeltaTime * (((1 - FireRate) * 50) + 50);
			}
			else
			{
				if(LaserLengthTracker <= 0)
				{
					FireRateTracker = FireRate;
					SweepTracker.Yaw = 0.f;
					LaserLengthTracker = 0.f;
					bStartSweep = false;
				}
				else
				{
					for (int i = 0; i < TriggerAmount; i++)
					{
						LaserLengthTracker -= DeltaTime * (((1 - FireRate) * 300) + 300);
						const FRotator NewLaserRot = FRotator(0.f, (LaserRot.Yaw * (i + 1)) + 45, 0.f) + SweepTracker;
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
		const FVector StartLocation = this->GetActorLocation();
		const FRotator LaserRot = FRotator(0.f, 360 / TriggerAmount, 0.f);
		for (int i = 0; i < TriggerAmount; i++)
		{
			const FRotator NewLaserRot = FRotator(0.f, (LaserRot.Yaw * (i + 1)) + 45, 0.f) + SweepTracker;
			const FVector EndLocation = StartLocation + NewLaserRot.Vector() * AffectRadius;
			const TArray<FHitResult> ActiveActorsHit = LineTrace(StartLocation, EndLocation);
			ActorsHit.Append(ActiveActorsHit);
		}

		SweepTracker.Yaw += DeltaTime * (((1 - FireRate) * 50) + 50);
		SweepTracker.Normalize();
		
	}
}