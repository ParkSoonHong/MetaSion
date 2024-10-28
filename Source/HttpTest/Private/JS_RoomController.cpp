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
#include "Components/WidgetComponent.h"
#include "KGW/WBP_Image.h"
#include "../../../../Plugins/Experimental/PythonScriptPlugin/Source/PythonScriptPlugin/Public/IPythonScriptPlugin.h"  // 파이썬 자동 실행
#include "HighResScreenshot.h"
#include "Engine/World.h"
#include "Engine/Engine.h"


AJS_RoomController::AJS_RoomController()
{
    PrimaryActorTick.bCanEverTick = true; // Tick Ȱ��ȭ
}

void AJS_RoomController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FHitResult HitResult;
    bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);

    AActor* HoveredActor = bHitSuccessful ? HitResult.GetActor() : nullptr;

    // ���� ���ο� ���Ϳ� ���콺�� �����Ǿ��ٸ�, OnMouseHover ȣ��
    if (HoveredActor != CurrentHoveredActor)
    {
        if (CurrentHoveredActor)
        {
            OnMouseHoverEnd(CurrentHoveredActor); // ���� ���Ϳ��� ���콺�� ���
        }

        if (HoveredActor)
        {
            OnMouseHover(HoveredActor); // ���ο� ���Ϳ� ���콺�� ����
        }

        CurrentHoveredActor = HoveredActor; // ���� ������ ���� ������Ʈ
    }

}

void AJS_RoomController::BeginPlay()
{
    Super::BeginPlay();

    // ���콺 ���� ����
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    // UI�ʱ�ȭ
    InitializeUIWidgets();

    // ���Ӱ� UI �� �� ��ǲ�� ���� �� �ֵ��� ����
    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false); // ĸó �߿� ���콺 Ŀ���� ������ ����
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // ���콺�� ����� ����
    SetInputMode(InputMode);
}

void AJS_RoomController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // EnhancedInputLocalPlayerSubsystem���� InputMappingContext�� �߰��մϴ�.
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        //Subsystem->ClearAllMappings();
        Subsystem->AddMappingContext(IMC_Controller, 0);
    }
    // EnhancedInputComponent ����
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
//��
void AJS_RoomController::ShowCreateRoomUI()
{
    if (CR_WidgetUI)
    {
        CR_WidgetUI->SetVisibility(ESlateVisibility::Visible);
    }
}
//��
void AJS_RoomController::HideCreateRoomUI()
{
    UE_LOG(LogTemp, Log, TEXT(" AJS_RoomController::HideCreateRoomUI()"));

    if (CR_WidgetUI)
    {
        CR_WidgetUI->SetVisibility(ESlateVisibility::Hidden);
    }
}
//CreateRoom --------------------------------------------------------------------------

