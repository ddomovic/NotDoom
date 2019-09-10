// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine.h"
#include "FPSCharacter.h"
#include "Weapon.h"

// Sets default values
AWeaponPickup::AWeaponPickup(const FObjectInitializer &ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->CurrentTime = 0.f;

	SphereComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	SphereComp->InitSphereRadius(55.f);
	RootComponent = SphereComp;
	
	StaticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->BodyInstance.SetCollisionProfileName("Pickup");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponPickup::OnOverlap);
	//SphereComp->OnComponentEndOverlap.AddDynamic(this, &AWeaponPickup::OnOverlapEnd);

	this->OriginalLocation = this->StaticMeshComp->GetComponentLocation();
	this->OriginalRotation = this->StaticMeshComp->GetComponentRotation();
}

// Called every frame
void AWeaponPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector FloatingLocation = this->StaticMeshComp->GetComponentLocation();
	FRotator FloatingRotation = this->StaticMeshComp->GetComponentRotation();

	FloatingLocation.Z = OriginalLocation.Z + FloatAmplitude * FMath::Sin(FloatSpeed * CurrentTime) + FloatOffset;

	this->StaticMeshComp->SetWorldLocation(FloatingLocation);
	this->StaticMeshComp->SetWorldRotation(this->StaticMeshComp->GetComponentRotation() + FRotator(0, 360.f * (DeltaTime*SpinsPerSecond), 0));

	CurrentTime += DeltaTime;
}

void AWeaponPickup::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (OtherActor->IsA(AFPSCharacter::StaticClass())) {
		Cast<AFPSCharacter>(OtherActor)->EnableWeapon(this->WeaponIndex);
		this->Destroy();
		GEngine->ForceGarbageCollection(true);
	}
	
}

void AWeaponPickup::OnOverlapEnd(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
}