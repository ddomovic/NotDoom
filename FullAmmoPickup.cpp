// Fill out your copyright notice in the Description page of Project Settings.


#include "FullAmmoPickup.h"
#include "FPSCharacter.h"
#include "Components/PrimitiveComponent.h"
#include "Weapon.h"
#include "Engine.h"

AFullAmmoPickup::AFullAmmoPickup(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
}

void AFullAmmoPickup::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor->IsA(AFPSCharacter::StaticClass())) {
		Cast<AFPSCharacter>(OtherActor)->GetCurrentWeapon()->SetAmmoToMax();
		this->Destroy();
		GEngine->ForceGarbageCollection(true);
	}
}