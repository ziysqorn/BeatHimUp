// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "../../CustomGameInstance/MyGameInstance.h"
#include "../../CustomGameState/MainGameState.h"
#include "../../GameMode/MainGameMode/MainGameMode.h"
#include "../../Character/BaseCharacter/BaseCharacter.h"
#include "../../Interface/HaveAttributeSet.h"
#include "../../Interface/HaveHealthAttribute.h"
#include "../../Interface/HaveStaminaAttribute.h"
#include "../../Interface/HaveSpecialDeath.h"
#include "../../Interface/CanUseItem.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"

AMainController::AMainController()
{
	PlayerHUDComp = CreateDefaultSubobject<UPlayerHUDComponent>(FName("PlayerHUDComp"));
}

void AMainController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController()) {
		FInputModeGameOnly InputMode;
		SetShowMouseCursor(false);
		SetInputMode(InputMode);

		if (TargetLockPointWidgetActorSubclass) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			TargetLockPointWidgetActor = GetWorld()->SpawnActor<AActor>(TargetLockPointWidgetActorSubclass);
			if (IsValid(TargetLockPointWidgetActor)) {
				TargetLockPointWidgetActor->SetActorHiddenInGame(true);
			}
		}

		if (IsValid(PlayerCameraManager)) {
			PlayerCameraManager->ViewPitchMin = -50.0f;
			PlayerCameraManager->ViewPitchMax = 50.0f;
		}
	}
}

void AMainController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMainController::AcknowledgePossession(APawn* aPawn)
{
	Super::AcknowledgePossession(aPawn);

	if (IsValid(PlayerHUDComp)) {
		PlayerHUDComp->Client_AddHUD();
		if (UServiceControllerSubsystem* ServiceControllerSubsystem = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceControllerSubsystem->InGameController) {
				ServiceControllerSubsystem->InGameController->GetCharacterStats(FHttpRequestCompleteDelegate::CreateUObject(this, &AMainController::OnGetCharacterStatsComplete));
			}
		}
		if (AMainGameState* MainGS = GetWorld()->GetGameState<AMainGameState>()) {
			MainGS->OnRepMatchStatusDel.AddUObject(PlayerHUDComp, &UPlayerHUDComponent::DisplayMatchStatusMessage);
		}
		if (ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(aPawn)) {
			ControlledCharacter->OnLockTargetDel.AddUObject(this, &AMainController::SetWidgetToLockTarget);
		}
	}
}

void AMainController::UnbindControlledPawnHUD()
{
	if (IsValid(PlayerHUDComp)) {
		if (ICanUseItem* CanUseItem = Cast<ICanUseItem>(this->GetPawn())) {
			if (UItemComponent* ItemComp = CanUseItem->GetItemComponent()) {
				ItemComp->OnRepUsableItemListDel.RemoveAll(PlayerHUDComp);
				ItemComp->OnItemQuantityChangedDel.RemoveAll(PlayerHUDComp);
			}
		}
	}
}

void AMainController::Server_FetchCharacterStats_Implementation(const FString& JsonStr)
{
	TSharedPtr<FJsonObject> JsonObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonStr);
	if (FJsonSerializer::Deserialize(Reader, JsonObj)) {
		if (JsonObj.IsValid()) {
			if (IHaveAttributeSet* HaveAttributeSet = this->GetPawn<IHaveAttributeSet>()) {
				if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAttributeSet->GetAttributeSet())) {
					HaveHealthAttr->SetBaseMaxHealth(JsonObj->GetNumberField(TEXT("max_hp")));
					HaveHealthAttr->SetCurrentBaseHealth(JsonObj->GetNumberField(TEXT("hp")));
				}
				if (IHaveStaminaAttribute* HaveStaminaAttr = Cast<IHaveStaminaAttribute>(HaveAttributeSet->GetAttributeSet())) {
					HaveStaminaAttr->SetBaseMaxStamina(JsonObj->GetNumberField(TEXT("max_stamina")));
				}
			}
			if (ICanUseItem* CanUseItem = this->GetPawn<ICanUseItem>()) {
				if (UItemComponent* ItemComp = CanUseItem->GetItemComponent()) {
					if (UUsableItem* HealthPotion = Cast<UUsableItem>(ItemComp->GetItemByName(FName("HealthPotion")))) {
						HealthPotion->SetQuantity(JsonObj->GetNumberField(TEXT("health_potion_quant")));
					}
				}
			}
			if (IAbilitySystemInterface* ASI = this->GetPawn<IAbilitySystemInterface>()) {
				if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
					FString SavedState = JsonObj->GetStringField(TEXT("state"));
					if (SavedState == "State.Dead") {
						FGameplayTagContainer Container;
						Container.AddTag(FGameplayTag::RequestGameplayTag(FName(SavedState)));
						ASC->TryActivateAbilitiesByTag(Container);
					}
				}
			}
		}
	}
}

