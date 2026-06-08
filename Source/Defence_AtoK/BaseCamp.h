#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCamp.generated.h"

class UBoxComponent;

UCLASS()
class DEFENCE_ATOK_API ABaseCamp : public AActor
{
    GENERATED_BODY()

public:
    ABaseCamp();

protected:
    // 충돌(오버랩)이 발생했을 때 실행될 함수 선언
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    // 적이 닿았는지 판정할 투명한 박스
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* CollisionBox;

    // 화면에 보여질 집의 외형(메시)
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* BaseMesh;
};