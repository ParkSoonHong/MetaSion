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
#include "Camera/CameraActor.h"
#include "../../../../Plugins/Experimental/PythonScriptPlugin/Source/PythonScriptPlugin/Public/IPythonScriptPlugin.h"
#include "HighResScreenshot.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

AJS_RoomController::AJS_RoomController()
{
    PrimaryActorTick.bCanEverTick = true; // Tick È°ï¿½ï¿½È­
}

void AJS_RoomController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FHitResult HitResult;
    bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);

    AActor* HoveredActor = bHitSuccessful ? HitResult.GetActor() : nullptr;

    // ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Î¿ï¿½ ï¿½ï¿½ï¿½Í¿ï¿½ ï¿½ï¿½ï¿½ì½ºï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ç¾ï¿½ï¿½Ù¸ï¿½, OnMouseHover È£ï¿½ï¿½
    if (HoveredActor != CurrentHoveredActor)
    {
        if (CurrentHoveredActor)
        {
            OnMouseHoverEnd(CurrentHoveredActor); // ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Í¿ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ì½ºï¿½ï¿½ ï¿½ï¿½ï¿½î³²
        }

        if (HoveredActor)
        {
            OnMouseHover(HoveredActor); // ï¿½ï¿½ï¿½Î¿ï¿½ ï¿½ï¿½ï¿½Í¿ï¿½ ï¿½ï¿½ï¿½ì½ºï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
        }

        CurrentHoveredActor = HoveredActor; // ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ®
    }

}

void AJS_RoomController::BeginPlay()
{
    Super::BeginPlay();

    // ï¿½ï¿½ï¿½ì½º ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    // UIï¿½Ê±ï¿½È­
    InitializeUIWidgets();

    // ï¿½ï¿½ï¿½Ó°ï¿½ UI ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½Ç²ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½Öµï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false); // Ä¸Ã³ ï¿½ß¿ï¿½ ï¿½ï¿½ï¿½ì½º Ä¿ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // ï¿½ï¿½ï¿½ì½ºï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½
    SetInputMode(InputMode);

    // Å¸ÀÌ¸Ó ¼³Á¤: 1ÃÊ °£°ÝÀ¸·Î SpawnAndSwitchToCamera È£Ãâ
    GetWorldTimerManager().SetTimer(LevelCheckTimerHandle, this, &AJS_RoomController::SpawnAndSwitchToCamera, 1.0f, true);
}

