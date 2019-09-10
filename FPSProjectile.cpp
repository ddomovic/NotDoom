// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine.h"
#include "Weapon.h"
#include "BasicEnemyCharacter.h"
#include "FPSCharacter.h"

// Sets default values
AFPSProjectile::AFPSProjectile(const FObjectInitializer& ObjectInitializer)
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
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;

	this->InitialLifeSpan = 3.0f;
}

void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
	this->ProjectileDamage = Cast<AWeapon>(this->GetOwner())->DamagePerShot;
}

void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSProjectile::InitVelocity(const FVector& ShootDirection) {

	if (ProjectileMovement) {
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {

	if (OtherActor && (OtherActor != this) && OtherComp) {
		//OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
		if (OtherActor->IsA(ABasicEnemyCharacter::StaticClass())) {
			ABasicEnemyCharacter* TempEnemy = Cast<ABasicEnemyCharacter>(OtherActor);
			TempEnemy->UpdateHealth(-ProjectileDamage);
		}
		this->Destroy();
	}
}