// Fill out your copyright notice in the Description page of Project Settings.
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "JsonParseLib.h"
#include "Blueprint/UserWidget.h"
#include "JS_CreateRoomWidget.h"
#include "JS_RoomWidget.h" 
#include "JS_TestWidget.h"
#include "HttpWidget.h"
#include "Components/WidgetComponent.h"
#include "KGW/WBP_Image.h"
#include "Camera/CameraActor.h"
#include "../../../../Plugins/Experimental/PythonScriptPlugin/Source/PythonScriptPlugin/Public/IPythonScriptPlugin.h"
#include "HighResScreenshot.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "CJS/SessionGameInstance.h"

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

    /*USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetGameInstance());
    if (SessionGI)
    {
        UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay()::USessionGameInstance is set"));
        SessionGI->HandleMapChange(GetWorld());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("USessionGameInstance is not set"));
    }*/

    // ���콺 ���� ����
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    // UI�ʱ�ȭ
    InitializeUIWidgets();

    // ���Ӱ� UI �� �� ��ǲ�� ���� �� �ֵ��� ����
    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false); // ĸó �߿� ���콺 Ŀ���� ������ ����
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // ���콺�� �����?����
    SetInputMode(InputMode);

    // Ÿ�̸� ����: 1�� �������� SpawnAndSwitchToCamera ȣ��
    GetWorldTimerManager().SetTimer(LevelCheckTimerHandle, this, &AJS_RoomController::SpawnAndSwitchToCamera, 1.0f, true);
}

void AJS_RoomController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // EnhancedInputLocalPlayerSubsystem���� InputMappingContext�� �߰��մϴ�.
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        //Subsystem->ClearAllMappings();
        if (Subsystem)  // 예외 처리 추가
        {
            Subsystem->AddMappingContext(IMC_Controller, 0);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to get EnhancedInputLocalPlayerSubsystem."));
        }
    }
    // EnhancedInputComponent ����
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
         EnhancedInputComponent->BindAction(IA_LeftMouse, ETriggerEvent::Triggered, this, &AJS_RoomController::OnMouseClick);
    }
}

