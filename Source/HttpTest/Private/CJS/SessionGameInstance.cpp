// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/SessionGameInstance.h"
#include "../HttpTest.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

#include "GenericPlatform/GenericPlatformMisc.h"
#include "Kismet/GameplayStatics.h"
#include "string"



void USessionGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::Init()"));

	IOnlineSubsystem* subSystem = IOnlineSubsystem::Get();

	if (subSystem)
	{
		PRINTLOG(TEXT("OnlineSubsystem: %s"), *subSystem->GetSubsystemName().ToString());
		SessionInterface = subSystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			PRINTLOG(TEXT("SessionInterface is valid"));
		}
		else
		{
			PRINTLOG(TEXT("SessionInterface is invalid"));
		}
	}
	else
	{
		PRINTLOG(TEXT("No OnlineSubsystem found"));
	}

	if(subSystem)
	{
		// 방 생성 요청 -> 응답
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USessionGameInstance::OnCreateSessionComplete);

		// 방 찾기 요청 -> 응답
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USessionGameInstance::OnMyFindSessionCompleteDelegate);

		// 방 조인 요청 -> 응답
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USessionGameInstance::OnMyJoinSessionComplete);
	}

	GEngine->OnNetworkFailure().AddUObject(this, &USessionGameInstance::OnNetworkFailure);
}

// 방 생성 요청
void USessionGameInstance::CreateMySession()
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::CreateMySession()"));
	PRINTLOG(TEXT("CreateMySession()"));

	FOnlineSessionSettings settings;

	// 1. 전용 서버를 사용하는가?
	settings.bIsDedicated = false;

	// 2. 랜선(Lan)으로 매치하는가?
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	settings.bIsLANMatch = subsysName == "NULL";

	// 3. 매칭이 공개(true)냐, 비공개(false)냐?
	settings.bShouldAdvertise = true;

	// 4. 유저의 상태 정보(온라인/자리비움/ 등등) 사용 여부
	settings.bUsesPresence = true;

	// 5. 중간에 난입 가능한가?
	settings.bAllowJoinViaPresence = true;
	settings.bAllowJoinInProgress = true;

	settings.bAllowJoinViaPresenceFriendsOnly = false;
	settings.bAntiCheatProtected = false;
	settings.bUseLobbiesIfAvailable = true;

	// 6. 최대 몇명?
	settings.NumPublicConnections = 100;

	// 7. 커스텀 정보
	//settings.Set(FName("ROOM_NAME"), sessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	settings.Set(FName("HOST_NAME"), MySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	PRINTLOG(TEXT("Attempting to create session with name: %s"), *MySessionName);
	PRINTLOG(TEXT("Create Session Start hostName : %s"), *MySessionName);

	//FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	SessionInterface->CreateSession(0, FName(MySessionName), settings);
}

// 방 생성 응답
void USessionGameInstance::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::OnCreateSessionComplete()"));

	if (bWasSuccessful)
	{
		PRINTLOG(TEXT("OnCreateSessionComplete is Successes"));
		PRINTLOG(TEXT("Session created successfully with name: %s"), *sessionName.ToString());
		GetWorld()->ServerTravel(TEXT("/Game/CJS/Maps/LobbyMap?listen"));
	}
	else
	{
		PRINTLOG(TEXT("Failed to create session with name: %s"), *sessionName.ToString());
		PRINTLOG(TEXT("OnCreateSessionComplete is Failed"));
	}
}


void USessionGameInstance::FindSessions()
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::FindSessions()"));
	PRINTLOG(TEXT("SessionInterface valid: %s"), SessionInterface.IsValid() ? TEXT("True") : TEXT("False"));

	/*if (GetWorld()->GetNetMode() == NM_ListenServer)
	{
		PRINTLOG(TEXT("FindSessions: Skipping on server"));
		return;
	}

	if (!SessionInterface.IsValid())
	{
		PRINTLOG(TEXT("FindSessions: SessionInterface is invalid. Cannot find sessions."));
		return;
	}*/

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (!SessionSearch.IsValid())
	{
		PRINTLOG(TEXT("FindSessions: Failed to create SessionSearch!"));
		return;
	}

	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(FName("HOST_NAME"), MySessionName, EOnlineComparisonOp::Equals);
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 40;


	if (!SessionInterface->FindSessions(0, SessionSearch.ToSharedRef()))
	{
		PRINTLOG(TEXT("FindSessions: Failed to start FindSessions!"));
	}
	else
	{
		PRINTLOG(TEXT("FindSessions: Successfully started FindSessions"));
	}

	//PRINTLOG(TEXT("Searching for sessions with name: %s, IsLAN: %s"), *MySessionName, SessionSearch->bIsLanQuery ? TEXT("True") :
}

