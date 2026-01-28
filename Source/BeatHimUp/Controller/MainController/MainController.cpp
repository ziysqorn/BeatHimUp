// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "../../CustomGameInstance/MyGameInstance.h"
#include "../../CustomGameState/MainGameState.h"
#include "../../GameMode/MainGameMode/MainGameMode.h"
#include "../../Character/BaseCharacter/BaseCharacter.h"
#include "../../Interface/HaveAttributeSet.h"
#include "../../Interface/HaveHealthAttribute.h"
#include "../../Interface/HaveStaminaAttribute.h"
#include "../../Interface/CanUseItem.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"

AMainController::AMainController()
{
	PlayerHUDComp = CreateDefaultSubobject<UPlayerHUDComponent>(FName("PlayerHUDComp"));
}

void AMainController::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
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

		if (UServiceControllerSubsystem* ServiceControllerSubsystem = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceControllerSubsystem->InGameController) {
				ServiceControllerSubsystem->InGameController->GetCharacterStats(FHttpRequestCompleteDelegate::CreateUObject(this, &AMainController::OnGetCharacterStatsComplete));
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
		if (AMainGameState* MainGS = GetWorld()->GetGameState<AMainGameState>()) {
			MainGS->OnRepMatchStatusDel.AddUObject(PlayerHUDComp, &UPlayerHUDComponent::DisplayMatchStatusMessage);
		}
		if (ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(aPawn)) {
			ControlledCharacter->OnLockTargetDel.AddUObject(this, &AMainController::SetWidgetToLockTarget);
		}
	}
}

void AMainController::EndPlay(EEndPlayReason::Type EndPlayerReason)
{
	Super::EndPlay(EndPlayerReason);

	if (EndPlayerReason == EEndPlayReason::Quit) {
		if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
			SaveCharacterStats();
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
				if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAttributeSet)) {
					HaveHealthAttr->SetBaseMaxHealth(JsonObj->GetNumberField(TEXT("max_hp")));
					HaveHealthAttr->SetCurrentBaseHealth(JsonObj->GetNumberField(TEXT("hp")));
				}
				if (IHaveStaminaAttribute* HaveStaminaAttr = Cast<IHaveStaminaAttribute>(HaveAttributeSet)) {
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

void AMainController::SaveCharacterStats()
{
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
	if (IHaveAttributeSet* HaveAttributeSet = this->GetPawn<IHaveAttributeSet>()) {
		if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAttributeSet)) {
			JsonObj->SetNumberField(TEXT("max_hp"), HaveHealthAttr->GetBaseMaxHealth());
			JsonObj->SetNumberField(TEXT("hp"), HaveHealthAttr->GetCurrentBaseHealth());
		}
		if (IHaveStaminaAttribute* HaveStaminaAttr = Cast<IHaveStaminaAttribute>(HaveAttributeSet)) {
			JsonObj->SetNumberField(TEXT("max_stamina"), HaveStaminaAttr->GetBaseMaxStamina());
		}
	}
	if (ICanUseItem* CanUseItem = this->GetPawn<ICanUseItem>()) {
		if (UItemComponent* ItemComp = CanUseItem->GetItemComponent()) {
			if (UUsableItem* HealthPotion = Cast<UUsableItem>(ItemComp->GetItemByName(FName("HealthPotion")))) {
				JsonObj->SetNumberField(TEXT("health_potion_quant"), HealthPotion->GetQuantity());
			}
		}
	}
	if (IAbilitySystemInterface* ASI = this->GetPawn<IAbilitySystemInterface>()) {
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
			if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) {
				JsonObj->SetStringField(TEXT("state"), TEXT("State.Dead"));
			}
		}
	}
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

void AMainController::OnGetCharacterStatsComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (pResponse.IsValid()) {
			switch (pResponse->GetResponseCode()) {
			case EHttpResponseCodes::Ok:
			{
				Server_FetchCharacterStats(pResponse->GetContentAsString());
				break;
			}
			}
		}
	}
}

void AMainController::SpectatePlayer()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
		if (Iterator->IsValid()) {
			if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Iterator->Get()->GetPawn())) {
				if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
					if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) {
						CurSpectatedPlayerIdx = Iterator.GetIndex();
						CurrentSpectatedPlayer = Iterator->Get(false);
						SetViewTargetWithBlend(Iterator->Get());
						return;
					}
				}
			}
		}
	}
}

void AMainController::Client_TravelToMap_Implementation(FName MapName)
{
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
		if (Target)
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

void AMainController::Server_SpectateNextPlayer_Implementation()
{
	if (CurSpectatedPlayerIdx != -1) {
		int NumPlayers = GetWorld()->GetNumPlayerControllers();
		int curIdx = CurSpectatedPlayerIdx + 1;
		while (true) {
			if (curIdx == NumPlayers) {
				curIdx = 0;
			}
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), curIdx)) {
				if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(PC->GetPawn())) {
					if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
						if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) {
							CurSpectatedPlayerIdx = curIdx;
							CurrentSpectatedPlayer = PC;
							SetViewTargetWithBlend(PC);
							return;
						}
					}
				}
			}
			++curIdx;
		}
	}
}
