#include "MyGameModeBase.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "MonsterSpawner.h"

AMyGameMode::AMyGameMode()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();

    // 월드에 있는 스포너를 배열에 수집
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterSpawner::StaticClass(), FoundActors);

    Spawners.Empty(); // 혹시 모를 배열 초기화
    for (AActor* Actor : FoundActors)
    {
        AMonsterSpawner* Spawner = Cast<AMonsterSpawner>(Actor);
        if (Spawner)
        {
            Spawners.Add(Spawner);
        }
    }

    // 게임 시작 시 첫 번째 준비 단계 진입
    CurrentWave = 0;
    StartPreparationPhase();
}

//----웨이브 관리 함수----
void AMyGameMode::StartPreparationPhase()
{
    if (CurrentWave >= MaxWave)
    {
        // 최대 웨이브가 모두 끝났다면 게임 승리(클리어) 처리
        CurrentPhase = EDefensePhase::GameOver;
        OnVictoryEvent();
        return;
    }

    CurrentPhase = EDefensePhase::Preparation;

    // 웨이브 진입 시 숫자를 1 올림 (0 -> 1웨이브)
    CurrentWave++;

    ActiveSpawnersForWave.Empty();

    // 현재 웨이브 숫자만큼 스포너를 가동, 맵에 배치된 전체 스포너 개수를 넘지 않게 제한
    int32 SpawnerCountToPick = FMath::Min(CurrentWave, Spawners.Num());

    // 중복 선택을 막기 위해 전체 스포너 배열을 복사해서 임시 배열을 생성
    TArray<AMonsterSpawner*> TempSpawners = Spawners;
    for (int32 i = 0; i < SpawnerCountToPick; i++)
    {
        if (TempSpawners.Num() == 0) break;

        // 복사한 임시 배열에서 하나를 랜덤으로 뽑아 활성 배열에 넣고, 뽑은 건 삭제
        int32 RandomIndex = FMath::RandRange(0, TempSpawners.Num() - 1);
        ActiveSpawnersForWave.Add(TempSpawners[RandomIndex]);
        TempSpawners.RemoveAt(RandomIndex);
    }

    // 블루프린트로 웨이브가 시작되었다는 신호를 전송
    OnWavePreparationStarted();

    // 제한 시간(PrepTimeLimit)이 지나면 자동으로 웨이브 시작
    GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &AMyGameMode::StartWavePhase, PrepTimeLimit, false);
}

void AMyGameMode::SkipPreparationPhase()
{
    // 현재 준비 단계일 때만 스킵 가능
    if (CurrentPhase == EDefensePhase::Preparation)
    {
        // 타이머 취소 후 바로 웨이브 시작
        GetWorldTimerManager().ClearTimer(PhaseTimerHandle);
        StartWavePhase();
    }
}

void AMyGameMode::StartWavePhase()
{
    CurrentPhase = EDefensePhase::Wave;

    // 1. 인덱스에 웨이브 데이터가 정상적으로 있는지 확인
    if (Waves.IsValidIndex(CurrentWave - 1))
    {
        FWaveInfo CurrentWaveData = Waves[CurrentWave - 1];

        // 2. 준비 단계에서 미리 뽑은 스포너들에게 몬스터 스폰 명령 전달
        for (AMonsterSpawner* Spawner : ActiveSpawnersForWave)
        {
            if (Spawner)
            {
                Spawner->StartSpawning(CurrentWaveData.EnemiesToSpawn, CurrentWaveData.TotalCount);
            }
        }
    }
    else
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: Wave Data is missing!"));
    }

    // 웨이브 강제 종료 타이머
    GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &AMyGameMode::EndWavePhase, WaveTimeLimit, false);
}

void AMyGameMode::EndWavePhase()
{
    // 1. 타이머 제거 (적이 다 죽어서 조기 종료되었을 경우를 대비)
    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

    // 2. 필드에 존재하는 모든 AEnemy 액터를 찾아서 배열에 담음
    TArray<AActor*> FoundEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);

    // 3. 찾은 모든 적을 파괴 (일괄 삭제)
    for (AActor* EnemyActor : FoundEnemies)
    {
        if (IsValid(EnemyActor))
        {
            EnemyActor->Destroy();
        }
    }

    // 4. 다시 준비 단계로 넘어감
    StartPreparationPhase();
}

float AMyGameMode::GetPhaseRemainingTime() const
{
    // 타이머 매니저에서 PhaseTimerHandle의 남은 시간을 알아내어 반환합니다.
    return GetWorldTimerManager().GetTimerRemaining(PhaseTimerHandle);
}

//----체력 관리 함수----
void AMyGameMode::DecreaseLife()
{
    if (PlayerLifes > 0)
    {
        PlayerLifes--;

        // 체력이 0이 되면 게임 오버 처리
        if (PlayerLifes <= 0)
        {
            CurrentPhase = EDefensePhase::GameOver;
            GetWorldTimerManager().ClearTimer(PhaseTimerHandle); // 페이즈 진행 타이머 제거
            OnGameOverEvent();
        }
    }
}

//----재화 관리 함수----
void AMyGameMode::AddMoney(int32 Amount)
{
    CurrentMoney += Amount;
}

bool AMyGameMode::SpendMoney(int32 Amount)
{
    // 현재 보유 재화가 소비할 금액과 같거나 큰지(살 수 있는지) 확인
    if (CurrentMoney >= Amount)
    {
        CurrentMoney -= Amount;

        return true;
    }
    else
    {
        return false;
    }
}

void AMyGameMode::CheckWaveEndCondition()
{
    // 현재 웨이브 페이즈가 아니면 체크하지 않음
    if (CurrentPhase != EDefensePhase::Wave) return;

    // 필드에 존재하는 모든 AEnemy 액터를 검색
    TArray<AActor*> FoundEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);

    // 남아있는 적이 1개 이하인지 확인 
    if (FoundEnemies.Num() <= 1)
    {
        // 기존의 웨이브 제한 시간 타이머를 제거
        GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

        // 3초 뒤에 EndWavePhase가 실행되도록 타이머를 재설정
        GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &AMyGameMode::EndWavePhase, 3.0f, false);
    }
}