#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BasicEnemyAIController.generated.h"

UCLASS()
class NOTDOOM_API ABasicEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	class UBehaviorTreeComponent* BehaviorTreeComp;
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName LocationToGoKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName TargetKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName AlertedOthersKey;

	TArray<class AAIPatrolPoint*> PatrolPoints;
	int32 CurrentPatrolPoint = 0;

	TArray<class ABasicEnemyCharacter*> BuddyEnemies;

	class ATargetPoint* MovementLocationPoint;

public:
	ABasicEnemyAIController();

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float MaxMoveDistance;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float MaxFiringDistance;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float MoveCloserDistance;

	class UBlackboardComponent* GetBlackboardComp() const;
	TArray<class AAIPatrolPoint*>& GetPatrolPoints();
	int32 GetCurrentPatrolPoint();
	void SetCurrentPatrolPoint(int32 NewPatrolPoint);
	void SetPlayerCaught(APawn* Pawn, bool IsCaught);
	virtual void OnPossess(APawn* Pawn) override;

	class ATargetPoint* GetMovementLocationPoint();
	void SetMovementLocation(FVector& NewLocation);
	void SetMovementLocationXY(float X, float Y);
	TArray<class ABasicEnemyCharacter*>& GetBuddies();
};
