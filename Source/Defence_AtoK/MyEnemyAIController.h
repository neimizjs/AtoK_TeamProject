#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyEnemyAIController.generated.h"

UCLASS()
class DEFENCE_ATOK_API AMyEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    // BP에서 복사해온 데이터를 담을 배열
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TArray<AActor*> Waypoints;

protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
    int32 CurrentWaypointIndex = 0;
    void MoveToNextWaypoint();
};