// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EnemyFollower.generated.h"

/**
 * 
 */
UCLASS()
class PARADIGM_IQ_API AEnemyFollower : public AEnemyCharacter
{
	GENERATED_BODY()

protected:

	virtual void Tick(const float DeltaTime) override;
	
};
