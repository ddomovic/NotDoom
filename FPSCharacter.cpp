#include "FPSCharacter.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/PlayerController.h"
#include "FPSProjectile.h"
#include "Weapon.h"
#include "ConstructorHelpers.h"
#include "FPSCharacterUI.h"
#include "Runtime/Core/Public/Containers/Array.h"

AFPSCharacter::AFPSCharacter(const FObjectInitializer &ObjectInitializer)
{
 	PrimaryActorTick.bCanEverTick = true;

	this->bPressedShoot = false;
	this->currentTime = 0.f;
	
	FirstPersonCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 50.f + BaseEyeHeight);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	this->playerJumpVelocity = 500.f;
	this->GetCharacterMovement()->MaxWalkSpeed = 1000.f;

	FirstPersonMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->AttachToComponent(FirstPersonCameraComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);

	static::ConstructorHelpers::FClassFinder<UFPSCharacterUI> HUDClass(TEXT("/Game/UI/PlayerHUD"));
	if (HUDClass.Class) {
		PlayerHUDClass = HUDClass.Class;
	}
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetOwnerNoSee(true);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	
	this->GetCharacterMovement()->JumpZVelocity = this->playerJumpVelocity;

	UWorld* const World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = Instigator;
		AWeapon* const Weapon = World->SpawnActor<AWeapon>(WeaponClasses[0], GunLocation, FRotator::ZeroRotator, SpawnParameters);
		Weapon->WeaponMesh->SetupAttachment(RootComponent);
		Weapon->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));
		Weapon->SetActorRelativeRotation(Weapon->RelativeRotation);
		CurrentWeapon = Weapon;
		WeaponInventory.Add(CurrentWeapon);
		WeaponInventoryAvailability.Add(true);
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetActorEnableCollision(false);
		CurrentWeaponIndex = 0;

		for (int i = 1; i < WeaponClasses.Num(); i++) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			AWeapon* Wp = World->SpawnActor<AWeapon>(WeaponClasses[i], GunLocation, FRotator::ZeroRotator, SpawnParams);
			Wp->WeaponMesh->SetupAttachment(RootComponent);
			Wp->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));
			Wp->SetActorRelativeRotation(Wp->RelativeRotation);
			WeaponInventory.Add(Wp);
			WeaponInventoryAvailability.Add(false);
			Wp->SetActorHiddenInGame(true);
			Wp->SetActorEnableCollision(false);
			Wp->SetActorTickEnabled(false);
		}

		CurrentWeapon->SetActorHiddenInGame(false);
		CurrentWeapon->SetActorEnableCollision(true);
		CurrentWeapon->SetActorTickEnabled(true);
	}

	if (PlayerHUDClass) {
		PlayerHUD = CreateWidget<UFPSCharacterUI>(Cast<APlayerController>(this->GetController()), PlayerHUDClass);
		if (PlayerHUD) {
			if (UGameplayStatics::GetCurrentLevelName(GetWorld()) != FString("MainMenu") && UGameplayStatics::GetCurrentLevelName(GetWorld()) != FString("GameOverScreen")) {
				PlayerHUD->AddToViewport();
				PlayerHUD->UpdateHealth(CurrentHP, MaxHP);
				PlayerHUD->UpdateAmmo(CurrentWeapon->GetCurrentAmmo(), CurrentWeapon->GetMaxAmmo());
			}
		}
	}
}

void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AFPSCharacter::ShootWeapon);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AFPSCharacter::ShootWeaponStop);

	PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &AFPSCharacter::SwapToWeapon1);
	PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &AFPSCharacter::SwapToWeapon2);
	PlayerInputComponent->BindAction("Weapon3", IE_Pressed, this, &AFPSCharacter::SwapToWeapon3);
	PlayerInputComponent->BindAction("Weapon4", IE_Pressed, this, &AFPSCharacter::SwapToWeapon4);
	PlayerInputComponent->BindAction("Weapon5", IE_Pressed, this, &AFPSCharacter::SwapToWeapon5);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPSCharacter::Interact);
}

void AFPSCharacter::MoveForward(float Val) {
	
	if ((Controller != NULL) && (Val != 0.0f)) {
		
		FRotator Rotation = Controller->GetControlRotation();
		
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling()) {
			Rotation.Pitch = 0.0f;
		}

		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}
}

void AFPSCharacter::MoveRight(float Val) {

	if ((Controller != NULL) && (Val != 0.0f)) {

		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Val);
	}
}

void AFPSCharacter::OnStartJump() {
	
	bPressedJump = true;
}

void AFPSCharacter::OnStopJump() {

	bPressedJump = false;
}

