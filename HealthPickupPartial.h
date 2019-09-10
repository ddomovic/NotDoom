#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickupPartial.generated.h"

UCLASS()
class NOTDOOM_API AHealthPickupPartial : public APickup
{
	GENERATED_BODY()

public:
	AHealthPickupPartial(const FObjectInitializer &ObjectInitializer);
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float HealthRestorePercentage;
};
