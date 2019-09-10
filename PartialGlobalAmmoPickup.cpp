#include "PartialGlobalAmmoPickup.h"
#include "FPSCharacter.h"
#include "Weapon.h"
#include "Components/PrimitiveComponent.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "Engine.h"

APartialGlobalAmmoPickup::APartialGlobalAmmoPickup(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
	AmmoRestorePercentage = 0.25f;
}

void APartialGlobalAmmoPickup::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (OtherActor->IsA(AFPSCharacter::StaticClass())) {
		
		AFPSCharacter* tempChar = Cast<AFPSCharacter>(OtherActor);

		TArray<AWeapon*> weapInv = tempChar->GetWeaponInventory();
		
		for (auto w : weapInv) {
			w->SetCurrentAmmo((uint16)(w->GetMaxAmmo()*AmmoRestorePercentage) + w->GetCurrentAmmo());
		}

		tempChar->UpdateAmmo(tempChar->GetCurrentWeapon()->GetCurrentAmmo());

		this->Destroy();
		GEngine->ForceGarbageCollection(true);
	}
}