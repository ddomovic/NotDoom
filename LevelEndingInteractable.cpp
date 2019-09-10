// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEndingInteractable.h"
#include "GameFramework/Pawn.h"
#include "InteractInterface.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "FPSCharacter.h"
#include "Engine.h"

// Sets default values
ALevelEndingInteractable::ALevelEndingInteractable(const FObjectInitializer &ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	SphereComp->InitSphereRadius(55.f);

	StaticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMeshComp"));
	StaticMeshComp2 = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMeshComp2"));

	RootComponent = SphereComp;

	StaticMeshComp->SetupAttachment(SphereComp);
	StaticMeshComp2->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void ALevelEndingInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->BodyInstance.SetCollisionProfileName("Pickup");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ALevelEndingInteractable::OnOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ALevelEndingInteractable::OnOverlapEnd);
}

// Called every frame
void ALevelEndingInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ALevelEndingInteractable::GetInteractMessage() {
	return FString(TEXT("Finish the level"));
}

void ALevelEndingInteractable::StartInteract(APawn* Initiator) {
	
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void ALevelEndingInteractable::FinishInteract(APawn* Initiator) {

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Interact finished!"));
}

void ALevelEndingInteractable::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor->IsA(AFPSCharacter::StaticClass())) {
		AFPSCharacter* PlayerChar = Cast<AFPSCharacter>(OtherActor);
		IInteractInterface* Interface = Cast<IInteractInterface, AFPSCharacter>(PlayerChar);
		if (Interface) {
			SetOwner(PlayerChar);
			Interface->NotifyInInteractRange(this);
		}
	}
}

void ALevelEndingInteractable::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	if (OtherActor->IsA(AFPSCharacter::StaticClass())) {
		AFPSCharacter* PlayerChar = Cast<AFPSCharacter>(OtherActor);
		IInteractInterface* Interface = Cast<IInteractInterface, AFPSCharacter>(PlayerChar);
		if (Interface) {
			SetOwner(nullptr);
			Interface->NotifyLeaveInteractRange(this);
		}
	}
}