// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Sound/SoundCue.h"
#include "Perception/AISense_Hearing.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->ShootTime = 1.f / this->FireRate;
	this->CurrentTime = this->ShootTime;
	this->CurrentTime2 = this->ShootTime;

	this->WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	this->NoiseEmitterComp = ObjectInitializer.CreateDefaultSubobject<UPawnNoiseEmitterComponent>(this, TEXT("NoiseEmitterComp"));
	this->WeaponLoudness = 1.0f;
	RootComponent = WeaponMesh;
	
	this->MuzzleFlashMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("MuzzleFlashMesh"));
	MuzzleFlashMesh->SetupAttachment(WeaponMesh);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	this->ShootTime = 1.f / this->FireRate;
	this->CurrentTime = this->ShootTime;
	this->CurrentTime2 = this->ShootTime;
	this->OwningCharacter = Cast<AFPSCharacter>(this->GetOwner());
	this->IsCurrentlyFlashing = false;
	MuzzleFlashMesh->SetVisibility(false);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->CurrentTime += DeltaTime;
	this->CurrentTime2 += DeltaTime;

	if (this->OwningCharacter->GetIsShooting()) {

		if (CurrentTime >= ShootTime) {
			if (CurrentAmmo > 0) {
				MuzzleFlashMesh->SetVisibility(true);
				//MuzzleFlashMesh->SetRelativeRotation(FRotator(0.f, 0.f, FMath::FRandRange(0, 360)));
				IsCurrentlyFlashing = true;
				CurrentTime2 = ShootTime;
			}
		}

		if (CurrentTime >= ShootTime) {
			if (CurrentAmmo > 0) {
				Shooting();
			}
			else {
				// play empty clip sound
			}

			CurrentTime = 0.f;
		}
		
	}

	if (IsCurrentlyFlashing && CurrentTime2 >= ShootTime + MuzzleFlashTime) {
		MuzzleFlashMesh->SetVisibility(false);
		IsCurrentlyFlashing = false;
		CurrentTime2 = CurrentTime;
	}
}

void AWeapon::Shooting() {

	if (ProjectileClass != NULL) {
		FVector CameraLoc;
		FRotator CameraRot;
		this->OwningCharacter->GetActorEyesViewPoint(CameraLoc, CameraRot);
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
			AFPSProjectile* const Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

			if (Projectile) {
				FVector const LaunchDir = MuzzleRotation.Vector();
				Projectile->InitVelocity(LaunchDir);
				if (SoundCue) {
					ReportNoise(SoundCue, WeaponLoudness);
				}
				CurrentAmmo--;
				OwningCharacter->UpdateAmmo(CurrentAmmo);
			}
		}
	}
}

void AWeapon::SetCurrentAmmo(uint16 Ammo) {
	CurrentAmmo = Ammo;

	if (CurrentAmmo > MaxAmmo) {
		CurrentAmmo = MaxAmmo;
	}

	OwningCharacter->UpdateAmmo(CurrentAmmo);
}

uint16 AWeapon::GetCurrentAmmo() {
	return CurrentAmmo;
}

uint16 AWeapon::GetMaxAmmo() {
	return MaxAmmo;
}

void AWeapon::ReportNoise(USoundCue* SoundToPlay, float Volume) {
	if (SoundToPlay) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, this->GetActorLocation(), Volume);

		NoiseEmitterComp->MakeNoise(Cast<AFPSCharacter>(this->GetOwner()), Volume, GetActorLocation());
	}
}

void AWeapon::SetAmmoToMax() {
	CurrentAmmo = MaxAmmo;
	OwningCharacter->UpdateAmmo(CurrentAmmo);
}
