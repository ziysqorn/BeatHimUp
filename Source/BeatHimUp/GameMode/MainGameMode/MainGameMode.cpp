// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "../../Controller/MainController/MainController.h"
#include "../../PlayerState/MainPlayerState.h"
#include "../../CustomGameInstance/MyGameInstance.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"

void AMainGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FParse::Value(FCommandLine::Get(), TEXT("server_id="), Server_ID);
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AMainPlayerState* MainPlayerState = NewPlayer->GetPlayerState<AMainPlayerState>()) {
		MainPlayerState->Client_RequestPlayerInfo();
		MainPlayerState->Client_RequestLobbyInfo();
	}
}

void AMainGameMode::Logout(AController* Exiting)
{
	UE_LOG(LogTemp, Display, TEXT("A Player is exiting..."));

	if (GetNumPlayers() == 1) {
		if (UServiceControllerSubsystem* ServiceControllerSubsystem = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceControllerSubsystem->GameServerController) {
				UE_LOG(LogTemp, Display, TEXT("Current player count is reducing to 0, start requesting for server drop"));
				ServiceControllerSubsystem->GameServerController->DropGameServer(Server_ID, FHttpRequestCompleteDelegate::CreateUObject(this, &AMainGameMode::OnDropServerComplete));
			}
		}
	}

	Super::Logout(Exiting);
}

void AMainGameMode::OnDropServerComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
			if (pResponse.IsValid()) {
				switch (pResponse->GetResponseCode()) {
				case EHttpResponseCodes::Created:
					UE_LOG(LogTemp, Display, TEXT("Server starts exiting"));
					FGenericPlatformMisc::RequestExit(false);
					break;
				}
			}
		}
	}
}

void AMainGameMode::OnBossKilled()
{
	StartEndMatch(EMatchStatus::MATCH_WON);
}

void AMainGameMode::OnPlayerKilled()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
		if (Iterator->IsValid()) {
			if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Iterator->Get()->GetPawn())) {
				if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
					if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) {
						return;
					}
				}
			}
		}
	}
	StartEndMatch(EMatchStatus::MATCH_LOST);
}

void AMainGameMode::EndMatch(EMatchStatus inMatchStatus)
{
	switch (inMatchStatus) {
	case EMatchStatus::MATCH_WON:
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("CONGRATULATIONS ! YOU WON !!!!!!!!!"));
		break;
	case EMatchStatus::MATCH_LOST:
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SORRY ! YOU LOSE......."));
		break;
	default:
		break;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
		if (Iterator && Iterator->IsValid()) {
			if (AMainController* MainController = Cast<AMainController>(Iterator->Get())) {
				MainController->Client_TravelToMap(FName("Level_MainMenu"));
			}
		}
	}
}

void AMainGameMode::StartEndMatch(EMatchStatus inMatchStatus)
{
	if (OnMatchEndDel.IsBound()) {
		OnMatchEndDel.Broadcast(inMatchStatus);
	}
}
