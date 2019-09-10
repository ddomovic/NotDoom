// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EndFiring.h"
#include "BasicEnemyAIController.h"
#include "BasicEnemyCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

EBTNodeResult::Type UBTTask_EndFiring::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	ABasicEnemyAIController* EnemyAIController = Cast<ABasicEnemyAIController>(OwnerComp.GetAIOwner());

	if (EnemyAIController) {

		ABasicEnemyCharacter* EnemyChar = Cast<ABasicEnemyCharacter>(EnemyAIController->GetCharacter());

		if (EnemyChar) {
			EnemyChar->bIsFighting = false;
			FRotator tempRot = EnemyChar->GetActorRotation();
			EnemyChar->SetActorRotation(FRotator(0.f, tempRot.Yaw, tempRot.Roll));
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
};