// Fill out your copyright notice in the Description page of Project Settings.


#include "SecretInteractable.h"
#include "GameFramework/Pawn.h"
#include "InteractInterface.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "FPSCharacter.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Engine.h"

// Sets default values
ASecretInteractable::ASecretInteractable(const FObjectInitializer &ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	SphereComp->InitSphereRadius(55.f);

	StaticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMeshComp"));
	
	RootComponent = SphereComp;

	StaticMeshComp->SetupAttachment(SphereComp);

	TimeInSecret = 10.f;
}

// Called when the game starts or when spawned
void ASecretInteractable::BeginPlay()
{
	Super::BeginPlay();
	bIsInSecret = false;
	CurrentTime = 0.f;

	SphereComp->BodyInstance.SetCollisionProfileName("Pickup");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASecretInteractable::OnOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ASecretInteractable::OnOverlapEnd);
}

// Called every frame
void ASecretInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInSecret) {
		CurrentTime += DeltaTime;
		if (CurrentTime >= TimeInSecret) {
			InitiatorPawn->SetActorLocation(ReturnLocation->GetActorLocation());
			this->Destroy();
		}
	}
	
}

FString ASecretInteractable::GetInteractMessage() {
	return FString(TEXT("Secret"));
}

void ASecretInteractable::StartInteract(APawn* Initiator) {
	
	Initiator->SetActorLocation(SecretLocation->GetActorLocation());
	InitiatorPawn = Initiator;
	bIsInSecret = true;
}

void ASecretInteractable::FinishInteract(APawn* Initiator) {

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Interact finished!"));
}

void ASecretInteractable::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor->IsA(AFPSCharacter::StaticClass())) {
		AFPSCharacter* PlayerChar = Cast<AFPSCharacter>(OtherActor);
		IInteractInterface* Interface = Cast<IInteractInterface, AFPSCharacter>(PlayerChar);
		if (Interface) {
			SetOwner(PlayerChar);
			Interface->NotifyInInteractRange(this);
		}
	}
}

void ASecretInteractable::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	if (OtherActor->IsA(AFPSCharacter::StaticClass())) {
		AFPSCharacter* PlayerChar = Cast<AFPSCharacter>(OtherActor);
		IInteractInterface* Interface = Cast<IInteractInterface, AFPSCharacter>(PlayerChar);
		if (Interface) {
			SetOwner(nullptr);
			Interface->NotifyLeaveInteractRange(this);
		}
	}
}
