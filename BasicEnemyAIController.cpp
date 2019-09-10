// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicEnemyAIController.h"
#include "AIPatrolPoint.h"
#include "BasicEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "Runtime/Core/Public/Containers/Array.h"

ABasicEnemyAIController::ABasicEnemyAIController() {
	this->BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	this->BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	this->LocationToGoKey = "LocationToGo";
	this->TargetKey = "Target";
	this->AlertedOthersKey = "AlertedOthers";
	this->CurrentPatrolPoint = 0;

	this->MaxMoveDistance = 300.f;
	this->MaxFiringDistance = 2500.f;
	this->MoveCloserDistance = 1500.f;
}

UBlackboardComponent* ABasicEnemyAIController::GetBlackboardComp() const {
	return this->BlackboardComp;
}

TArray<AAIPatrolPoint*>& ABasicEnemyAIController::GetPatrolPoints() {
	return this->PatrolPoints;
}

int32 ABasicEnemyAIController::GetCurrentPatrolPoint() {
	return this->CurrentPatrolPoint;
}

void ABasicEnemyAIController::SetCurrentPatrolPoint(int32 NewPatrolPoint) {
	this->CurrentPatrolPoint = NewPatrolPoint;
}

void ABasicEnemyAIController::OnPossess(APawn* Pawn) {
	
	Super::OnPossess(Pawn);
	ABasicEnemyCharacter* EnemyAI = Cast<ABasicEnemyCharacter>(Pawn);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	this->MovementLocationPoint = GetWorld()->SpawnActor<ATargetPoint>(EnemyAI->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
	this->BlackboardComp->SetValueAsBool(AlertedOthersKey, false);

	if (EnemyAI) {
		if (EnemyAI->BehaviorTree->BlackboardAsset) {
			this->BlackboardComp->InitializeBlackboard(*(EnemyAI->BehaviorTree->BlackboardAsset));
		}
		
		this->BehaviorTreeComp->StartTree(*EnemyAI->BehaviorTree);
		this->PatrolPoints = EnemyAI->GetPatrolPoints();
	
		for (TActorIterator<ABasicEnemyCharacter> Itr(GetWorld()); Itr; ++Itr) {
			ABasicEnemyCharacter *OtherGuard = *Itr;
			if (EnemyAI != OtherGuard) {
				float currentDistance = EnemyAI->GetDistanceTo(OtherGuard);
				if (currentDistance <= EnemyAI->EnemyBuddiesRange) {
					this->BuddyEnemies.Add(OtherGuard);
				}
			}
		}
	}
}

void ABasicEnemyAIController::SetPlayerCaught(APawn* Pawn, bool IsCaught) {
	if (this->BlackboardComp) {
		if (IsCaught) {
			BlackboardComp->SetValueAsObject(TargetKey, Pawn);
			this->StopMovement();
		}
		else {
			BlackboardComp->SetValueAsObject(TargetKey, nullptr);
		}
	}
}

ATargetPoint* ABasicEnemyAIController::GetMovementLocationPoint() {
	return this->MovementLocationPoint;
}

void ABasicEnemyAIController::SetMovementLocation(FVector& NewLocation) {
	this->MovementLocationPoint->SetActorLocation(NewLocation);
}
void ABasicEnemyAIController::SetMovementLocationXY(float X, float Y) {
	this->MovementLocationPoint->SetActorLocation(FVector(X, Y, this->GetCharacter()->GetActorLocation().Z));
}

TArray<ABasicEnemyCharacter*>& ABasicEnemyAIController::GetBuddies() {
	return this->BuddyEnemies;
}