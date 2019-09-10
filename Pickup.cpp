#include "Pickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"

APickup::APickup(const FObjectInitializer &ObjectInitializer)
{
 	PrimaryActorTick.bCanEverTick = true;

	this->CurrentTime = 0.f;

	SphereComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	SphereComp->InitSphereRadius(55.f);
	RootComponent = SphereComp;

	StaticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SphereComp);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->BodyInstance.SetCollisionProfileName("Pickup");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlap);
	//SphereComp->OnComponentEndOverlap.AddDynamic(this, &APickup::OnOverlapEnd);

	this->OriginalLocation = this->StaticMeshComp->GetComponentLocation();
	this->OriginalRotation = this->StaticMeshComp->GetComponentRotation();
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector FloatingLocation = this->StaticMeshComp->GetComponentLocation();
	FRotator FloatingRotation = this->StaticMeshComp->GetComponentRotation();

	FloatingLocation.Z = OriginalLocation.Z + FloatAmplitude * FMath::Sin(FloatSpeed * CurrentTime) + FloatOffset;

	this->StaticMeshComp->SetWorldLocation(FloatingLocation);
	this->StaticMeshComp->SetWorldRotation(this->StaticMeshComp->GetComponentRotation() + FRotator(0, 360.f * (DeltaTime*SpinsPerSecond), 0));

	CurrentTime += DeltaTime;
}

void APickup::OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	unimplemented();
}

void APickup::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	unimplemented();
}
