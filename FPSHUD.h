// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class NOTDOOM_API AFPSHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AFPSHUD(const FObjectInitializer& ObjectInitializer);

private:
	class UTexture2D* CrosshairTex;
	virtual void DrawHUD() override;
};
