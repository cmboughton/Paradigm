// Fill out your copyright notice in the Description page of Project Settings.


#include "DronesWeapon.h"

void ADronesWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);

	ApplyDamage(ActorsHit);
	
	ActorsHit.Empty();
	FRotator DroneRot = FRotator(0.f, 360 / TriggerAmount, 0.f);
	for (int i = 0; i < TriggerAmount; i++)
	{
		FRotator NewLaserRot = FRotator(0.f, (DroneRot.Yaw * (i + 1)), 0.f) + SweepTracker;
		FVector StartLocation = this->GetActorLocation();
		if (RadialDistance < (AffectRadius * 5))
		{
			RadialDistance += DeltaTime * 100;
			FVector EndLocation = StartLocation + NewLaserRot.Vector() * RadialDistance;
			TArray<FHitResult> ActiveActorsHit = SphereTrace(EndLocation, EndLocation , AffectRadius);
			ActorsHit.Append(ActiveActorsHit);
		}
		else
		{
			FVector EndLocation = StartLocation + NewLaserRot.Vector() * RadialDistance;
			TArray<FHitResult> ActiveActorsHit = SphereTrace(EndLocation, EndLocation, AffectRadius);
			ActorsHit.Append(ActiveActorsHit);
		}
		//UE_LOGFMT(LogTemp, Warning, "LaserRot: {0}", FireRate);
	}


	if (bShouldSweep)
	{
		SweepRotation.Yaw = DroneRot.Yaw * 10;
		if (RadialDistance >= AffectRadius * 3)
		{
			if (DroneDurationTracker <= DroneDuration)
			{
				SweepTracker.Yaw += DeltaTime * RotationSpeed;
				DroneDurationTracker += DeltaTime;
				//UE_LOGFMT(LogTemp, Warning, "LaserRot: {0}", SweepTracker.Yaw);
			}
			else if (DroneDelayTracker >= .2f)
			{
				DroneDurationTracker = 0.f;
				FireRateTracker = FireRate;
				SweepTracker.Yaw = 0.f;
				RadialDistance = 0.f;
				DroneDelayTracker = 0.f;
				//UE_LOGFMT(LogTemp, Warning, "Fire Rate Tracker Set: {0}", FireRateTracker);
			}
			else
			{
				DroneDelayTracker += DeltaTime;
				//UE_LOGFMT(LogTemp, Warning, "Delay: {0}", LaserEndDelayTracker);
			}
		}
	}
}