void AMainController::SaveCharacterStats(TSharedPtr<FJsonObject> JsonObj)
{
	if (JsonObj.IsValid()) {
		FString contentString;
		TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&contentString);
		if (FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWriter)) {
			if (UServiceControllerSubsystem* ServiceController = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
				if (ServiceController->InGameController) {
					ServiceController->InGameController->SaveCharacterStats(contentString, FHttpRequestCompleteDelegate());
				}
			}
		}
	}
}

void AMainController::Client_UpdateNewSpectatedPlayerHUD_Implementation(AActor* SpectatedActor)
{
	if (IsValid(PlayerHUDComp)) {
		PlayerHUDComp->HideHUD();
		if (IHaveAttributeSet* HaveAttributeSet = Cast<IHaveAttributeSet>(SpectatedActor)) {
			if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAttributeSet->GetAttributeSet())) {
				PlayerHUDComp->BindHealthProgress(HaveAttributeSet->GetAttributeSet(), FName("GetHealthPercentage"));
			}
			if (IHaveStaminaAttribute* HaveStaminaAttr = Cast<IHaveStaminaAttribute>(HaveAttributeSet->GetAttributeSet())) {
				PlayerHUDComp->BindStaminaProgress(HaveAttributeSet->GetAttributeSet(), FName("GetStaminaPercentage"));
			}
		}
		if (ICanUseItem* CanUseItem = Cast<ICanUseItem>(SpectatedActor)) {
			if (UItemComponent* ItemComp = CanUseItem->GetItemComponent()) {
				PlayerHUDComp->SetupItemFrameList(ItemComp->GetUsableItemList());
				ItemComp->OnRepUsableItemListDel.AddUObject(PlayerHUDComp, &UPlayerHUDComponent::SetupItemFrameList);
				ItemComp->OnItemQuantityChangedDel.AddUObject(this, &AMainController::Server_RequestViewTargetPawnToUpdateItemFrameQuantity);
			}
		}
		PlayerHUDComp->ShowHUD();
	}
}

void AMainController::Client_UnbindPreviousSpectatedPlayerHUD_Implementation(AActor* PreviousSpectactedPawn)
{
	if (IsValid(PlayerHUDComp)) {
		if (ICanUseItem* CanUseItem = Cast<ICanUseItem>(PreviousSpectactedPawn)) {
			if (UItemComponent* ItemComp = CanUseItem->GetItemComponent()) {
				ItemComp->OnRepUsableItemListDel.RemoveAll(PlayerHUDComp);
				ItemComp->OnItemQuantityChangedDel.RemoveAll(PlayerHUDComp);
			}
		}
	}
}

void AMainController::Server_RequestViewTargetPawnToUpdateItemFrameQuantity_Implementation(UUsableItem* Item)
{
	//Passing controller's view target because on the server, View Target is the character, not Player State.
	Client_UpdateHUDItemQuantity(Item, this->GetViewTarget());
}

void AMainController::Client_UpdateHUDItemQuantity_Implementation(UUsableItem* Item, AActor* CurrentViewTargetPawn)
{
	if (IsValid(PlayerHUDComp)) {
		PlayerHUDComp->Client_UpdateItemFrameQuantityByViewTarget(Item, CurrentViewTargetPawn);
	}
}

void AMainController::OnGetCharacterStatsComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (pResponse.IsValid()) {
			switch (pResponse->GetResponseCode()) {
			case EHttpResponseCodes::Ok:
			{
				if (!pResponse->GetContentAsString().IsEmpty()) {
					Server_FetchCharacterStats(pResponse->GetContentAsString());
				}
				break;
			}
			}
		}
	}
}

void AMainController::SetSpectatorPawn(ASpectatorPawn* NewSpectatorPawn)
{
	Super::SetSpectatorPawn(NewSpectatorPawn);

	if (IsLocalController()) {
		ServerViewNextPlayer();
	}
}

