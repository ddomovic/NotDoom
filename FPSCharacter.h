// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractInterface.h"
#include "InteractableInterface.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "FPSCharacter.generated.h"

UCLASS()
class NOTDOOM_API AFPSCharacter : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter(const FObjectInitializer &ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bPressedShoot;

	UPROPERTY(EditAnywhere, Category= "PlayerParameters")
	float playerJumpVelocity;

	void Interact();

	UPROPERTY()
	class TSubclassOf<class UFPSCharacterUI> PlayerHUDClass;

	UPROPERTY()
	class UFPSCharacterUI* PlayerHUD;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetIsShooting();
	void SetIsShooting(bool isShooting);

	UFUNCTION()
	void MoveForward(float Val);

	UFUNCTION()
	void MoveRight(float Val);

	UFUNCTION()
	void OnStartJump();

	UFUNCTION()
	void OnStopJump();

	UFUNCTION()
	void ShootWeapon();

	UFUNCTION()
	void ShootWeaponStop();

	UFUNCTION()
	void OpenWeapon(uint8 WeaponIndex);

	UFUNCTION()
	void SwapToWeapon1();

	UFUNCTION()
	void SwapToWeapon2();

	UFUNCTION()
	void SwapToWeapon3();

	UFUNCTION()
	void SwapToWeapon4();

	UFUNCTION()
	void SwapToWeapon5();

	UFUNCTION()
	void EnableWeapon(uint8 WeaponIndex);

	virtual void NotifyInInteractRange(AActor* Interactable) override;
	virtual void NotifyLeaveInteractRange(AActor* Interactable) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(EditAnywhere, Category = "Character Stats")
	float MaxHP;

	UPROPERTY(EditAnywhere, Category = "Character Stats")
	float CurrentHP;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FVector GunLocation;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FRotator GunOffsetRotation;

	class AWeapon* CurrentWeapon;
	uint8 CurrentWeaponIndex;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<class AWeapon> > WeaponClasses;

	TArray<class AWeapon*> WeaponInventory;
	TArray<bool> WeaponInventoryAvailability;

	UFUNCTION()
	void ShowInteractText(const FString& Text);

	UFUNCTION()
	void HideInteractText();

	UFUNCTION()
	void UpdateHealth(int Value);

	UFUNCTION()
	void UpdateAmmo(int Value);

	UFUNCTION()
	void UpdateAmmoWeaponSwap(uint8 WeaponIndex);

	UFUNCTION()
	void OnPlayerDeath();

	UFUNCTION()
	class AWeapon* GetCurrentWeapon();

	UFUNCTION()
	TArray<class AWeapon*>& GetWeaponInventory();

	UFUNCTION()
	float GetMaxHP();

	UFUNCTION()
	float GetCurrentHP();

private:
	float currentTime;
	IInteractableInterface* CurrentInteractable = nullptr;
};
