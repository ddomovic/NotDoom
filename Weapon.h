#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class NOTDOOM_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	float CurrentTime;
	float CurrentTime2;
	class UAudioComponent* AudioComp;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Custom Transformation Params")
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	uint16 MaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	uint16 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	uint32 DamagePerShot;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float FireRate;
	float ShootTime;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float AccuracyOffsetPitch;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float AccuracyOffsetYaw;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float MuzzleFlashTime;

	UPROPERTY(EditAnywhere, Category = "Weapon Sound")
	class USoundCue* SoundCue;

	UFUNCTION()
	void Shooting();
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	class AFPSCharacter* OwningCharacter;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Muzzle Flash Mesh")
	class UStaticMeshComponent* MuzzleFlashMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPawnNoiseEmitterComponent* NoiseEmitterComp;

	UPROPERTY(EditAnywhere, Category = "Weapon Sound")
	float WeaponLoudness;

	UFUNCTION()
	void SetCurrentAmmo(uint16 Ammo);

	UFUNCTION()
	uint16 GetCurrentAmmo();

	UFUNCTION()
	uint16 GetMaxAmmo();

	UFUNCTION()
	void SetAmmoToMax();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void ReportNoise(class USoundCue* SoundToPlay, float Volume);

private:
	bool IsCurrentlyFlashing;
};
