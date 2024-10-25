// Fill out your copyright notice in the Description page of Project Settings.
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

AJS_RoomController::AJS_RoomController()
{
    PrimaryActorTick.bCanEverTick = true; // Tick 활성화
}

void AJS_RoomController::BeginPlay()
{
    Super::BeginPlay();

    // 마우스 관련 설정
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    // 게임과 UI 둘 다 인풋을 받을 수 있도록 설정
    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false); // 캡처 중에 마우스 커서를 숨기지 않음
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 마우스를 잠그지 않음
    SetInputMode(InputMode);
}

void AJS_RoomController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // EnhancedInputLocalPlayerSubsystem에서 InputMappingContext를 추가합니다.
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        //Subsystem->ClearAllMappings();
        Subsystem->AddMappingContext(IMC_Controller, 0);
    }
    // EnhancedInputComponent 설정
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // 마우스 클릭 액션 바인딩
        //if (IA_LeftMouse)
        {
           /* EnhancedInputComponent->BindAction(IA_LeftMouse, ETriggerEvent::Started, this, &AJS_RoomController::OnMouseClick);*/
            EnhancedInputComponent->BindAction(IA_LeftMouse, ETriggerEvent::Triggered, this, &AJS_RoomController::OnMouseClick);
        }
    }
}

void AJS_RoomController::OnMouseClick()
{
    FHitResult HitResult;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
    UE_LOG(LogTemp, Warning, TEXT("Click!!"));

    if (bHitSuccessful) {
        AActor* HitActor = HitResult.GetActor();
        UE_LOG(LogTemp, Warning, TEXT("in bHitSuccessful"));

        if (HitActor)
        {
            UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s at Location: %s"),
                *HitActor->GetName(), *HitResult.Location.ToString());

            // 태그 체크
            if (HitActor->ActorHasTag(TEXT("WallPaper")))
            {
                UE_LOG(LogTemp, Log, TEXT("WallPaper Hit - Processing interaction"));
                // 여기에 사진 찍기 로직 추가
            }
            if (HitActor->ActorHasTag(TEXT("Lobby")))
            {
                UE_LOG(LogTemp, Log, TEXT("Lobby Hit - Loading lobby level"));
                UGameplayStatics::OpenLevel(this, FName("JS_Lobby"));
            }
        }
    }
}