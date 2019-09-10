// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSCharacterUI.generated.h"

/**
 * 
 */
UCLASS()
class NOTDOOM_API UFPSCharacterUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void ShowInteractText(const FString& Text);

	UFUNCTION(BlueprintImplementableEvent)
	void HideInteractText();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealth(int CurrentHealth, int MaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAmmo(int CurrentAmmo, int MaxAmmo);
};
