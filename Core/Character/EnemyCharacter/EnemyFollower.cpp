// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFollower.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

void AEnemyFollower::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetCharacterState() != ECharacterState::Death)
		{
			const FVector TargetLocation = PlayerCharacter->GetActorLocation();
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this->GetController(), TargetLocation);
			if (DistanceCheck(AttackRange))
			{
				ApplyDamage(PlayerCharacter);
				EnemyDeath(false, EDeathType::Normal);
			}
		}
	}
	else
	{
		PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}
