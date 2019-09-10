#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "FullAmmoPickup.generated.h"

UCLASS()
class NOTDOOM_API AFullAmmoPickup : public APickup
{
	GENERATED_BODY()

public:
	AFullAmmoPickup(const FObjectInitializer &ObjectInitializer);
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
