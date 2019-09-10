// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "NotDoomGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NOTDOOM_API ANotDoomGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ANotDoomGameModeBase();
	virtual void StartPlay() override;
};