//Room --------------------------------------------------------------------------
//��
void AJS_RoomController::ShowRoomUI()
{
    UE_LOG(LogTemp, Log, TEXT(" AJS_RoomController::ShowRoomUI()"));

    if (R_UI)
    {
        R_UI->SetVisibility(ESlateVisibility::Visible);
    }
}
//���� ����
void AJS_RoomController::HideRoomUI()
{
    if (R_UI)
    {
        R_UI->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AJS_RoomController::PlayUIAnimation()
{
    UE_LOG(LogTemp, Log, TEXT(" AJS_RoomController::PlayUIAnimation()"));

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
            UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s at Location: %s"), *HitActor->GetName(), *HitResult.Location.ToString());
            // �±� üũ
            if (HitActor->ActorHasTag(TEXT("WallPaper")))
            {
                UE_LOG(LogTemp, Warning, TEXT("-----------------------------"));
                // ���⿡ ���� ��� ���� �߰� 
                //if (bShowUI) {
                    UE_LOG(LogTemp, Log, TEXT("bShowUI true"));
                    HideCreateRoomUI();
                    ShowRoomUI();
                    PlayUIAnimation();
                    ScreenCapture();
               // }
            }
            else if (HitActor->ActorHasTag(TEXT("Lobby")))
            {

				UE_LOG(LogTemp, Warning, TEXT("Lobby Hit - Loading lobby level"));
                if (bShowUI) {
                    UGameplayStatics::OpenLevel(this, FName("JS_Lobby"));
                }
            }
            else if (HitActor->ActorHasTag(TEXT("EnterCreateRoom")))
            {

                UE_LOG(LogTemp, Warning, TEXT("Lobby Hit - Loading lobby level"));

                UGameplayStatics::OpenLevel(this, FName("JS_Lobby"));

            }

        }
    }
}
//Mouse Interaction --------------------------------------------------------------------------
void AJS_RoomController::OnMouseHover(AActor* HoveredActor)
{
    if (HoveredActor)
    {
        if (HoveredActor->ActorHasTag(TEXT("ShowImage")))
        {
            UWidgetComponent* WidgetComp = HoveredActor->FindComponentByClass<UWidgetComponent>();
            if (WidgetComp)
            {
                UE_LOG(LogTemp, Log, TEXT("Hovered:ShowImage "));

                // ������ ���̵��� ����
                WidgetComp->SetVisibility(true);

                // ������ �ִϸ��̼��� ���
                UUserWidget* Widget = WidgetComp->GetWidget();
                if (Widget)
                {

                    if (UWBP_Image* WBPImage = Cast<UWBP_Image>(Widget))
                    {
                        if (!WBPImage->IsAnimationPlaying(WBPImage->ShowImage)) // �̹� �ִϸ��̼��� ��� ������ Ȯ��
                        {
                            WBPImage->PlayShowImageAnimation();
                            UE_LOG(LogTemp, Log, TEXT("play ShowImage "));

                        }
                    }
                }
            }
        }
    }
}

void AJS_RoomController::OnMouseHoverEnd(AActor* HoveredActor)
{
    if (HoveredActor)
    {
        if (HoveredActor->ActorHasTag(TEXT("ShowImage")))
        {
            UE_LOG(LogTemp, Log, TEXT("Hovere end:ShowImage "));

            // Widget Component ��������
            UWidgetComponent* WidgetComp = HoveredActor->FindComponentByClass<UWidgetComponent>();
            if (WidgetComp)
            {
                // ������ �����
                WidgetComp->SetVisibility(false);
                //             UE_LOG(LogTemp, Log, TEXT("get  ShowImage "));

            }
        }
    }
}

//Screen Capture Start ---------------------------------------------------------------------------------------
void AJS_RoomController::ScreenCapture()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ScreenCapture()"));

    // 파일 경로 설정 (예: 프로젝트 디렉토리 내 Screenshots 폴더)
    FString ScreenshotPath = FPaths::ProjectDir() + TEXT("Screenshots/ScreenCapture.png");

    // 스크린샷 캡처 요청
    FScreenshotRequest::RequestScreenshot(ScreenshotPath, false, false);

    UE_LOG(LogTemp, Warning, TEXT("Screenshot saved at: %s"), *ScreenshotPath);

    ExecuteWallPaperPython();
}
//Screen Capture End -----------------------------------------------------------------------------------------

//Wallpaper Python Auto Execute Start ------------------------------------------------------------------------
void AJS_RoomController::ExecuteWallPaperPython()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ExecuteWallPaperPython()"));

    // 파이썬 파일 경로 설정
    FString ScriptPath = FPaths::ProjectContentDir() + TEXT("Python/Wallpaper.py");

    // 파이썬 스크립트 실행
    IPythonScriptPlugin* PythonPlugin = IPythonScriptPlugin::Get();
    if (PythonPlugin && PythonPlugin->IsPythonAvailable())
    {
        PythonPlugin->ExecPythonCommand(*ScriptPath);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Python is not available in this build."));
    }
}
//Wallpaper Python Auto Execute End ------------------------------------------------------------------------

