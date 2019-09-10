#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicEnemyProjectile.generated.h"

UCLASS()
class NOTDOOM_API ABasicEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasicEnemyProjectile(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Projectile stats")
	float ProjectileDamage;

	void InitVelocity(const FVector& ShootDirection);

	UFUNCTION()
	void OnHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
