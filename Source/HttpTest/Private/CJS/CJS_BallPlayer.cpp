﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_BallPlayer.h"
#include "CJS/CJS_HeartActor.h"
#include "CJS/CJS_BallPlayerAnimInstance.h"
#include "CJS/CJS_AimPointWidget.h"
#include "CJS/CJS_MultiRoomActor.h"
#include "CJS/CJS_HttpActor.h"
#include "HttpActor.h"
#include "JsonParseLib.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Blueprint/UserWidget.h"

#include "JsonUtilities.h" // JSON 관련 유틸리티
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "CJS/SessionGameInstance.h"

#include "../../../../Plugins/Experimental/PythonScriptPlugin/Source/PythonScriptPlugin/Public/IPythonScriptPlugin.h"  // 파이썬 자동 실행




// Sets default values
ACJS_BallPlayer::ACJS_BallPlayer() : Super()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	//SpringArmComp->SetupAttachment(RootComponent);
	//SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->SetupAttachment(GetMesh());
	//SpringArmComp->SetRelativeLocation(FVector(0, 0, 0));
	SpringArmComp->TargetArmLength = 3000.f;
	SpringArmComp->SocketOffset = FVector(0.f, 0.f, 100.0f);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	//CameraComp->bUsePawnControlRotation = true;
	CameraComp->bUsePawnControlRotation = false;

	// 컨트롤러 회전 사용 설정
	bUseControllerRotationYaw = true;
	// 캐릭터가 이동 방향을 따르지 않도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// 설정할 하트의 초기 위치를 위한 위치 값 (직접 값을 조정 가능)
	HeartSpawnPosition = FVector(300.f, 0.f, 50.f); // 적당히 초기 위치 오프셋 지정 

	// 멀티 플레이 적용
	bReplicates = true; // 네트워크 복제를 설정
    SetReplicateMovement(true); // 이동 복제를 설정


	// 초기 설정 ================================================================================
	/* 재질 색상 설정 */ 
	//SetInitColorValue(1.0, 0.9225690792809692, 0.4);
	//InitColorValue = FLinearColor(0.1, 1.0, 0.7);
	/* 추천방 정보 설정 */
	//SetInitMultiRoomInfo(1, 5, "빛나는 호수", 87);
	// 월드에서 MultiRoomActor 클래스의 인스턴스를 찾습니다.

	InitJsonData(Json);
}
	


