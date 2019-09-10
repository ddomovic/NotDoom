// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class NOTDOOM_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup(const FObjectInitializer &ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	float CurrentTime;
	FVector OriginalLocation;
	FRotator OriginalRotation;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, Category = "Pickup Parameters")
	float FloatAmplitude;

	UPROPERTY(EditAnywhere, Category = "Pickup Parameters")
	float FloatSpeed;

	UPROPERTY(EditAnywhere, Category = "Pickup Parameters")
	float FloatOffset;

	UPROPERTY(EditAnywhere, Category = "Pickup Parameters")
	float SpinsPerSecond;

	UPROPERTY(VisibleDefaultsOnly, Category = "Pickup Parameters")
	class USphereComponent* SphereComp;

	UFUNCTION()
	virtual void OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
