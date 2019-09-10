#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"

UCLASS()
class NOTDOOM_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AFPSProjectile(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	float ProjectileDamage;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	void InitVelocity(const FVector& ShootDirection);

	UFUNCTION()
	void OnHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
