// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFollower.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Paradigm_IQ/Core/Character/PlayerCharacter/PlayerCharacter.h"
#include "Engine/DamageEvents.h"

void AEnemyFollower::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetCharacterState() != ECharacterState::Death)
		{
			const FVector TargetLocation = PlayerCharacter->GetActorLocation();
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this->GetController(), TargetLocation);
			if (FVector::DistSquared(this->GetActorLocation(), PlayerCharacter->GetActorLocation()) <= AttackRange * AttackRange)
			{
				FHitResult EmptyResult;
				const FPointDamageEvent DamageEvent(Damage, EmptyResult, this->GetActorLocation(), nullptr);
				PlayerCharacter->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
				bHasAttacked = true;
				Death();
			}
		}
	}
	else
	{
		PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}
