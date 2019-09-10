// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckDistanceAndModify.h"
#include "BasicEnemyAIController.h"
#include "BasicEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Engine.h"

EBTNodeResult::Type UBTTask_CheckDistanceAndModify::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	ABasicEnemyAIController* EnemyAIController = Cast<ABasicEnemyAIController>(OwnerComp.GetAIOwner());

	if (EnemyAIController) {

		ABasicEnemyCharacter* EnemyChar = Cast<ABasicEnemyCharacter>(EnemyAIController->GetCharacter());
		UBlackboardComponent* BBComp = EnemyAIController->GetBlackboardComp();
		ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		if (EnemyChar && BBComp && PlayerChar) {
			float currentDistance = EnemyChar->GetDistanceTo(PlayerChar);
			if (EnemyAIController->MaxFiringDistance < currentDistance) {
				FVector playerLocation = PlayerChar->GetActorLocation();
				FVector enemyLocation = EnemyChar->GetActorLocation();
				float desiredX = (EnemyAIController->MoveCloserDistance / currentDistance) * (enemyLocation.X - playerLocation.X) + playerLocation.X;
				float desiredY = (EnemyAIController->MoveCloserDistance / currentDistance) * (enemyLocation.Y - playerLocation.Y) + playerLocation.Y;

				FVector newLocation = FVector(desiredX, desiredY, PlayerChar->GetActorLocation().Z);

				EnemyAIController->SetMovementLocation(newLocation);

				ATargetPoint* tempPoint = EnemyAIController->GetMovementLocationPoint();
				BBComp->SetValueAsObject("LocationToGo", tempPoint);

				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
};