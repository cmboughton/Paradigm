// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(SpawnCheckTracker <= 0)
	{
		SpawnCheckTracker = 20.f;
		float CurrentGameTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		if(CurrentGameTime <= 180)
		{
			
		}
		else if(CurrentGameTime > 180 && CurrentGameTime <= 360)
		{
			
		}
		else
		{
			
		}
	}
	else
	{
		SpawnCheckTracker -= DeltaTime;
	}

}

