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

AJS_RoomController::AJS_RoomController()
{
    PrimaryActorTick.bCanEverTick = true; // Tick 활성화
}

void AJS_RoomController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FHitResult HitResult;
    bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);

    AActor* HoveredActor = bHitSuccessful ? HitResult.GetActor() : nullptr;

    // 만약 새로운 액터에 마우스가 오버되었다면, OnMouseHover 호출
    if (HoveredActor != CurrentHoveredActor)
    {
        if (CurrentHoveredActor)
        {
            OnMouseHoverEnd(CurrentHoveredActor); // 이전 액터에서 마우스가 벗어남
        }

        if (HoveredActor)
        {
            OnMouseHover(HoveredActor); // 새로운 액터에 마우스가 들어옴
        }

        CurrentHoveredActor = HoveredActor; // 현재 오버된 액터 업데이트
    }

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

    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

    if (LevelName == TEXT("Main_Sky")) {
        SpawnAndSwitchToCamera();
    }
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
            else if (HitActor->ActorHasTag(TEXT("EnterCreateRoom")))
            {

                UE_LOG(LogTemp, Log, TEXT("Lobby Hit - Loading lobby level"));

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

                // 위젯을 보이도록 설정
                WidgetComp->SetVisibility(true);

                // 위젯의 애니메이션을 재생
                UUserWidget* Widget = WidgetComp->GetWidget();
                if (Widget)
                {

                    if (UWBP_Image* WBPImage = Cast<UWBP_Image>(Widget))
                    {
                        if (!WBPImage->IsAnimationPlaying(WBPImage->ShowImage)) // 이미 애니메이션이 재생 중인지 확인
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

                // 위젯을 보이도록 설정
                WidgetComp->SetVisibility(true);

                // 위젯의 애니메이션을 재생
                UUserWidget* Widget = WidgetComp->GetWidget();
                if (Widget)
                {

                    if (UWBP_Image* WBPImage = Cast<UWBP_Image>(Widget))
                    {
                        if (!WBPImage->IsAnimationPlaying(WBPImage->ShowImage)) // 이미 애니메이션이 재생 중인지 확인
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

            // Widget Component 가져오기
            UWidgetComponent* WidgetComp = HoveredActor->FindComponentByClass<UWidgetComponent>();
            if (WidgetComp)
            {
                // 위젯을 숨기기
                WidgetComp->SetVisibility(false);
                //             UE_LOG(LogTemp, Log, TEXT("get  ShowImage "));

            }
        }
        if (HoveredActor->ActorHasTag(TEXT("Showlist")))
        {
            UE_LOG(LogTemp, Log, TEXT("Hovere end:Showlist "));

            // Widget Component 가져오기
            UWidgetComponent* WidgetComp = HoveredActor->FindComponentByClass<UWidgetComponent>();
            if (WidgetComp)
            {
                // 위젯을 숨기기
                WidgetComp->SetVisibility(false);
                //             UE_LOG(LogTemp, Log, TEXT("get  ShowImage "));

            }
        }

    }
}

void AJS_RoomController::SpawnAndSwitchToCamera()
{
    // 원하는 위치와 회전을 지정
    FVector CameraLocation(-470047.589317, 643880.898148, 648118.610643);
    FRotator CameraRotation(9.157953, 200.435537, 0.000001);

    // 카메라 액터를 스폰
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ACameraActor* TargetCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), CameraLocation, CameraRotation, SpawnParams);

    // 카메라가 정상적으로 생성되었는지 확인 후 뷰 타겟으로 전환
    if (TargetCamera)
    {
        SetViewTarget(TargetCamera);
        UE_LOG(LogTemp, Log, TEXT("Camera view switched to target camera successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn target camera."));
    }
}



