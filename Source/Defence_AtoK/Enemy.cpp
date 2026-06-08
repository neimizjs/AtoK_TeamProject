#include "Enemy.h"
#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
    
	CurrentHP = MaxHP;
}

// 데미지를 받았을 때
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 이미 죽었으면 무시
    if (CurrentHP <= 0.f) return 0.f;
    
    CurrentHP -= DamageAmount;
    if (CurrentHP < 0.f) CurrentHP = 0.f;

    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 체력이 0 이하면 사망 처리
	if (CurrentHP <= 0.f)
	{
		Die();
	}

	return ActualDamage;
}

// 사망 시 실행
void AEnemy::Die()
{
	// 중복 사망 방지
	CurrentHP = 0.f;

	if (GetController())
	{
		GetController()->StopMovement();
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->AddMoney(DropMoney);
		GameMode->CheckWaveEndCondition();
	}

	Destroy(); 
}
MissileTarget = CreateDefaultSubobject<USceneComponent>(TEXT("MissileTarget"));
MissileTarget->SetupAttachment(RootComponent);
MissileTarget->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
void AEnemy::HandleDestruction()
{
	AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->CheckWaveEndCondition();
	}
	
	Destroy();
}