// Called when the game starts or when spawned
void ACJS_BallPlayer::BeginPlay()
{
	Super::BeginPlay();

	// SessionGameInstance 할당
	/*SessionGI = Cast<USessionGameInstance>(GetGameInstance());
	if (SessionGI)
	{
		UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance is set"));
		InitJsonData(SessionGI->GetNetInfoCharacterTOLobby());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USessionGameInstance is not set"));
	}*/

	// SessionGameInstance 할당 + 컨트롤러 변경
	//SessionGI = Cast<USessionGameInstance>(GetGameInstance());
	//if (SessionGI)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay()::USessionGameInstance is set"));
	//	//InitJsonData(SessionGI->GetNetInfoCharacterTOLobby());
	//	SessionGI->HandleMapChange(GetWorld());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("USessionGameInstance is not set"));
	//}


	// Initialize from JSON data
	InitializeFromJson(JsonData);

	// Material 설정 부분 추가 (SkeletalMesh 사용)
	if (GetMesh()) // SkeletalMeshComponent 접근
	{
		UMaterialInterface* MaterialInterface = GetMesh()->GetMaterial(0);
		if (MaterialInterface)
		{
			// Dynamic Material Instance 생성
			//UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInterface, this);
			if (DynamicMaterialInstance)
			{
				GetMesh()->SetMaterial(0, DynamicMaterialInstance);

				// InitColorValue를 사용하여 TransmitB 파라미터 설정
				DynamicMaterialInstance->SetVectorParameterValue(FName("TransmitB"), InitColorValue);
				UE_LOG(LogTemp, Warning, TEXT("SkeletalMesh Material color set using 'TransmitB' parameter to R: %f, G: %f, B: %f"), InitColorValue.R, InitColorValue.G, InitColorValue.B);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create Dynamic Material Instance."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MaterialInterface is null."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SkeletalMeshComponent (GetMesh()) is null."));
	}
	// 초기 회전값을 Identity로 설정
	MaterialRotationQuat = FQuat::Identity;
	TargetYaw = 0.0f; // 초기 목표 Yaw 값


	// 추천방 정보 초기화
	//SetInitMultiRoomInfo(1, 5, "SunnyWorld", 87);
	//AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACJS_MultiRoomActor::StaticClass());
	//if (FoundActor)
	//{
	//	// ACJS_MultiRoomActor로 캐스팅
	//	RefMultiRoom = Cast<ACJS_MultiRoomActor>(FoundActor);
	//	if (RefMultiRoom)
	//	{
	//		// 초기 설정 함수 호출
	//		RefMultiRoom->InitRefRoomInfoWidget(1, 5, "SunnyWorld", 87);
	//		UE_LOG(LogTemp, Warning, TEXT("MultiRoom information initialized successfully."));
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Failed to cast FoundActor to ACJS_MultiRoomActor."));
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("No ACJS_MultiRoomActor found in the world."));
	//}


	// 컨트롤러를 가져와서 캐스팅
	PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay()::PlayerController (pc) is assigned in BeginPlay"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay()::PlayerController (pc) is null in BeginPlay"));
		return;
	}

	// 애니메이션 시퀀스가 제대로 로드되었는지 확인 (1개씩 적용)
	/*if (TestAnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() - TestAnimSequence loaded successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay() - TestAnimSequence is not set"));
	}*/
	
	// 애니메이션 시퀀스 배열이 제대로 설정되었는지 확인
	if (AnimSequences.Num() == 8)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() - AnimSequences initialized with 8 elements"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay() - AnimSequences does not have 8 elements"));
	}

	// WBP_AimPoint 위젯 생성
	if (WBP_AimPoint)  // WBP_aimpoint가 올바르게 할당되어 있는지 확인
	{
		AimPointUI = CreateWidget<UCJS_AimPointWidget>(GetWorld(), WBP_AimPoint);
		if (AimPointUI) 
		{
			AimPointUI->AddToViewport();
			AimPointUI->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Warning, TEXT("AimPointUI successfully created and added to viewport & Hidden right now"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create AimPointUI Widget"));
		}
		UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::BeginPlay()::WBP_AimPoint is assigned!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_UserCharacter::BeginPlay()::WBP_AimPoint is not assigned! Please assign it in the Blueprint."));
	}
	
	bAimPointUIShowing = false;



	// HttpActor 초기화 시도
	//HttpActor = Cast<ACJS_HttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACJS_HttpActor::StaticClass()));
	HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
	if (HttpActor == nullptr)
	{
		// HttpActor를 찾지 못한 경우, 새로 생성
		FActorSpawnParameters SpawnParams;
		//HttpActor = GetWorld()->SpawnActor<ACJS_HttpActor>(ACJS_HttpActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		HttpActor = GetWorld()->SpawnActor<AHttpActor>(AHttpActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (HttpActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("HttpActor spawned successfully."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn HttpActor."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HttpActor found and cast successfully."));
	}
	
	/*USessionGameInstance* sgi = Cast<USessionGameInstance>(GetGameInstance());
	sgi->AssignSessionNameFromPlayerState();*/


	// 물리 시뮬레이션 활성화
	if (GetMesh())
	{
		// 물리 시뮬레이션 및 중력 활성화
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetEnableGravity(true);
		
		// 축 고정 해제
		/*GetMesh()->BodyInstance.bLockXRotation = false;
		GetMesh()->BodyInstance.bLockYRotation = false;
		GetMesh()->BodyInstance.bLockZRotation = false;
		GetMesh()->BodyInstance.bLockXTranslation = false;
		GetMesh()->BodyInstance.bLockYTranslation = false;
		GetMesh()->BodyInstance.bLockZTranslation = false;*/

		// 움직임 멈춤을 위해 물리 속도 감쇠 적용
		GetMesh()->SetLinearDamping(3.0f); // 값이 높을수록 빠르게 감속
		GetMesh()->SetAngularDamping(3.0f); // 값이 높을수록 회전 감속이 빠름
	
	}

	// 기본 이동 힘 설정
	MoveForce = 150.0f;
}

// Called every frame
void ACJS_BallPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// 카메라 Yaw를 따라 캐릭터 Yaw 회전을 업데이트
	if (Controller)
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw = Controller->GetControlRotation().Yaw;
		SetActorRotation(NewRotation);
	}

	// 디버그 로그로 Direction 확인
	//UE_LOG(LogTemp, Warning, TEXT("Direction: X=%f, Y=%f, Z=%f"), Direction.X, Direction.Y, Direction.Z);

	// 방향에 힘을 적용하여 이동
	if (!Direction.IsNearlyZero())
	{
		FVector Force = Direction * MoveForce;
		//GetMesh()->AddForce(Force, NAME_None, true);
		GetMesh()->AddImpulse(Force, NAME_None, true);

		// 방향을 리셋
		Direction = FVector::ZeroVector;
	}
}


