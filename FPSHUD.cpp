// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"
#include "Engine/Texture2D.h"
#include "ConstructorHelpers.h"
#include "Engine/Canvas.h"
#include "FPSCharacter.h"


AFPSHUD::AFPSHUD(const FObjectInitializer& ObjectInitializer) {

	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/UI/crosshair_unreal.crosshair_unreal'"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AFPSHUD::DrawHUD() {
	Super::DrawHUD();

	/*const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	const FVector2D CrosshairDrawPosition((Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5f)), (Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)));

	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);*/
}
