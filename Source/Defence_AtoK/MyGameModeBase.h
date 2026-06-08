#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

// 게임 상태를 관리하기 위한 열거형
UENUM(BlueprintType)
enum class EDefensePhase : uint8
{
    Preparation UMETA(DisplayName = "Preparation Phase"),
    Wave        UMETA(DisplayName = "Wave Phase"),
    GameOver    UMETA(DisplayName = "Game Over")
};

USTRUCT(BlueprintType)
struct FWaveInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<class AEnemy>> EnemiesToSpawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalCount = 5;
};

UCLASS()
class DEFENCE_ATOK_API AMyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyGameMode();

protected:
    virtual void BeginPlay() override;

public:
    // 승리 시 함수
    UFUNCTION(BlueprintImplementableEvent, Category = "GameManager")
    void OnVictoryEvent();

    // 패배 시 함수
    UFUNCTION(BlueprintImplementableEvent, Category = "GameManager")
    void OnGameOverEvent();
    
    // 플레이어 초기 목숨
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameRules")
    int32 PlayerLifes = 10;

    // 목숨 감소 함수
    UFUNCTION(BlueprintCallable, Category = "GameRules")
    void DecreaseLife();

    // 플레이어의 현재 보유 재화
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
    int32 CurrentMoney = 100;

    // 재화 추가 함수
    UFUNCTION(BlueprintCallable, Category = "Economy")
    void AddMoney(int32 Amount);

    // 재화 소비 함수
    UFUNCTION(BlueprintCallable, Category = "Economy")
    bool SpendMoney(int32 Amount);

    // --- 페이즈 관리 함수 ---
    void StartPreparationPhase();

    UFUNCTION(BlueprintCallable, Category = "GameManager")
    void SkipPreparationPhase();

    void StartWavePhase();
    void EndWavePhase();

    //웨이브 준비 단계 시작을 알림
    UFUNCTION(BlueprintImplementableEvent, Category = "Wave")
    void OnWavePreparationStarted();

    // 맵에 배치된 스포너들을 수집할 배열
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<class AMonsterSpawner*> Spawners;

    // 이번 웨이브에 활성화되기로 결정된 스포너들 (준비 단계에서 미리 채워짐)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
    TArray<class AMonsterSpawner*> ActiveSpawnersForWave;

    // 웨이브별 적 스폰 정보 배열 (0번 인덱스 = 1웨이브)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<FWaveInfo> Waves;
    
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void CheckWaveEndCondition();

    // --- 설정 ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager")
    EDefensePhase CurrentPhase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameManager")
    int32 CurrentWave = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameManager")
    int32 MaxWave = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameManager")
    float PrepTimeLimit = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameManager")
    float WaveTimeLimit = 120.0f; // 웨이브 진행 제한 시간

    UFUNCTION(BlueprintPure, Category = "GameManager")
    float GetPhaseRemainingTime() const;

private:
    FTimerHandle PhaseTimerHandle;
};