void AJS_RoomController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // EnhancedInputLocalPlayerSubsystemï¿½ï¿½ï¿½ï¿½ InputMappingContextï¿½ï¿½ ï¿½ß°ï¿½ï¿½Õ´Ï´ï¿½.
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        //Subsystem->ClearAllMappings();
        Subsystem->AddMappingContext(IMC_Controller, 0);
    }
    // EnhancedInputComponent ï¿½ï¿½ï¿½ï¿½
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
//ï¿½ï¿½
void AJS_RoomController::ShowCreateRoomUI()
{
    if (CR_WidgetUI)
    {
        CR_WidgetUI->SetVisibility(ESlateVisibility::Visible);
    }
}
//ï¿½ï¿½
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
//ï¿½ï¿½
void AJS_RoomController::ShowRoomUI()
{
    UE_LOG(LogTemp, Log, TEXT(" AJS_RoomController::ShowRoomUI()"));

    if (R_UI)
    {
        R_UI->SetVisibility(ESlateVisibility::Visible);
    }
}
//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
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
            // ï¿½Â±ï¿½ Ã¼Å©
            if (HitActor->ActorHasTag(TEXT("WallPaper")))
            {
                UE_LOG(LogTemp, Warning, TEXT("-----------------------------"));
                // ï¿½ï¿½ï¿½â¿¡ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ ï¿½ß°ï¿½ 
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

                // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ìµï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
                WidgetComp->SetVisibility(true);

                // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö´Ï¸ï¿½ï¿½Ì¼ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿?
                UUserWidget* Widget = WidgetComp->GetWidget();
                if (Widget)
                {

                    if (UWBP_Image* WBPImage = Cast<UWBP_Image>(Widget))
                    {
                        if (!WBPImage->IsAnimationPlaying(WBPImage->ShowImage)) // ï¿½Ì¹ï¿½ ï¿½Ö´Ï¸ï¿½ï¿½Ì¼ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ È®ï¿½ï¿½
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

                // À§Á¬À» º¸ÀÌµµ·Ï ¼³Á¤
                WidgetComp->SetVisibility(true);

                // À§Á¬ÀÇ ¾Ö´Ï¸ÞÀÌ¼ÇÀ» Àç»ý
                UUserWidget* Widget = WidgetComp->GetWidget();
                if (Widget)
                {

                    if (UWBP_Image* WBPImage = Cast<UWBP_Image>(Widget))
                    {
                        if (!WBPImage->IsAnimationPlaying(WBPImage->ShowImage)) // ÀÌ¹Ì ¾Ö´Ï¸ÞÀÌ¼ÇÀÌ Àç»ý ÁßÀÎÁö È®ÀÎ
                        {
                            WBPImage->PlayShowImageAnimation();
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

            // Widget Component ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
            UWidgetComponent* WidgetComp = HoveredActor->FindComponentByClass<UWidgetComponent>();
            if (WidgetComp)
            {
                // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿?
                WidgetComp->SetVisibility(false);
                //             UE_LOG(LogTemp, Log, TEXT("get  ShowImage "));

            }
        }
        if (HoveredActor->ActorHasTag(TEXT("Showlist")))
        {
            UE_LOG(LogTemp, Log, TEXT("Hovere end:Showlist "));

            // Widget Component °¡Á®¿À±â
            UWidgetComponent* WidgetComp = HoveredActor->FindComponentByClass<UWidgetComponent>();
            if (WidgetComp)
            {
                // À§Á¬À» ¼û±â±â
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
        // ÇÏ´Ã ·¹º§ À§Ä¡¿Í È¸Àü ¼³Á¤
        CameraLocation = FVector(-470047.589317, 643880.898148, 648118.610643);
        CameraRotation = FRotator(9.157953, 200.435537, 0.000001);
    }
    else if (LevelName == "Main_Room")
    {
        // ¹æ ·¹º§ À§Ä¡¿Í È¸Àü ¼³Á¤
        CameraLocation = FVector(3004.710844, -40.193309, 83.381573);
        CameraRotation = FRotator(4.510870, 1980.785016, 0);
    }
    else
    {
        return;  // Á¶°Ç¿¡ ¸ÂÁö ¾ÊÀ¸¸é ¹ÝÈ¯
    }

    // Ä«¸Þ¶ó ¾×ÅÍ¸¦ ½ºÆù
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ACameraActor* TargetCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), CameraLocation, CameraRotation, SpawnParams);

    if (TargetCamera)
    {
        SetViewTarget(TargetCamera);
        UE_LOG(LogTemp, Log, TEXT("Camera view switched to target camera successfully."));

        // Ä«¸Þ¶ó ÀüÈ¯ÀÌ ¿Ï·áµÇ¸é Å¸ÀÌ¸Ó Á¾·á
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

    // ?Œì¼ ê²½ë¡œ ?¤ì • (?? ?„ë¡œ?íŠ¸ ?”ë ‰? ë¦¬ ??Screenshots ?´ë”)
    FString ScreenshotPath = FPaths::ProjectDir() + TEXT("Screenshots/ScreenCapture.png");

    // ?¤í¬ë¦°ìƒ· ìº¡ì²˜ ?”ì²­
    FScreenshotRequest::RequestScreenshot(ScreenshotPath, false, false);

    UE_LOG(LogTemp, Warning, TEXT("Screenshot saved at: %s"), *ScreenshotPath);

    ExecuteWallPaperPython();
}
//Screen Capture End -----------------------------------------------------------------------------------------

//Wallpaper Python Auto Execute Start ------------------------------------------------------------------------
void AJS_RoomController::ExecuteWallPaperPython()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ExecuteWallPaperPython()"));

    // ?Œì´???Œì¼ ê²½ë¡œ ?¤ì •
    FString ScriptPath = FPaths::ProjectContentDir() + TEXT("Python/Wallpaper.py");

    // ?Œì´???¤í¬ë¦½íŠ¸ ?¤í–‰
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

