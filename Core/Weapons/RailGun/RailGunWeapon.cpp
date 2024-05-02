// Fill out your copyright notice in the Description page of Project Settings.


#include "RailGunWeapon.h"

#include "Kismet/GameplayStatics.h"

void ARailGunWeapon::WeaponTriggered(const float DeltaTime)
{
	Super::WeaponTriggered(DeltaTime);
	int SpawnLocationTracker = 0;
	if (BulletTracker < TriggerAmount)
	{
		BulletTracker++;
		BulletDelayTracker = (FireRate / TriggerAmount) * 0.5;
		FRotator PlayerRot = PlayerCharacterRef->GetBaseModel()->GetRelativeRotation();
		FVector SpawnLocation = GetActorLocation() + PlayerRot.Vector();
		const FTransform BulletSpawnLocation = FTransform(PlayerRot, SpawnLocation, FVector(1.f, 1.f, 1.f));
		SpawnProjectile(BulletSpawnLocation);
	}
	else
	{
		FireRateTracker = FireRate;
		BulletTracker = 0;
	}
}

void ARailGunWeapon::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacterRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

							/* This is another way to spawn projectiles that are in a fan visual */	

		//SpawnLocationTracker = (FMath::Pow(-1.f, BulletTracker + 1) * (AffectRadius * 2) * ((BulletTracker + 2) / 2));
		//FVector AdjustedTranslation = FVector(SpawnTranslation.X, SpawnTranslation.Y + SpawnLocationTracker, SpawnTranslation.Z);
		//float AngleIncrement = 360.f / TriggerAmount;

		//SpawnLocationTracker = (FMath::Pow(-1.f, BulletTracker) * (AffectRadius * 1.5) * ((BulletTracker + 1) / 2));

		//FVector ForwardVector = PlayerCharacterRef->GetBaseModel()->GetRelativeRotation().Vector();
		////FVector RightVector = FVector::CrossProduct(ForwardVector, FVector::UpVector);
		//FVector RightVector = FVector::CrossProduct(FVector::UpVector, ForwardVector).GetSafeNormal();
		//FVector SpawnTranslation = this->GetActorLocation() + ForwardVector;
		//FVector SpawnLocation = SpawnTranslation + RightVector * SpawnLocationTracker;
		//UE_LOGFMT(LogTemp, Warning, "Right Vector: {0} {1} {2}", ForwardVector.X, ForwardVector.Y, ForwardVector.Z);
		////FVector SpawnLocation = SpawnTranslation + RightVector.RotateAngleAxis(AngleIncrement * SpawnLocationTracker, FVector::UpVector) * 40;

		//const FTransform BulletSpawnLocation = FTransform(FRotator(0, PlayerCharacterRef->GetBaseModel()->GetRelativeRotation().Yaw, 0), SpawnLocation, FVector(1.f, 1.f, 1.f));
		//SpawnProjectile(BulletSpawnLocation);