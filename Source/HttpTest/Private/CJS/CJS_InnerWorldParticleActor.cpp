// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_InnerWorldParticleActor.h"
#include "Components/PointLightComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values
ACJS_InnerWorldParticleActor::ACJS_InnerWorldParticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create DefaultSceneRoot and set it as the RootComponent
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;

    // Initialize and position 6 Point Light Components
    FVector LightPositions[6] = {
        FVector(0.0f, 500.0f, 0.0f),   // Light 0
        FVector(0.0f, -500.0f, 0.0f),  // Light 1
        FVector(-500.0f, 0.0f, 0.0f),  // Light 2
        FVector(500.0f, 0.0f, 0.0f),   // Light 3
        FVector(0.0f, 0.0f, -500.0f),  // Light 4
        FVector(0.0f, 0.0f, 500.0f)    // Light 5
    };

    for (int32 i = 0; i < 6; ++i)
    {
        UPointLightComponent* PointLight = CreateDefaultSubobject<UPointLightComponent>(FName(*FString::Printf(TEXT("PointLight%d"), i)));
        PointLight->SetupAttachment(RootComponent);
        PointLight->SetRelativeLocation(LightPositions[i]);
        PointLights.Add(PointLight);
    }

    // Initialize Niagara System Component
    InnerWorldParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    InnerWorldParticle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACJS_InnerWorldParticleActor::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ACJS_InnerWorldParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACJS_InnerWorldParticleActor::UpdateMyWorldParticle(FLinearColor LightColor, int32 NiagaraAssetIndex)
{
    // Set the Light Color for each Point Light
    for (UPointLightComponent* PointLight : PointLights)
    {
        if (PointLight)
        {
            PointLight->SetLightColor(LightColor);
        }
    }

    // Set the Niagara System Asset based on the index
    if (InnerWorldParticle && NiagaraSystemAssets.IsValidIndex(NiagaraAssetIndex))
    {
        UNiagaraSystem* SelectedNiagaraSystem = NiagaraSystemAssets[NiagaraAssetIndex];
        InnerWorldParticle->SetAsset(SelectedNiagaraSystem); // Set the asset to the NiagaraComponent
        InnerWorldParticle->Activate(); // Activate the Niagara system if needed
    }
}

