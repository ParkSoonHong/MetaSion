// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_UserCharacter.h"

// Sets default values
ACJS_UserCharacter::ACJS_UserCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACJS_UserCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACJS_UserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACJS_UserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

