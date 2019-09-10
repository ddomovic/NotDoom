// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "SecretInteractable.generated.h"

UCLASS()
class NOTDOOM_API ASecretInteractable : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecretInteractable(const FObjectInitializer &ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FString GetInteractMessage() override;
	virtual void StartInteract(APawn* Initiator) override;
	virtual void FinishInteract(APawn* Initiator) override;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Pickup stuff")
	class USphereComponent* SphereComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	class ATargetPoint* SecretLocation;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	class ATargetPoint* ReturnLocation;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float TimeInSecret;

	UFUNCTION()
	void OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	bool bIsInSecret;
	float CurrentTime;
	APawn* InitiatorPawn;
};
