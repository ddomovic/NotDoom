#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicEnemyCharacter.generated.h"

UCLASS()
class NOTDOOM_API ABasicEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UFUNCTION()
	virtual void OnPlayerCaught(APawn *Pawn);
	UFUNCTION()
	virtual void OnPlayerHeard(APawn* PawnInstigator, const FVector& Location, float Volume);
	APawn *CurrentEnemy;

public:
	ABasicEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	void OnDeath();
	void Shooting();

	float CurrentTime;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	TArray<class AAIPatrolPoint*>& GetPatrolPoints();

	APawn* GetCurrentEnemy();
	void SetCurrentEnemy(APawn* NewEnemy);

	UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, Category = AI)
	class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere, Category = AI)
	uint32 EnemyID;

	UPROPERTY(EditAnywhere, Category = AI)
	float EnemyAlertRange;

	UPROPERTY(EditAnywhere, Category = AI)
	float EnemyBuddiesRange;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<class AAIPatrolPoint*> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float CurrentHealth;

	bool bIsFighting;

	UPROPERTY(EditAnywhere, Category = "Weapon stats")
	float AccuracyOffsetPitch;

	UPROPERTY(EditAnywhere, Category = "Weapon stats")
	float AccuracyOffsetYaw;

	UPROPERTY(EditAnywhere, Category = "Weapon stats")
	float FireRate;
	float ShootTime;

	UFUNCTION(BlueprintCallable, Category = "AI Fighting")
	bool GetIsFighting();

	UFUNCTION(BlueprintCallable, Category = "AI Fighting")
	void SetIsFighting(bool IsFighting);

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ABasicEnemyProjectile> ProjectileClass;

	void UpdateHealth(float Value);

	UPROPERTY(EditAnywhere, Category = "Weapon Sound")
	class USoundCue* SoundToPlay;

	UPROPERTY(EditAnywhere, Category = "Weapon Sound")
	float WeaponLoudness;
};
