// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Delegates/DelegateCombinations.h"
#include "SessionGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FRoomInfo
{
	GENERATED_BODY()

    // 방 이름
	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	// 호스트 이름
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	UPROPERTY(BlueprintReadOnly)
	// 최대 플레이어 수
	int32 maxPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	// 입장한 플레이어 수 (최대 - 남은)
	int32 currentPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	// 핑 정보
	int32 pingMS;

	int32 index;

	FString ToString()
	{
		return FString::Printf(TEXT("%d) [%s] [%s] %d / %d - ping : %dms"), index, *roomName, *hostName, currentPlayerCount, maxPlayerCount, pingMS);
	}
};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, const struct FRoomInfo&, info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionSearchComplete);

/**
 * 
 */
UCLASS()
class HTTPTEST_API USessionGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	// 온라인 세션 인터페이스를 기억하고 싶다 (비동기 방식)
	IOnlineSessionPtr SessionInterface;

	// 호스트 이름
	UPROPERTY()
	FString MySessionName = TEXT("User2");

	// 방 생성 요청
	UFUNCTION()
	void CreateMySession();
	// 방 생성 응답
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	// 찾을 방의 목록
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	// 방 찾기 요청
	//void FindOtherSessions();
	UFUNCTION()
	void FindSessions();
	// 방 찾기 응답
	void OnMyFindSessionCompleteDelegate(bool bWasSuccessful);

	// 방 조인 요청
	UFUNCTION()
	void JoinSession(int32 index);
	// 방 조인 응답	
	void OnMyJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type);

	// 네트워크 연결 실패 시
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

};

