// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponPickup.generated.h"

UCLASS()
class NOTDOOM_API AWeaponPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPickup(const FObjectInitializer &ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float CurrentTime;
	FVector OriginalLocation;
	FRotator OriginalRotation;

public:	
	// Called every frame
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

	UPROPERTY(EditAnywhere, Category = "Pickup Parameters")
	uint8 WeaponIndex;

	UFUNCTION()
	void OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
