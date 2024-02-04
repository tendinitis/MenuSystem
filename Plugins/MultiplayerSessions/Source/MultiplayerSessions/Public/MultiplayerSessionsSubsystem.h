// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MultiplayerSessionsSubsystem.generated.h"

/*
 * Defining our own dynamic multicast delegates for our Menu class to bind callbacks to
 * Next, go to public section of this header
 */
// Dynamic - delegate can be serialized and saved/loaded from within a Blueprint graph where they're called event dispatchers
// Multicast - multiple classes can bind their functions to this delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerSessionsSubsystemOnCreateSessionComplete, bool, bWasSuccessful);
// FOnlineSessionSearchResult is not a UCLASS (i.e. Blueprint-compatible), so delegate cannot be dynamic
// Non-dynamic delegates have a slight syntax difference, no comma inbetween the paramters
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerSessionsSubsystemOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerSessionsSubsystemOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerSessionsSubsystemOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerSessionsSubsystemOnStartSessionComplete, bool, bWasSuccessful);

/*
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMultiplayerSessionsSubsystem();

	/*
	 * To handle session functionality, the Menu class will call these functions
	 */ 
	void CreateSession(int32 NumPublicConnections, FString MatchType);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();

	/*
	 * Custom delegates continuation
	 * This is where we declare/create our own custom delegates for the Menu class to bind callbacks to
	 */
	FMultiplayerSessionsSubsystemOnCreateSessionComplete MultiplayerSessionsSubsystemOnCreateSessionComplete;
	FMultiplayerSessionsSubsystemOnFindSessionsComplete MultiplayerSessionsSubsystemOnFindSessionsComplete;
	FMultiplayerSessionsSubsystemOnJoinSessionComplete MultiplayerSessionsSubsystemOnJoinSessionComplete;
	FMultiplayerSessionsSubsystemOnDestroySessionComplete MultiplayerSessionsSubsystemOnDestroySessionComplete;
	FMultiplayerSessionsSubsystemOnStartSessionComplete MultiplayerSessionsSubsystemOnStartSessionComplete;

protected:
	/*
	 * Internal callbacks for the delegates we'll add to the Online Session Interface delegate list
	 * These don't need to be called outside this class
	 */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	
	/*
	 * To add to the Online Session Interface delegate list
	 * We'll bind our MultiplayerSessionsSubsystem internal callbacks to these
	 */
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

};
