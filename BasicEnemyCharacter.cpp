#include "BasicEnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "AIPatrolPoint.h"
#include "BasicEnemyAIController.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "BasicEnemyProjectile.h"
#include "ConstructorHelpers.h"
#include "FPSCharacter.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "Sound/SoundCue.h"
#include "Engine.h"

ABasicEnemyCharacter::ABasicEnemyCharacter() {
	PrimaryActorTick.bCanEverTick = true;
	this->EnemyID = 0;
	this->PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	this->PawnSensingComp->SetPeripheralVisionAngle(120.0f);
	this->PawnSensingComp->SightRadius = 1500.0f;
	this->EnemyAlertRange = 2000.0f;
	this->EnemyBuddiesRange = 3000.0f;
	this->WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	this->WeaponMesh->SetupAttachment(this->GetMesh(), TEXT("GunGripSocket"));
}

void ABasicEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->ShootTime = 1.f / this->FireRate;
	this->CurrentTime = ShootTime;
	
	if (this->PawnSensingComp) {
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ABasicEnemyCharacter::OnPlayerCaught);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &ABasicEnemyCharacter::OnPlayerHeard);
	}

	this->WeaponMesh->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunGripSocket"));
	//this->WeaponMesh->AttachTo(this->GetMesh(), TEXT("GunGripSocket"));
	bIsFighting = false;
}

void ABasicEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->CurrentTime += DeltaTime;

	if (this->bIsFighting && CurrentTime >= ShootTime) {

		Shooting();
		this->CurrentTime = 0.f;
	}
}

void ABasicEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicEnemyCharacter::OnPlayerCaught(APawn* Pawn) {

	ABasicEnemyAIController* AIController = Cast<ABasicEnemyAIController>(GetController());
	if (Pawn) {
		this->CurrentEnemy = Pawn;
	}
	if (AIController) {
		AIController->SetPlayerCaught(Pawn, true);
	}
}

void ABasicEnemyCharacter::OnPlayerHeard(APawn* PawnInstigator, const FVector& Location, float Volume) {

	ABasicEnemyAIController* EnemyAIController = Cast<ABasicEnemyAIController>(GetController());

	if (EnemyAIController && PawnInstigator != this) {
		if (PawnInstigator) {
			this->CurrentEnemy = PawnInstigator;
		}
		if (EnemyAIController) {
			EnemyAIController->SetPlayerCaught(PawnInstigator, true);
		}
	}
}

APawn* ABasicEnemyCharacter::GetCurrentEnemy() {
	return this->CurrentEnemy;
}

void ABasicEnemyCharacter::SetCurrentEnemy(APawn* NewEnemy) {
	this->CurrentEnemy = NewEnemy;
}

TArray<AAIPatrolPoint*>& ABasicEnemyCharacter::GetPatrolPoints() {
	return this->PatrolPoints;
}

void ABasicEnemyCharacter::UpdateHealth(float Value) {
	this->CurrentHealth += Value;

	if (this->CurrentHealth > this->MaxHealth) {
		this->CurrentHealth = this->MaxHealth;
	}
	
	if (this->CurrentHealth <= 0.f) {
		OnDeath();
	}
}

void ABasicEnemyCharacter::OnDeath() {

	USkeletalMeshComponent* SkelMesh = this->GetMesh();
	
	bIsFighting = false;

	this->DetachFromControllerPendingDestroy();
	SkelMesh->SetAllBodiesBelowSimulatePhysics("pelvis", true);
	SkelMesh->SetAllBodiesBelowPhysicsBlendWeight("pelvis", 1.0);
}

bool ABasicEnemyCharacter::GetIsFighting() {
	return this->bIsFighting;
}

void ABasicEnemyCharacter::SetIsFighting(bool IsFighting) {
	bIsFighting = IsFighting;
}

void ABasicEnemyCharacter::Shooting() {
	
	if (ProjectileClass != NULL) {
		FVector CameraLoc;
		FRotator CameraRot;
		this->GetActorEyesViewPoint(CameraLoc, CameraRot);
		FVector const MuzzleLocation = this->WeaponMesh->GetSocketLocation("MuzzleSocket");
		FRotator MuzzleRotation = CameraRot;
		MuzzleRotation.Pitch += -5.f;
		MuzzleRotation.Pitch += FMath::FRandRange(-AccuracyOffsetPitch, AccuracyOffsetPitch);
		MuzzleRotation.Yaw += FMath::FRandRange(-AccuracyOffsetYaw, AccuracyOffsetYaw);
		UWorld* const World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			ABasicEnemyProjectile* const Projectile = World->SpawnActor<ABasicEnemyProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

			if (Projectile) {
				FVector const LaunchDir = MuzzleRotation.Vector();
				if (SoundToPlay) {
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, this->GetActorLocation(), WeaponLoudness);
				}
				Projectile->InitVelocity(LaunchDir);
			}
		}
	}
}