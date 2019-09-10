// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "PartialGlobalAmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class NOTDOOM_API APartialGlobalAmmoPickup : public APickup
{
	GENERATED_BODY()

public:
	APartialGlobalAmmoPickup(const FObjectInitializer &ObjectInitializer);
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float AmmoRestorePercentage;
};
