#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Enemy.generated.h"

UCLASS()
class DEFENCE_ATOK_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float MaxHP = 100.f;

	// 현재 체력
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float CurrentHP;

	//사망시 얻는 돈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int DropMoney = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* MissileTarget;
public:
	/** 
	 * 언리얼 내장 데미지 수신 함수 
	 * @param DamageAmount 전달받은 데미지 양
	 * @param DamageEvent 데미지 종류 및 세부 정보
	 * @param EventInstigator 공격을 명령한 컨트롤러 (AI/Player)
	 * @param DamageCauser 데미지를 준 원인 액터 (총알/타워 등)
	 */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 사망 처리 함수
	void Die();
	
	// 적이 게임에서 제거될 때 호출할 함수
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void HandleDestruction();
};