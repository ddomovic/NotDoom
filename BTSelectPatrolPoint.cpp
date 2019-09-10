// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSelectPatrolPoint.h"
#include "AIPatrolPoint.h"
#include "BasicEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "Engine.h"

EBTNodeResult::Type UBTSelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	ABasicEnemyAIController* EnemyAIController = Cast<ABasicEnemyAIController>(OwnerComp.GetAIOwner());

	if (EnemyAIController) {
		UBlackboardComponent* BBComp = EnemyAIController->GetBlackboardComp();
		AAIPatrolPoint* CurrentPatrolPoint = Cast<AAIPatrolPoint>(BBComp->GetValueAsObject("LocationToGo"));
		TArray<AAIPatrolPoint*> AllPatrolPoints = EnemyAIController->GetPatrolPoints();
		AAIPatrolPoint* NextPatrolPoint = nullptr;

		int32 NumberOfPoints = AllPatrolPoints.Num();
		NextPatrolPoint = AllPatrolPoints[EnemyAIController->GetCurrentPatrolPoint()];
		BBComp->SetValueAsObject("LocationToGo", NextPatrolPoint);
		EnemyAIController->SetCurrentPatrolPoint((EnemyAIController->GetCurrentPatrolPoint() + 1) % NumberOfPoints);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