// Called to bind functionality to input  =========================================================================================================================================
void ACJS_BallPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetupPlayerInputComponent"));
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
	{
		// 이동
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACJS_BallPlayer::OnMyActionMove);
		// 방향
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ACJS_BallPlayer::OnMyActionLook);
		// 점프
		input->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionJump);
		// 던지기
		input->BindAction(IA_Throw, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionThrow);
		// 클릭
		input->BindAction(IA_Click, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionClick);
		// 토글 조준점
		input->BindAction(IA_AimPoint, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionToggleAimPointUI);
		// 숫자키 애니메이션 (인덱스 사용해 바인딩)
		for (int32 i = 0; i < 8; i++)
		{
			if (IA_NumKeys[i])
			{
				// 로그 출력 추가
				UE_LOG(LogTemp, Warning, TEXT("Binding action for Key %d with Index %d"), i, i);
				input->BindAction(IA_NumKeys[i], ETriggerEvent::Started, this, &ACJS_BallPlayer::OnNumberKeyPressed, i);
			}
			else
			{
				// 로그 출력: 인풋 액션이 null일 경우
				UE_LOG(LogTemp, Error, TEXT("IA_NumKeys[%d] is null"), i);
			}
		}
		// Log to check if input actions are bound
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetupPlayerInputComponent() - Input actions are bound"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::SetupPlayerInputComponent():: EnhancedInputComponent is null"));
	}
}

void ACJS_BallPlayer::UpdateMaterialRotation(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::UpdateMaterialRotation()"));

	if (!DynamicMaterialInstance) return;

	// 목표 회전 쿼터니언 설정
	FQuat TargetRotationQuat = FQuat(FRotator(0.0f, TargetYaw, 0.0f));
	//UE_LOG(LogTemp, Warning, TEXT("TargetYaw: %f, TargetRotationQuat: X=%f, Y=%f, Z=%f, W=%f"), TargetYaw, TargetRotationQuat.X, TargetRotationQuat.Y, TargetRotationQuat.Z, TargetRotationQuat.W);

	// 현재 회전 쿼터니언을 목표 회전값으로 보간하여 점진적으로 회전
	MaterialRotationQuat = FQuat::Slerp(MaterialRotationQuat, TargetRotationQuat, DeltaTime * 5.0f); // 회전 속도 조절
	//UE_LOG(LogTemp, Warning, TEXT("MaterialRotationQuat after Slerp: X=%f, Y=%f, Z=%f, W=%f"), MaterialRotationQuat.X, MaterialRotationQuat.Y, MaterialRotationQuat.Z, MaterialRotationQuat.W);

	// 머터리얼에 회전값 전달
	FRotator Rotation = MaterialRotationQuat.Rotator();
	//UE_LOG(LogTemp, Warning, TEXT("Rotation applied to material - Pitch: %f, Yaw: %f, Roll: %f"), Rotation.Pitch, Rotation.Yaw, Rotation.Roll);

	DynamicMaterialInstance->SetScalarParameterValue(FName("RotationAngleX"), Rotation.Pitch);
	DynamicMaterialInstance->SetScalarParameterValue(FName("RotationAngleY"), Rotation.Yaw);
	DynamicMaterialInstance->SetScalarParameterValue(FName("RotationAngleZ"), Rotation.Roll);

	float SetPitchRotation, SetYawRotation, SetRollRotation;
	DynamicMaterialInstance->GetScalarParameterValue(FName("RotationAngleX"), SetPitchRotation);
	DynamicMaterialInstance->GetScalarParameterValue(FName("RotationAngleY"), SetYawRotation);
	DynamicMaterialInstance->GetScalarParameterValue(FName("RotationAngleZ"), SetRollRotation);
	//UE_LOG(LogTemp, Warning, TEXT("After Set Rotation to material - Pitch: %f, Yaw: %f, Roll: %f"), SetPitchRotation, SetYawRotation, SetRollRotation);

}

