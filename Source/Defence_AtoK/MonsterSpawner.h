#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "MonsterSpawner.generated.h"

UCLASS()
class DEFENCE_ATOK_API AMonsterSpawner : public AActor
{
    GENERATED_BODY()

public:
    AMonsterSpawner();

protected:
    virtual void BeginPlay() override;
    void SpawnOneEnemy();

    FTimerHandle SpawnTimerHandle;
    int32 CurrentSpawnCount = 0;

public:

    void StartSpawning(TArray<TSubclassOf<class AEnemy>> WaveEnemies, int32 WaveSpawnCount);

    UPROPERTY(EditAnywhere, Category = "Spawner")
    TArray<TSubclassOf<class AEnemy>> EnemyClassesToSpawn;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    TArray<AActor*> Waypoints;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    int32 TotalSpawnCount = 5;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnInterval = 2.0f;
};