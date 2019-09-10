// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AlertBuddiesInRange.h"
#include "BasicEnemyAIController.h"
#include "BasicEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/Core/Public/Containers/Array.h"

EBTNodeResult::Type UBTTask_AlertBuddiesInRange::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) {

	ABasicEnemyAIController *EnemyAIController = Cast<ABasicEnemyAIController>(OwnerComp.GetAIOwner());
	ABasicEnemyCharacter *EnemyAI = Cast<ABasicEnemyCharacter>(EnemyAIController->GetPawn());

	if (EnemyAIController && EnemyAI) {

		UBlackboardComponent *BBComp = EnemyAIController->GetBlackboardComp();
		TArray<ABasicEnemyCharacter*> Buddies = EnemyAIController->GetBuddies();
		for (int i = 0; i < Buddies.Num(); i++) {
			if (Buddies[i]->GetController() == nullptr) {
				continue;
			}
			if (EnemyAI->GetDistanceTo(Buddies[i]) <= EnemyAI->EnemyAlertRange) {
				UBlackboardComponent *tempBBComp = Cast<ABasicEnemyAIController>(Buddies[i]->GetController())->GetBlackboardComp();
				tempBBComp->SetValueAsObject("Target", BBComp->GetValueAsObject("Target"));
				tempBBComp->SetValueAsBool("AlertedOthers", true);
				BBComp->SetValueAsBool("AlertedOthers", true);
			}
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