APlayerState* AMainController::GetNextViewablePlayer(int32 dir)
{
	UWorld* World = GetWorld();
	AGameModeBase* GameMode = World->GetAuthGameMode();
	AGameStateBase* GameState = World->GetGameState();

	// Can't continue unless we have the GameState and GameMode
	if (!GameState || !GameMode)
	{
		return nullptr;
	}

	APlayerState* NextPlayerState = (PlayerCameraManager ? PlayerCameraManager->ViewTarget.GetPlayerState() : nullptr);

	// If we don't have a NextPlayerState, use our own.
	// This will allow us to attempt to find another player to view or, if all else fails, makes sure we have a playerstate set for next time.
	int32 NextIndex = (NextPlayerState ? GameState->PlayerArray.Find(NextPlayerState) : GameState->PlayerArray.Find(PlayerState));

	//Check that NextIndex is a valid index, as Find() may return INDEX_NONE
	if (!GameState->PlayerArray.IsValidIndex(NextIndex))
	{
		return nullptr;
	}

	// Cycle through the player states until we find a valid one.
	for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		UE_LOG(LogTemp, Display, TEXT("Player Array Num: %d"), GameState->PlayerArray.Num());

		NextIndex = ((NextIndex == 0) && (dir < 0)) ? (GameState->PlayerArray.Num() - 1) : ((NextIndex == (GameState->PlayerArray.Num() - 1)) && (dir > 0)) ? 0 : NextIndex += dir;
		NextPlayerState = GameState->PlayerArray[NextIndex];

		// Make sure we're not trying to view our own player state.
		if (NextPlayerState != PlayerState)
		{
			UE_LOG(LogTemp, Display, TEXT("Next Player State: %s"), *NextPlayerState->GetName());

			AController* NextController = Cast<AController>(NextPlayerState->GetOwner());

			// Check they have a pawn & the game mode is ok with us spectating them.
			if (NextController && NextController->GetPawn() && GameMode->CanSpectate(this, NextPlayerState))
			{
				if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(NextController->GetPawn())) {
					if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
						if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) {
							break;
						}
					}
				}
			}
		}
	}

	// If we've failed to find another player to view, we'll be back to our original view target playerstate.
	return NextPlayerState;
}

void AMainController::SetViewTarget(AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
	Super::SetViewTarget(NewViewTarget, TransitionParams);

	if (HasAuthority()) {
		Client_UnbindPreviousSpectatedPlayerHUD(this->GetViewTarget());
		if (APlayerState* NewSpectatedPlayerState = Cast<APlayerState>(NewViewTarget)) {
			Client_UpdateNewSpectatedPlayerHUD(NewSpectatedPlayerState->GetPawn());
		}
	}
}

void AMainController::SwitchToSpectate()
{
	if (!HasAuthority())
	{
		UnbindControlledPawnHUD();
		return;
	}

	PlayerState->SetIsSpectator(true);
	ChangeState(NAME_Spectating);

	bPlayerIsWaiting = true;

	ClientGotoState(NAME_Spectating);
}

void AMainController::Client_TravelToMap_Implementation(FName MapName)
{
	if (IsValid(PlayerHUDComp)) {
		PlayerHUDComp->ShowLoadingScreen();
	}
	UGameplayStatics::OpenLevel(GetWorld(), MapName);
}

void AMainController::EndGameProcess(EMatchStatus inMatchStatus)
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		if (MyGameInstance->GetPlayerInfo().Username.IsEqual(MyGameInstance->GetLobbyInfo().Leader_Username)) {
			Server_RequestEndGame(inMatchStatus);
		}
	}
}

void AMainController::SetWidgetToLockTarget(AActor* Target)
{
	if (GetNetMode() == ENetMode::NM_DedicatedServer) return;
	if (IsValid(TargetLockPointWidgetActor)) {
		if (IsValid(Target))
		{
			if (ACharacter* TargetCharacter = Cast<ACharacter>(Target)) {
				TargetLockPointWidgetActor->SetActorHiddenInGame(false);
				TargetLockPointWidgetActor->AttachToComponent(TargetCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Socket_TargetLockPoint"));
			}
		}
		else
		{
			TargetLockPointWidgetActor->SetActorHiddenInGame(true);
			TargetLockPointWidgetActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void AMainController::PauseGame()
{
	if (IsValid(PlayerHUDComp)) {
		PlayerHUDComp->DisplayPauseUI();
	}
}

void AMainController::HandleAfterUIRemove()
{
	SetShowMouseCursor(false);
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AMainController::Server_RequestEndGame_Implementation(EMatchStatus inMatchStatus)
{
	if (AMainGameMode* MainGM = GetWorld()->GetAuthGameMode<AMainGameMode>()) {
		MainGM->EndMatch(inMatchStatus);
	}
}
