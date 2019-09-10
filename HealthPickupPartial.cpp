// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickupPartial.h"
#include "Components/PrimitiveComponent.h"
#include "FPSCharacter.h"
#include "Engine.h"

AHealthPickupPartial::AHealthPickupPartial(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {

	HealthRestorePercentage = 0.25f;
}

void AHealthPickupPartial::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (OtherActor->IsA(AFPSCharacter::StaticClass())) {
		AFPSCharacter* tempChar = Cast<AFPSCharacter>(OtherActor);
		tempChar->UpdateHealth((int)(tempChar->GetMaxHP()*HealthRestorePercentage));
		this->Destroy();
		GEngine->ForceGarbageCollection(true);
	}
}