void ACJS_BallPlayer::OnMyActionMove(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionMove()"));

	FVector2D v = Value.Get<FVector2D>();

	// 카메라의 Yaw를 기준으로 방향을 변환
	FRotator CameraYawRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::X) * v.X;
	FVector RightDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::Y) * v.Y;

	// 이동 방향을 카메라 기준으로 설정
	Direction = (ForwardDirection + RightDirection).GetSafeNormal();

	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionMove():: Move Direction: X=%f, Y=%f"), Direction.X, Direction.Y);
}
void ACJS_BallPlayer::OnMyActionLook(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionLook()"));

	FVector2D v = Value.Get<FVector2D>();

	// Log to check if the look input is being received
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionLook():: Look Direction: X=%f, Y=%f"), v.X, v.Y);

	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);
}
void ACJS_BallPlayer::OnMyActionJump(const FInputActionValue& Value)
{
	Jump();
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionJump() - Jump Triggered"));
}
void ACJS_BallPlayer::OnMyActionThrow(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionThrow()"));
	//if (HeartItemFactory)
	//{
	//	// Get the spawn location and rotation from the actor
	//	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * HeartSpawnPosition.X + GetActorUpVector() * HeartSpawnPosition.Z;
	//	FRotator SpawnRotation = GetActorRotation();

	//	// Spawn the heart actor
	//	FActorSpawnParameters SpawnParams;
	//	SpawnParams.Owner = this;
	//	ACJS_HeartActor* SpawnedHeart = GetWorld()->SpawnActor<ACJS_HeartActor>(HeartItemFactory, SpawnLocation, SpawnRotation, SpawnParams);

	//	if (SpawnedHeart && SpawnedHeart->ProjectileMovementComp)
	//	{
	//		// Apply an initial impulse to make the heart fly forward
	//		FVector LaunchDirection = SpawnRotation.Vector();
	//		SpawnedHeart->ProjectileMovementComp->Velocity = LaunchDirection * SpawnedHeart->ProjectileMovementComp->InitialSpeed;
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::OnMyActionThrow() - HeartItemFactory is null"));
	//}

	if (HasAuthority())
	{
		// 서버인 경우 바로 멀티캐스트 실행
		MulticastRPC_ThrowHeart();
	}
	else
	{
		// 클라이언트인 경우 서버에 요청
		ServerRPC_ThrowHeart();
	}

}

