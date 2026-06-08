#include "MyEnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Engine/Engine.h"
#include "UObject/UnrealType.h"
#include "TimerManager.h"

void AMyEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!InPawn) return;

    // 1. 블루프린트에 선언된 'Waypoints' 변수를 리플렉션으로 탐색
    FArrayProperty* ArrayProp = FindFProperty<FArrayProperty>(InPawn->GetClass(), TEXT("Waypoints"));

    if (ArrayProp)
    {
        // 2. 실제 데이터 주소를 TArray 포인터로 캐스팅하여 안전하게 접근
        TArray<AActor*>* PawnWaypoints = ArrayProp->ContainerPtrToValuePtr<TArray<AActor*>>(InPawn);

        if (PawnWaypoints && PawnWaypoints->Num() > 0)
        {
            // 데이터 복사 및 인덱스 초기화
            Waypoints = *PawnWaypoints;
            CurrentWaypointIndex = 0;

            // 3. 내비게이션 시스템이 완전히 준비될 수 있도록 다음 프레임에 이동 시작
            GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AMyEnemyAIController::MoveToNextWaypoint);
        }
    }
}

void AMyEnemyAIController::MoveToNextWaypoint()
{
    // 배열 범위 확인 및 유효성 검사
    if (Waypoints.IsValidIndex(CurrentWaypointIndex))
    {
        AActor* TargetWaypoint = Waypoints[CurrentWaypointIndex];

        if (TargetWaypoint)
        {
            // 수용 반경을 50.f 정도로 주어 자연스러운 도착 유도
            MoveToActor(TargetWaypoint, 50.f);
        }
    }
}

void AMyEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    // 이동이 성공적으로 끝났을 때만 다음 목적지로 갱신
    if (Result.IsSuccess())
    {
        CurrentWaypointIndex++;
        MoveToNextWaypoint();
    }
    else if (Result.IsInterrupted())
    {
        // 장애물 등에 의해 중단되었을 경우의 처리
    }
}