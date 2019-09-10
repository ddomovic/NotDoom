// Fill out your copyright notice in the Description page of Project Settings.

#include "NotDoomGameModeBase.h"
#include "Engine.h"
#include "ConstructorHelpers.h"
#include "FPSHUD.h"

ANotDoomGameModeBase::ANotDoomGameModeBase() : Super() {
	//DefaultPawnClass = AFPSCharacter::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("Pawn'/Game/PlayableCharacter/BP_FPSCharacter.BP_FPSCharacter_C'"));

	if (PlayerPawnObject.Class != NULL) {
		DefaultPawnClass = PlayerPawnObject.Class;
	}

	HUDClass = AFPSHUD::StaticClass();
}

void ANotDoomGameModeBase::StartPlay() {
	Super::StartPlay();
}