void ACJS_BallPlayer::OnMyActionClick(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionClick()"));

	if (!bAimPointUIShowing)
	{
		// AimPointUI가 표시되지 않았을 때는 클릭을 무시
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionClick()::bAimPointUIShowing==false return"));
		return;
	}

	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 100000.0f;
	FHitResult Outhit;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Outhit, Start, End, TraceChannel, CollisionParams);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit something!"));

		if (Outhit.Component.IsValid())
		{
			FString HitComponentName = Outhit.Component->GetName();
			UE_LOG(LogTemp, Warning, TEXT("Hit Component: %s"), *HitComponentName);
		}

		AActor* HitActor = Outhit.GetActor();
		if (HitActor)
		{
			FString HitActorName = HitActor->GetName();
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActorName);

			if (HitActorName.Contains("MultiRoom"))
			{
				UE_LOG(LogTemp, Warning, TEXT("BP_CJS_MultiRoom Clicked"));
				if (PC)
				{				
					//RequestMoveMultiRoom(PC);

					// GameInstance에서 MySessionName 값을 가져옴
					FString UserId;
					int32 ActorIndex;
					FString RoomOwner;
					FString RoomNum;
					USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
					if (GameInstance)
					{
						UserId = GameInstance->MySessionName;
						UE_LOG(LogTemp, Warning, TEXT("Assigned UserId from MySessionName: %s"), *UserId);
					}
					
					// MultiRoomActors의 인덱스를 찾기
					ActorIndex = MultiRoomActors.IndexOfByKey(HitActor);
					if (ActorIndex != INDEX_NONE && AllUsersArray.IsValidIndex(ActorIndex))
					{
						// AllUsersArray에서 UserObject를 가져와 정보 추출
						TSharedPtr<FJsonObject> UserObject = AllUsersArray[ActorIndex]->AsObject();
						if (UserObject.IsValid())
						{
							RoomOwner = UserObject->GetStringField(TEXT("UserId"));
							//RoomNum = UserObject->GetStringField(TEXT("RoomNum"));
							RoomNum = "3";
							//UE_LOG(LogTemp, Warning, TEXT("MultiRoomActor Owner UserId: %s, RoomNum: %s"), *RoomOwner, *RoomNum);
							UE_LOG(LogTemp, Warning, TEXT("MultiRoomActor RoomOwner: %s, RoomNum: %s"), *RoomOwner, *RoomNum);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UserObject from AllUsersArray"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Could not find the MultiRoomActor in MultiRoomActors or invalid index in AllUsersArray"));
					}

					// 사용자 데이터를 맵에 추가
					TMap<FString, FString> MultiRoomData;

					//MyRoomData.Add("userId", UserId);
					//MultiRoomData.Add("userId", UserId);
					//MultiRoomData.Add("room_num", "3");

					MultiRoomData.Add("room_num", RoomNum);
					
					// JSON 형식으로 변환
					FString json = UJsonParseLib::MakeJson(MultiRoomData);

					// 로그 출력 (디버깅용)
					UE_LOG(LogTemp, Warning, TEXT("MakeJson() Ok!!!!"));
					UE_LOG(LogTemp, Warning, TEXT("userId: %s, room_num: %s"), *UserId, *RoomNum);
					UE_LOG(LogTemp, Warning, TEXT("json: %s"), *json);

					HttpActor->ReqPostClickMultiRoom(MultiRoomURL, json);
					UE_LOG(LogTemp, Warning, TEXT("----------  post reqeust done"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr. Cannt Move to the MultiRoomMap"));
				}
			}
			else if (HitActorName.Contains("MyRoom"))
			{
				UE_LOG(LogTemp, Warning, TEXT("BP_CJS_MyRoom Clicked"));
				if (PC)
				{
					//JS ReWrite 이쪽에 방 데이터 송수신 하는 부분 넣고 수신 하는 부분에서 방 이동
					FString UserId;
					USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
					if (GameInstance)
					{
						//UserId = GameInstance->MySessionName;
						UserId = "testuser";
						UE_LOG(LogTemp, Warning, TEXT("Assigned UserId from MySessionName: %s"), *UserId);
					}

					// 사용자 데이터를 맵에 추가
					TMap<FString, FString> MyRoomData;
					MyRoomData.Add("userId", UserId);

					// JSON 형식으로 변환
					FString JsonRequest = UJsonParseLib::MakeJson(MyRoomData);

					// 로그 출력 (디버깅용)
					UE_LOG(LogTemp, Warning, TEXT("userId: %s"), *UserId);
					UE_LOG(LogTemp, Warning, TEXT("Json Request: %s"), *JsonRequest);

					// 서버로 요청 전송
					HttpActor->ReqPostClickMyRoom(HttpActor->MyRoomURL, JsonRequest);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr. Cannt Move to the MyRoomMap"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionClick()::Hit Actor is NULL"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionClick()::No Hit Detected"));
	}

}

void ACJS_BallPlayer::OnMyActionToggleAimPointUI(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionToggleAimPointUI()"));

	if (AimPointUI)
	{
		if (!bAimPointUIShowing)
		{
			bAimPointUIShowing = true;
			AimPointUI->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Warning, TEXT("AimPointUI is now Visible, Visibility State: %d"), (int32)AimPointUI->GetVisibility());
		}
		else
		{
			bAimPointUIShowing = false;
			AimPointUI->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Warning, TEXT("AimPointUI is now Hidden, Visibility State: %d"), (int32)AimPointUI->GetVisibility());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AimPointUI is null! UI cannot be toggled."));
	}
}


void ACJS_BallPlayer::OnNumberKeyPressed(const FInputActionValue& Value, int32 KeyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnNumberKeyPressed() - Key %d pressed"), KeyIndex + 1);
	//PlayAnimationByIndex(KeyIndex);

	// 서버에 애니메이션 재생을 요청합니다.
	if (HasAuthority())
	{
		MulticastRPC_PlayAnimation(KeyIndex); // 서버는 직접 멀티캐스트 실행
	}
	else
	{
		ServerRPC_PlayAnimation(KeyIndex); // 클라이언트는 서버에 요청
	}
}
void ACJS_BallPlayer::PlayAnimationByIndex(int32 Index)
{
	// 유효한 애니메이션 인덱스와 Mesh 여부 확인
	if (AnimSequences.IsValidIndex(Index) && AnimSequences[Index] && GetMesh())
	{
		// 물리적 속도 및 회전 초기화 (움직임을 멈추기 위함)
		GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);   // 선형 속도 초기화
		GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector); // 각속도 초기화

		// Actor와 메시의 전체 회전을 (0, 0, 0)으로 초기화
		SetActorRotation(FRotator::ZeroRotator);

		// 메시의 로컬 회전을 (0, 0, 0)으로 설정하여 Actor와 메시 회전을 동기화 (Teleport 플래그 사용)
		/*keletalMeshComponent가 물리 시뮬레이션을 활성화한 상태에서 회전이나 위치를 강제로 설정하려면, Teleport 플래그를 사용해야 함.
		  ETeleportType::TeleportPhysics 플래그는 물리 시뮬레이션이 활성화된 메시에도 강제로 회전을 적용하도록 합*/
		if (GetMesh())
		{
			GetMesh()->SetRelativeRotation(FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
		}

		// 메시 컴포넌트의 실제 회전 값 가져오기
		//FRotator MeshRotation = GetMesh()->GetComponentRotation();
		// 메시의 Pitch, Yaw, Roll 각도 출력
		//UE_LOG(LogTemp, Warning, TEXT("Mesh Rotation - Pitch: %f, Yaw: %f, Roll: %f"), MeshRotation.Pitch, MeshRotation.Yaw, MeshRotation.Roll);

		// 볼 플레이어의 현재 회전 각도 가져오기
		//FRotator PlayerRotation = GetActorRotation();
		// 로그로 볼 플레이어의 Pitch, Yaw, Roll 각도 출력
		//UE_LOG(LogTemp, Warning, TEXT("Player Rotation - Pitch: %f, Yaw: %f, Roll: %f"), PlayerRotation.Pitch, PlayerRotation.Yaw, PlayerRotation.Roll);


		// 애니메이션 재생
		GetMesh()->PlayAnimation(AnimSequences[Index], false);
		//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::PlayAnimationByIndex() - Animation %d played"), Index + 1);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::PlayAnimationByIndex() - Invalid index or animation sequence"));
	}
}


