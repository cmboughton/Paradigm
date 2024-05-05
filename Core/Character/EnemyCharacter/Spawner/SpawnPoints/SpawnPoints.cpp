// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoints.h"
#include "Components/BoxComponent.h"

ASpawnPoints::ASpawnPoints()
{
	EdgeSpawnerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawner Zone"));
	EdgeSpawnerZone->SetupAttachment(RootComponent);
}
