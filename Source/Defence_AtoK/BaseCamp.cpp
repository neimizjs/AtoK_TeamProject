#include "BaseCamp.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"
#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ABaseCamp::ABaseCamp()
{
    PrimaryActorTick.bCanEverTick = false;

    // 1. 충돌 박스 생성 및 설정
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    CollisionBox->SetBoxExtent(FVector(100.f, 100.f, 100.f)); // 박스 크기
    CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // 모든 동적 물체와 겹침 허용

    // 2. 외형 메시 생성 및 충돌 박스에 부착
    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    BaseMesh->SetupAttachment(RootComponent);

    // 3. 충돌 이벤트(함수) 연결
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCamp::OnOverlapBegin);
}

void ABaseCamp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 나랑 부딪힌 물체(OtherActor)가 AEnemy(적)인지 캐스팅해서 확인
    AEnemy* Enemy = Cast<AEnemy>(OtherActor);
    if (Enemy)
    {
        // 적 파괴 
        Enemy->HandleDestruction();

        // 현재 게임의 심판(GameMode)을 불러와서 목숨 깎기 함수 실행
        AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        if (GameMode)
        {
            GameMode->DecreaseLife();
        }
    }
}