// 공끼리 충돌 시  =========================================================================================================================================
void ACJS_BallPlayer::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Check if the hit actor is another BallPlayer
	if (Other && Other != this && Other->IsA(ACJS_BallPlayer::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::NotifyHit() - Collided with another BallPlayer"));

		// Activate particle effect at the hit location
		if (HitVFXFactory)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitVFXFactory, HitLocation);
		}
		if (HitSFX)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), HitSFX);
		}
		TriggerSelfHitEffects(HitLocation);
	}
}
void ACJS_BallPlayer::TriggerSelfHitEffects(FVector HitLocation)
{
	// Log to indicate collision effects triggered
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::TriggerCollisionEffects() - Triggered on collision"));

	// Activate particle effect at the hit location
	if (SelfHitVFXFactory)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelfHitVFXFactory, HitLocation);
	}
	//// Play sound effect
	//if (HitSFX)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSFX, HitLocation);
	//}
}

// 숫자키 애니메이션 작동 (멀티)
void ACJS_BallPlayer::ServerRPC_PlayAnimation_Implementation(int32 AnimationIndex)
{
	// 서버에서 멀티캐스트 호출
	MulticastRPC_PlayAnimation(AnimationIndex);
}

bool ACJS_BallPlayer::ServerRPC_PlayAnimation_Validate(int32 AnimationIndex)
{
	return true;
}

void ACJS_BallPlayer::MulticastRPC_PlayAnimation_Implementation(int32 AnimationIndex)
{
	PlayAnimationByIndex(AnimationIndex);
}


// 하트 던지기 (멀티)
void ACJS_BallPlayer::ServerRPC_ThrowHeart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::ServerRPC_ThrowHeart()"));
	// 서버에서만 하트 생성
	if (HasAuthority())
	{
		MulticastRPC_ThrowHeart(); // 모든 클라이언트에게 하트를 던지라고 브로드캐스트
	}
}

bool ACJS_BallPlayer::ServerRPC_ThrowHeart_Validate()
{
	return true;
}

void ACJS_BallPlayer::MulticastRPC_ThrowHeart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::MulticastRPC_ThrowHeart()"));
	// 실제 하트를 던지는 로직 (기존의 OnMyActionThrow 로직을 여기로 옮기기)
	if (HeartItemFactory)
	{
		// 일정한 방향으로 던지기
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * HeartSpawnPosition.X + GetActorUpVector() * HeartSpawnPosition.Z;
		FRotator SpawnRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		ACJS_HeartActor* SpawnedHeart = GetWorld()->SpawnActor<ACJS_HeartActor>(HeartItemFactory, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedHeart && SpawnedHeart->ProjectileMovementComp)
		{
			FVector LaunchDirection = SpawnRotation.Vector();
			SpawnedHeart->ProjectileMovementComp->Velocity = LaunchDirection * SpawnedHeart->ProjectileMovementComp->InitialSpeed;
		}	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::Multicast_ThrowHeart() - HeartItemFactory is null"));
	}
}

 
// 멀티방 이동
void ACJS_BallPlayer::RequestMoveMultiRoom(APlayerController* RequestingPC)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::RequestMoveMultiRoom()"));

	if (PC && !HasAuthority())  // 클라이언트만 요청
	{
		UE_LOG(LogTemp, Warning, TEXT("Requesting Server to move to MultiRoom"));
		ServerRPC_RequestMoveMultiRoom(PC);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Authority or PC is not valid"));
	}
}
void ACJS_BallPlayer::ServerRPC_RequestMoveMultiRoom_Implementation(APlayerController* RequestingPC)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::ServerRPC_RequestMoveMultiRoom_Implementation()"));

	if (RequestingPC)
	{
		APawn* ControlledPawn = RequestingPC->GetPawn();
		if (ControlledPawn)
		{
			// 클릭한 클라이언트의 캐릭터만 이동합니다.
			FVector NewLocation(9950.0f, 0.0f, 0.0f); // 이동하고 싶은 위치 지정
			ControlledPawn->SetActorLocation(NewLocation);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ControlledPawn is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::ServerRPC_RequestMoveMultiRoom_Implementation():: NO RequestingPC"));
	}
	
}

