// Fill out your copyright notice in the Description page of Project Settings.
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "JsonParseLib.h"
#include "Blueprint/UserWidget.h"
#include "JS_CreateRoomWidget.h"
#include "JS_RoomWidget.h" 
#include "JS_TestWidget.h"
#include "HttpWidget.h"

AJS_RoomController::AJS_RoomController()
{
    PrimaryActorTick.bCanEverTick = true; // Tick 활성화
}

void AJS_RoomController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AJS_RoomController::BeginPlay()
{
    Super::BeginPlay();

    // 마우스 관련 설정
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    // UI초기화
    InitializeUIWidgets();

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
         EnhancedInputComponent->BindAction(IA_LeftMouse, ETriggerEvent::Triggered, this, &AJS_RoomController::OnMouseClick);
    }
}

void AJS_RoomController::InitializeUIWidgets()
{
    if (LoginUIFactory) {
        LoginUI = CreateWidget<UHttpWidget>(this, LoginUIFactory);
        if (LoginUI) {
            LoginUI->AddToViewport();
            LoginUI->SetVisibility(ESlateVisibility::Visible);
        }
    }
    if (CR_UIFactory) {
        CR_WidgetUI = CreateWidget<UJS_CreateRoomWidget>(this, CR_UIFactory);
        if (CR_WidgetUI) {
            CR_WidgetUI->AddToViewport();
            CR_WidgetUI->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    if (R_UIFactory)
    {
        R_UI = CreateWidget<UJS_RoomWidget>(this, R_UIFactory);
        if (R_UI)
        {
            R_UI->AddToViewport();
            R_UI->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}
void AJS_RoomController::ShowLoginUI()
{
    if (LoginUI)
    {
        LoginUI->SetVisibility(ESlateVisibility::Visible);
    }
}
void AJS_RoomController::HideLoginUI()
{
    if (LoginUI)
    {
        LoginUI->SetVisibility(ESlateVisibility::Hidden);
    }
}
//CreateRoom --------------------------------------------------------------------------
//씀
void AJS_RoomController::ShowCreateRoomUI()
{
    if (CR_WidgetUI)
    {
        CR_WidgetUI->SetVisibility(ESlateVisibility::Visible);
    }
}
//씀
void AJS_RoomController::HideCreateRoomUI()
{
    if (CR_WidgetUI)
    {
        CR_WidgetUI->SetVisibility(ESlateVisibility::Hidden);
    }
}
//CreateRoom --------------------------------------------------------------------------

//Room --------------------------------------------------------------------------
//씀
void AJS_RoomController::ShowRoomUI()
{
    if (R_UI)
    {
        R_UI->SetVisibility(ESlateVisibility::Visible);
    }
}
//아직 못씀
void AJS_RoomController::HideRoomUI()
{
    if (R_UI)
    {
        R_UI->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AJS_RoomController::PlayUIAnimation()
{
    if (R_UI) {
        R_UI->PlayAnimation(R_UI->CameraSutterEffect);
    }
}
//Room --------------------------------------------------------------------------

//Mouse Interaction --------------------------------------------------------------------------
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
            UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s at Location: %s"), *HitActor->GetName(), *HitResult.Location.ToString());
            // 태그 체크
            if (HitActor->ActorHasTag(TEXT("WallPaper")))
            {
                // 여기에 사진 찍기 로직 추가 
                if (bShowUI) {
                    HideCreateRoomUI();
                    ShowRoomUI();
                    PlayUIAnimation();
                }
            }
            else if (HitActor->ActorHasTag(TEXT("Lobby")))
            {

				UE_LOG(LogTemp, Log, TEXT("Lobby Hit - Loading lobby level"));
                if (bShowUI) {
                    UGameplayStatics::OpenLevel(this, FName("JS_Lobby"));
                }
            }
        }
    }
}
//Mouse Interaction --------------------------------------------------------------------------