void AFPSCharacter::ShootWeapon() {

	this->bPressedShoot = true;
}

void AFPSCharacter::ShootWeaponStop() {

	this->bPressedShoot = false;
}

void AFPSCharacter::OpenWeapon(uint8 WeaponIndex) {
	
	if (CurrentWeaponIndex != WeaponIndex && WeaponInventoryAvailability[WeaponIndex] == true) {
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetActorEnableCollision(false);
		CurrentWeapon->SetActorTickEnabled(false);
		CurrentWeapon = WeaponInventory[WeaponIndex];
		CurrentWeapon->SetActorHiddenInGame(false);
		CurrentWeapon->SetActorEnableCollision(true);
		CurrentWeapon->SetActorTickEnabled(true);
		CurrentWeaponIndex = WeaponIndex;
		UpdateAmmoWeaponSwap(WeaponIndex);
	}
}

void AFPSCharacter::SwapToWeapon1() {
	OpenWeapon(0);
}

void AFPSCharacter::SwapToWeapon2() {
	OpenWeapon(1);
}

void AFPSCharacter::SwapToWeapon3() {
	OpenWeapon(2);
}

void AFPSCharacter::SwapToWeapon4() {
	OpenWeapon(3);
}

void AFPSCharacter::SwapToWeapon5() {
	OpenWeapon(4);
}

bool AFPSCharacter::GetIsShooting() {
	return this->bPressedShoot;
}

void AFPSCharacter::SetIsShooting(bool isShooting) {
	this->bPressedShoot = isShooting;
}

void AFPSCharacter::EnableWeapon(uint8 WeaponIndex) {

	if (WeaponInventoryAvailability[WeaponIndex] == true) {
		WeaponInventory[WeaponIndex]->SetCurrentAmmo(WeaponInventory[WeaponIndex]->GetMaxAmmo() / 2 + WeaponInventory[WeaponIndex]->GetCurrentAmmo());
	}
	else {
		WeaponInventoryAvailability[WeaponIndex] = true;
		OpenWeapon(WeaponIndex);
	}
}

void AFPSCharacter::NotifyInInteractRange(AActor* Interactable) {

	CurrentInteractable = Cast<IInteractableInterface>(Interactable);

	if (CurrentInteractable) {
		ShowInteractText("E - " + CurrentInteractable->GetInteractMessage());
	}
}

void AFPSCharacter::NotifyLeaveInteractRange(AActor* Interactable) {
	
	CurrentInteractable = nullptr;

	if (CurrentInteractable == nullptr) {
		HideInteractText();
	}
}

void AFPSCharacter::Interact() {
	
	if (CurrentInteractable) {
		CurrentInteractable->StartInteract(this);
	}
}

void AFPSCharacter::ShowInteractText(const FString& Text) {
	
	if (PlayerHUD) {
		PlayerHUD->ShowInteractText(Text);
	}
}

void AFPSCharacter::HideInteractText() {

	if (PlayerHUD) {
		PlayerHUD->HideInteractText();
	}
}

void AFPSCharacter::UpdateHealth(int Value) {

	CurrentHP += Value;

	if (CurrentHP <= 0) {
		OnPlayerDeath();
	}
	else if (CurrentHP > MaxHP) {
		CurrentHP = MaxHP;
	}

	if (PlayerHUD) {
		PlayerHUD->UpdateHealth(CurrentHP, MaxHP);
	}
}

void AFPSCharacter::UpdateAmmo(int Value) {

	if (PlayerHUD) {
		PlayerHUD->UpdateAmmo(Value, CurrentWeapon->GetMaxAmmo());
	}
}

void AFPSCharacter::UpdateAmmoWeaponSwap(uint8 WeaponIndex) {

	uint16 MaxAmmo = this->WeaponInventory[WeaponIndex]->GetMaxAmmo();
	uint16 CurrentAmmo = this->WeaponInventory[WeaponIndex]->GetCurrentAmmo();

	if (PlayerHUD) {
		PlayerHUD->UpdateAmmo(CurrentAmmo, MaxAmmo);
	}
}

void AFPSCharacter::OnPlayerDeath() {
	UGameplayStatics::OpenLevel(GetWorld(), "GameOverScreen");
}

AWeapon* AFPSCharacter::GetCurrentWeapon() {
	return CurrentWeapon;
}

TArray<AWeapon*>& AFPSCharacter::GetWeaponInventory() {
	return WeaponInventory;
}

float AFPSCharacter::GetMaxHP() {
	return MaxHP;
}

float AFPSCharacter::GetCurrentHP() {
	return CurrentHP;
}