// 로비 진입 시, 캐릭터 초기 설정 ================================================================================================
void ACJS_BallPlayer::InitializeFromJson(const FString& LocalJsonData)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::InitializeFromJson()"));
	// JSON 문자열을 JSON 객체로 파싱
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(LocalJsonData);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// 1. RGB 값 추출 및 SetInitColorValue 호출
		float R = JsonObject->GetNumberField(TEXT("R"));
		float G = JsonObject->GetNumberField(TEXT("G"));
		float B = JsonObject->GetNumberField(TEXT("B"));
		SetInitColorValue(R, G, B);

		// 2. 월드에 배치된 5개의 MultiRoomActor를 찾고 저장 <-- 기존
		//TArray<AActor*> FoundActors;
		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACJS_MultiRoomActor::StaticClass(), FoundActors);

		//// 최대 5개만 저장
		//for (int32 i = 0; i < FoundActors.Num() && i < 5; i++)
		//{
		//	ACJS_MultiRoomActor* MultiRoomActor = Cast<ACJS_MultiRoomActor>(FoundActors[i]);
		//	if (MultiRoomActor)
		//	{
		//		MultiRoomActors.Add(MultiRoomActor);
		//	}
		//}

		// 2. 20개의 MultiRoomActor 생성 및 저장
		TSet<FVector> SpawnedLocations; // 중복 위치 체크를 위한 Set

		for (int32 i = 0; i < 20; i++)
		{
			FVector SpawnLocation;
			bool bValidLocation = false;

			// 최대 10회 반복하여 유효한 위치 찾기
			for (int32 j = 0; j < 10; j++)
			{
				// 랜덤 위치 생성
				float X = FMath::RandRange(-15000.0f, 15000.0f);
				float Y = FMath::RandRange(-15000.0f, 15000.0f);
				SpawnLocation = FVector(X, Y, 400.0f);

				// 간격 체크
				bool bIsOverlapping = false;
				for (const FVector& ExistingLocation : SpawnedLocations)
				{
					if (FVector::Dist(ExistingLocation, SpawnLocation) < 500.0f) // 800 이하일 경우 겹친다고 판단
					{
						bIsOverlapping = true;
						break;
					}
				}

				// 간섭이 없으면 위치를 등록하고 루프 종료
				if (!bIsOverlapping)
				{
					bValidLocation = true;
					SpawnedLocations.Add(SpawnLocation);
					break;
				}
			}

			// 유효한 위치가 발견되면 Actor 생성
			if (bValidLocation)
			{
				FRotator SpawnRotation = FRotator::ZeroRotator;
				UE_LOG(LogTemp, Warning, TEXT("Trying to spawn MultiRoomActor %d at location: %s"), i, *SpawnLocation.ToString());
				//ACJS_MultiRoomActor* NewMultiRoomActor = GetWorld()->SpawnActor<ACJS_MultiRoomActor>(ACJS_MultiRoomActor::StaticClass(), SpawnLocation, SpawnRotation);
				ACJS_MultiRoomActor* NewMultiRoomActor = GetWorld()->SpawnActor<ACJS_MultiRoomActor>(RefMultiRoom, SpawnLocation, SpawnRotation);
				if (NewMultiRoomActor)
				{
					MultiRoomActors.Add(NewMultiRoomActor);
					UE_LOG(LogTemp, Warning, TEXT("Successfully spawned MultiRoomActor %d at %s"), i, *SpawnLocation.ToString());
					// 추가적인 상태 확인
					UE_LOG(LogTemp, Warning, TEXT("MultiRoomActor %d is located at %s, Visibility: %s"), i, *NewMultiRoomActor->GetActorLocation().ToString(), NewMultiRoomActor->IsHidden() ? TEXT("Hidden") : TEXT("Visible"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to spawn MultiRoomActor %d at %s"), i, *SpawnLocation.ToString());
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to find valid spawn location for MultiRoomActor %d"), i);
			}
		}

		// 저장된 MultiRoomActor의 개수 출력
		UE_LOG(LogTemp, Warning, TEXT("Found %d MultiRoomActors in the world."), MultiRoomActors.Num());


		// 3. SimilarUsers 및 OppositeUsers 배열 추출 및 저장                            <-------------- 수정 필요 (소유자의 UserId, RoomNum 같이 저장 필요)
		TArray<TSharedPtr<FJsonValue>> SimilarUsersArray = JsonObject->GetArrayField(TEXT("SimilarUsers"));
		TArray<TSharedPtr<FJsonValue>> OppositeUsersArray = JsonObject->GetArrayField(TEXT("OppositeUsers"));

		//TArray<TSharedPtr<FJsonValue>> AllUsersArray;
		AllUsersArray.Append(SimilarUsersArray);
		AllUsersArray.Append(OppositeUsersArray);

		// 최대 5개의 방 정보를 저장하고, MultiRoomActor에 설정
		for (int32 i = 0; i < AllUsersArray.Num() && i < MultiRoomActors.Num(); i++)
		{
			TSharedPtr<FJsonObject> UserObject = AllUsersArray[i]->AsObject();
			if (UserObject.IsValid())
			{
				// EmotionScore와 RoomName을 가져와 설정
				FString Message = UserObject->GetStringField(TEXT("Message"));
				FString RoomName = UserObject->GetStringField(TEXT("RoomName"));

				// 현재 사용자 수와 최대 수 설정 (예시)
				int32 CurNumPlayer = FMath::RandRange(0, 5); // 예시로 랜덤 설정
				int32 MaxNumPlayer = 5;
				//float Percent = (Message / 500.0f) * 100.0f; // Percent 계산 (예시로 500.0을 기준으로)

				// 각 MultiRoomActor에 정보 설정
				SetInitMultiRoomInfo(MultiRoomActors[i], CurNumPlayer, MaxNumPlayer, RoomName, Message);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON data."));
	}
}

void ACJS_BallPlayer::SetInitColorValue(float r, float g, float b) // 색상
{
	InitColorValue = FLinearColor(r, g, b);
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetInitColorValue() - Initial Color set to R: %f, G: %f, B: %f"), r, g, b);
}

//void ACJS_BallPlayer::SetInitMultiRoomInfo(int32 CurNumPlayer, int32 MaxNumPlayer, const FString& RoomName, float Percent)
//{
//	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetInitMultiRoomInfo()"));
//
//	// 월드에서 MultiRoomActor 클래스의 인스턴스를 찾습니다.   ---> 1개
//	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACJS_MultiRoomActor::StaticClass());
//	if (FoundActor)
//	{
//		// ACJS_MultiRoomActor로 캐스팅
//		RefMultiRoom = Cast<ACJS_MultiRoomActor>(FoundActor);
//		if (RefMultiRoom)
//		{
//			// 초기 설정 함수 호출
//			RefMultiRoom->InitRefRoomInfoWidget(CurNumPlayer, MaxNumPlayer, RoomName, Percent);
//			UE_LOG(LogTemp, Warning, TEXT("MultiRoom information initialized successfully."));
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("Failed to cast FoundActor to ACJS_MultiRoomActor."));
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("No ACJS_MultiRoomActor found in the world."));
//	}
//}

void ACJS_BallPlayer::SetInitMultiRoomInfo(ACJS_MultiRoomActor* MultiRoomActor, int32 CurNumPlayer, int32 MaxNumPlayer, const FString& RoomName, const FString& Percent)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetInitMultiRoomInfo()"));
	if (MultiRoomActor)
	{
		MultiRoomActor->InitRefRoomInfoWidget(CurNumPlayer, MaxNumPlayer, RoomName, Percent);
		UE_LOG(LogTemp, Warning, TEXT("MultiRoom information initialized for Room: %s, Percent: %s"), *RoomName, *Percent);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid MultiRoomActor provided."));
	}
}

void ACJS_BallPlayer::InitJsonData(FString LocalJsonData)
{
	JsonData = LocalJsonData;
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::InitJsonData()::JsonData initialized with value: %s"), *JsonData);
}

void ACJS_BallPlayer::ExecuteWallPaperPython()
{
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

