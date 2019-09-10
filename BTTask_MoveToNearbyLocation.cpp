// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToNearbyLocation.h"
#include "BasicEnemyAIController.h"
#include "BasicEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Engine.h"

EBTNodeResult::Type UBTTask_MoveToNearbyLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	ABasicEnemyAIController* EnemyAIController = Cast<ABasicEnemyAIController>(OwnerComp.GetAIOwner());

	if (EnemyAIController) {

		ABasicEnemyCharacter* EnemyChar = Cast<ABasicEnemyCharacter>(EnemyAIController->GetCharacter());
		UBlackboardComponent* BBComp = EnemyAIController->GetBlackboardComp();

		if (EnemyChar && BBComp) {
			
			float moveDistance = FMath::RandRange(0.f, EnemyAIController->MaxMoveDistance);
			float moveX = FMath::RandRange(-moveDistance, moveDistance);
			float moveY = FMath::Sqrt(moveDistance * moveDistance - moveX * moveX);
			if ((FMath::Rand() % 2) == 0) {
				moveY *= -1;
			}

			FVector currentVec = EnemyChar->GetActorLocation();

			EnemyAIController->SetMovementLocationXY(currentVec.X + moveX, currentVec.Y + moveY);
		
			ATargetPoint* tempPoint = EnemyAIController->GetMovementLocationPoint();
			BBComp->SetValueAsObject("LocationToGo", tempPoint);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
};