void AJS_RoomController::InitializeUIWidgets()
{
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    if (LoginUIFactory && LevelName == "Main_Sky") {
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
                // ���⿡ ���� ���?���� �߰� 
                //if (bShowUI) {
                    UE_LOG(LogTemp, Log, TEXT("bShowUI true"));
                    HideCreateRoomUI();
                    ShowRoomUI();
                    PlayUIAnimation();
                    ScreenCapture();
                    R_UI->SendChangeIndexData();
               // }
            }
            else if (HitActor->ActorHasTag(TEXT("Lobby")))
            {
				UE_LOG(LogTemp, Warning, TEXT("Lobby Hit - Loading lobby level"));
                UGameplayStatics::OpenLevel(this, FName("Main_Lobby"));

                 // 서버가 있는 로비로 돌아가기 위한 ClientTravel 사용
			   /* APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PlayerController)
				{
					PlayerController->ClientTravel("/Game/Main/Maps/Main_Lobby", ETravelType::TRAVEL_Relative);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("AJS_RoomController::OnMouseClick():: No PlayerController"));
				}*/

                // GameInstance를 가져와서 JoinSession() 호출
                //USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetGameInstance());
                //if (GameInstance)
                //{
                //    GameInstance->FindSessions();  // 세션을 찾고, 성공 시 JoinSession을 호출
                //}
                //else
                //{
                //    UE_LOG(LogTemp, Error, TEXT("Failed to get SessionGameInstance"));
                //}
            }
            else if (HitActor->ActorHasTag(TEXT("EnterCreateRoom")))
            {

                UE_LOG(LogTemp, Warning, TEXT("Lobby Hit - Loading lobby level"));

                UGameplayStatics::OpenLevel(this, FName("Main_Lobby"));

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

                // ������ �ִϸ��̼��� ���?
                UUserWidget* Widget = WidgetComp->GetWidget();
                if (Widget)
                {

                    if (UWBP_Image* WBPImage = Cast<UWBP_Image>(Widget))
                    {
                        if (!WBPImage->IsAnimationPlaying(WBPImage->ShowImage)) // �̹� �ִϸ��̼��� ���?������ Ȯ��
                        {
                            WBPImage->PlayShowImageAnimation();
                            UE_LOG(LogTemp, Log, TEXT("play ShowImage "));

                        }
                    }
                }
            }

        }
        if (HoveredActor->ActorHasTag(TEXT("Showlist")))
        {
            UWidgetComponent* WidgetComp = HoveredActor->FindComponentByClass<UWidgetComponent>();
            if (WidgetComp)
            {
                UE_LOG(LogTemp, Log, TEXT("Hovered:Showlist "));

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
                            WBPImage->GetImage();
                            UE_LOG(LogTemp, Log, TEXT("play Showlist "));

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
                // ������ �����?
                WidgetComp->SetVisibility(false);
                //             UE_LOG(LogTemp, Log, TEXT("get  ShowImage "));

            }
        }
        if (HoveredActor->ActorHasTag(TEXT("Showlist")))
        {
            UE_LOG(LogTemp, Log, TEXT("Hovere end:Showlist "));

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

void AJS_RoomController::SpawnAndSwitchToCamera()
{
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

    FVector CameraLocation;
    FRotator CameraRotation;

    if (LevelName == "Main_Sky")
    {
        // �ϴ� ���� ��ġ�� ȸ�� ����
        CameraLocation = FVector(-470047.589317, 643880.898148, 648118.610643);
        CameraRotation = FRotator(9.157953, 200.435537, 0.000001);
    }
    else if (LevelName == "Main_Room")
    {
        // �� ���� ��ġ�� ȸ�� ����
        CameraLocation = FVector(3004.710844, -40.193309, 83.381573);
        CameraRotation = FRotator(4.510870, 1980.785016, 0);
    }
    else
    {
        return;  // ���ǿ� ���� ������ ��ȯ
    }

    // ī�޶� ���͸� ����
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ACameraActor* TargetCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), CameraLocation, CameraRotation, SpawnParams);

    if (LevelName == "Main_Room") {
        TargetCamera->GetCameraComponent()->SetFieldOfView(43);
    }
    if (TargetCamera)
    {
        SetViewTarget(TargetCamera);
        UE_LOG(LogTemp, Log, TEXT("Camera view switched to target camera successfully."));

        // ī�޶� ��ȯ�� �Ϸ�Ǹ� Ÿ�̸� ����
        GetWorldTimerManager().ClearTimer(LevelCheckTimerHandle);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn target camera."));
    }
}
//Screen Capture Start ---------------------------------------------------------------------------------------
void AJS_RoomController::ScreenCapture()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ScreenCapture()"));

    // ?�일 경로 ?�정 (?? ?�로?�트 ?�렉?�리 ??Screenshots ?�더)
    FString ScreenshotPath = FPaths::ProjectDir() + TEXT("Screenshots/ScreenCapture.png");

    // ?�크린샷 캡처 ?�청
    FScreenshotRequest::RequestScreenshot(ScreenshotPath, false, false);

    UE_LOG(LogTemp, Warning, TEXT("Screenshot saved at: %s"), *ScreenshotPath);

    ExecuteWallPaperPython();
}
//Screen Capture End -----------------------------------------------------------------------------------------

//Wallpaper Python Auto Execute Start ------------------------------------------------------------------------
void AJS_RoomController::ExecuteWallPaperPython()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ExecuteWallPaperPython()"));

    // ?�이???�일 경로 ?�정
    FString ScriptPath = FPaths::ProjectContentDir() + TEXT("Python/Wallpaper.py");

    // ?�이???�크립트 ?�행
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

