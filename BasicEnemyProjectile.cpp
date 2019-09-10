// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemyProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine.h"
#include "BasicEnemyCharacter.h"
#include "FPSCharacter.h"

ABasicEnemyProjectile::ABasicEnemyProjectile(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);

	RootComponent = CollisionComp;

	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 8000.f;
	ProjectileMovement->MaxSpeed = 8000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	this->InitialLifeSpan = 3.0f;
}

void ABasicEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABasicEnemyProjectile::OnHit);
}

// Called every frame
void ABasicEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicEnemyProjectile::InitVelocity(const FVector& ShootDirection) {

	if (ProjectileMovement) {
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void ABasicEnemyProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {

	if (OtherActor && (OtherActor != this) && OtherComp) {
		//OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
		if (OtherActor->IsA(AFPSCharacter::StaticClass())) {
			Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->UpdateHealth(-ProjectileDamage);
		}
		this->Destroy();
	}
}