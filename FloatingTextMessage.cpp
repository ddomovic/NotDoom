// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingTextMessage.h"
#include "Components/TextRenderComponent.h"

AFloatingTextMessage::AFloatingTextMessage(const FObjectInitializer &ObjectInitializer)
{
 	PrimaryActorTick.bCanEverTick = false;

	TextRenderComp = ObjectInitializer.CreateDefaultSubobject<UTextRenderComponent>(this, TEXT("TextComp"));
}

void AFloatingTextMessage::BeginPlay()
{
	Super::BeginPlay();

}

void AFloatingTextMessage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

