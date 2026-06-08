#include "MonsterSpawner.h"
#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "MyEnemyAIController.h" 
#include "Engine/World.h"
#include "TimerManager.h"

AMonsterSpawner::AMonsterSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMonsterSpawner::BeginPlay()
{
    Super::BeginPlay();
}

void AMonsterSpawner::SpawnOneEnemy()
{
    AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameMode && GameMode->CurrentPhase != EDefensePhase::Wave)
    {
        return;
    }

    if (EnemyClassesToSpawn.Num() == 0) return;

    UWorld* World = GetWorld();
    if (!World) return;

    int32 Index = CurrentSpawnCount % EnemyClassesToSpawn.Num();
    TSubclassOf<AEnemy> EnemyClass = EnemyClassesToSpawn[Index];

    if (EnemyClass)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FTransform SpawnTransform = GetActorTransform();

        AEnemy* NewEnemy = World->SpawnActor<AEnemy>(EnemyClass, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator(), Params);

        if (NewEnemy)
        {

            NewEnemy->SpawnDefaultController();

            AMyEnemyAIController* NewAIC = Cast<AMyEnemyAIController>(NewEnemy->GetController());

            if (NewAIC)
            {
                NewAIC->Waypoints = this->Waypoints;

                if (NewAIC->Waypoints.Num() > 0 && NewAIC->Waypoints[0] != nullptr)
                {
                    NewAIC->MoveToActor(NewAIC->Waypoints[0]);
                }
            }

            CurrentSpawnCount++;
        }
    }
    if (CurrentSpawnCount >= TotalSpawnCount)
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
    }
}

    void AMonsterSpawner::StartSpawning(TArray<TSubclassOf<class AEnemy>> WaveEnemies, int32 WaveSpawnCount)
    {
        // 게임 모드에서 넘겨준 웨이브 데이터를 스포너의 변수에 덮어씌웁니다.
        EnemyClassesToSpawn = WaveEnemies;
        TotalSpawnCount = WaveSpawnCount;
        CurrentSpawnCount = 0; // 스폰 카운트 초기화

        // 적이 세팅되었으면 그때 타이머를 돌려 스폰을 시작합니다.
        if (EnemyClassesToSpawn.Num() > 0 && TotalSpawnCount > 0)
        {
            GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AMonsterSpawner::SpawnOneEnemy, SpawnInterval, true);
        }
    }