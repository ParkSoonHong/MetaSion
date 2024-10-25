// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_WidgetFunction.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AJS_WidgetFunction::AJS_WidgetFunction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeScale3D(FVector(1));
	meshComp->SetRelativeLocation(FVector(0, 0, -50.f));
	
}

// Called when the game starts or when spawned
void AJS_WidgetFunction::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_WidgetFunction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

