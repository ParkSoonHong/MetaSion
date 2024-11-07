// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_UltraDynamicSkyActor.h"

// Sets default values
ACJS_UltraDynamicSkyActor::ACJS_UltraDynamicSkyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
ACJS_UltraDynamicSkyActor::~ACJS_UltraDynamicSkyActor()
{

}

// Called when the game starts or when spawned
void ACJS_UltraDynamicSkyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACJS_UltraDynamicSkyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACJS_UltraDynamicSkyActor::SetModifyAuroraColors()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetModifyAuroraColors()"));

	Color1 = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
	Color2 = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
	Color3 = FLinearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// 각 색상의 R, G, B, A 값 출력
	UE_LOG(LogTemp, Warning, TEXT("AuroraColor1: R=%f, G=%f, B=%f, A=%f"), Color1.R, Color1.G, Color1.B, Color1.A);
	UE_LOG(LogTemp, Warning, TEXT("AuroraColor2: R=%f, G=%f, B=%f, A=%f"), Color2.R, Color2.G, Color2.B, Color2.A);
	UE_LOG(LogTemp, Warning, TEXT("AuroraColor3: R=%f, G=%f, B=%f, A=%f"), Color3.R, Color3.G, Color3.B, Color3.A);
}

FLinearColor ACJS_UltraDynamicSkyActor::GetAuroraColor1()
{
	return Color1;
}

FLinearColor ACJS_UltraDynamicSkyActor::GetAuroraColor2()
{
	return Color2;
}

FLinearColor ACJS_UltraDynamicSkyActor::GetAurorqaColor3()
{
	return Color3;
}	

