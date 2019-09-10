#include "BTTaskFireWeapon.h"
#include "BasicEnemyAIController.h"
#include "BasicEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"

EBTNodeResult::Type UBTTaskFireWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	ABasicEnemyAIController* EnemyAIController = Cast<ABasicEnemyAIController>(OwnerComp.GetAIOwner());

	if (EnemyAIController) {
		ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		ABasicEnemyCharacter* EnemyChar = Cast<ABasicEnemyCharacter>(EnemyAIController->GetCharacter());

		if (EnemyChar && PlayerChar) {
			FRotator AngleToRotate = UKismetMathLibrary::FindLookAtRotation(EnemyChar->GetActorLocation(), PlayerChar->GetActorLocation());
			EnemyChar->SetActorRotation(AngleToRotate);
			EnemyChar->bIsFighting = true;

			return EBTNodeResult::Succeeded;

		}
	}

	return EBTNodeResult::Failed;
}