void USessionGameInstance::OnMyFindSessionCompleteDelegate(bool bWasSuccessful)
{
	/*UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::OnMyFindSessionCompleteDelegate()"));
	if (GetWorld()->GetNetMode() == NM_ListenServer || GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		PRINTLOG(TEXT("OnMyFindSessionsCompleteDelegates: Skipping on server"));
		return;
	}

	if (!SessionInterface.IsValid() || !SessionSearch.IsValid())
	{
		PRINTLOG(TEXT("OnMyFindSessionsCompleteDelegates: SessionInterface or SessionSearch is invalid!"));
		return;
	}*/

	if (bWasSuccessful)
	{
		TArray<FOnlineSessionSearchResult> results = SessionSearch->SearchResults;  // 결과가 담겨있음
		PRINTLOG(TEXT("Found %d sessions"), results.Num());
		bool bJoinSession = false;

		for (int32 i = 0; results.Num(); i++)
		{
			FOnlineSessionSearchResult ret = results[i];
			if (false == results[i].IsValid())
			{
				continue;
			}

			FRoomInfo roomInfo;
			roomInfo.index = i;

			FString HostName;
			results[i].Session.SessionSettings.Get(FName("HOST_NAME"), HostName);
			PRINTLOG(TEXT("Session %d: HostName=%s"), i, *HostName);

			if (HostName == MySessionName)
			{
				PRINTLOG(TEXT("Join Session Call"));
				JoinSession(roomInfo.index);
				bJoinSession = true;
				break;
			}
		}

		if (!bJoinSession)
		{
			CreateMySession();
		}
	}
	else
	{
		PRINTLOG(TEXT("UNetTPSGameInstance::OnMyFindSessionCompleteDelegate()::bWasSuccessful is false"));
	}
}

void USessionGameInstance::JoinSession(int32 index)
{
	// SessionSearch
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::JoinSession()"));
	PRINTLOG(TEXT("Join Session Called"));

	if (!SessionSearch.IsValid() || SessionSearch->SearchResults.Num() <= index)
	{
		PRINTLOG(TEXT("JoinSession failed: SessionSearch is invalid or no search results."));
		return;
	}

	auto result = SessionSearch->SearchResults[index];
	SessionInterface->JoinSession(0, FName(MySessionName), result);
}

void USessionGameInstance::OnMyJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type EOnJoinSessionCompleteResult)
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::OnMyJoinSessionComplete()"));
	PRINTLOG(TEXT("OnMyJoinSessionComplete"));

	if (EOnJoinSessionCompleteResult == EOnJoinSessionCompleteResult::Success)
	{
		// 서버가 있는 레벨로 여행을 떠나고 싶다.
		auto* pc = GetWorld()->GetFirstPlayerController();

		FString url;
		if (SessionInterface->GetResolvedConnectString(SessionName, url))
		{
			PRINTLOG(TEXT("Travelling to session URL: %s"), *url);
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
		else
		{
			PRINTLOG(TEXT("Failed to get resolved connect string for session."));
		}
	}
	else
	{
		PRINTLOG(TEXT("JoinSession failed with result: %d"), static_cast<int32>(EOnJoinSessionCompleteResult));
	}
}

void USessionGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::OnNetworkFailure()"));
	PRINTLOG(TEXT("Network Failure: %s"), *ErrorString);

	if (FailureType == ENetworkFailure::FailureReceived)
	{
		PRINTLOG(TEXT("Host closed connection, returning to Exit Map"));
		//UGameplayStatics::OpenLevel(this, FName("/Game/ArtProject/LHM/Maps/LHM_Exit"));
	}
}
