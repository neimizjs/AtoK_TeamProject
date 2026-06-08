#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

AProjectile::AProjectile()
{
    // 1. 충돌체 설정
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);

    CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);

    RootComponent = CollisionComp;

    RootComponent = CollisionComp;

    // 2. 외형 설정
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);

    // 3. 발사체 이동 설정
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f; // 초기 속도
    ProjectileMovement->MaxSpeed = 3000.f;     // 최대 속도
    ProjectileMovement->bRotationFollowsVelocity = true; // 날아가는 방향으로 회전
    ProjectileMovement->bShouldBounce = false;  // 튕기지 않음

    // 5초 뒤 자동 파괴 (허공으로 날아갔을 때 메모리 방지)
    InitialLifeSpan = 5.0f;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
    {
        // 상대방에게 데미지 전달
        UGameplayStatics::ApplyDamage(
            OtherActor,          // 피해자
            Damage,              // 데미지 양
            GetInstigatorController(), // 공격자 컨트롤러
            this,                // 공격 원인 액터
            UDamageType::StaticClass()
        );

        UE_LOG(LogTemp, Log, TEXT("Overlap Hit: %s"), *OtherActor->GetName());

        Destroy();
    }
}