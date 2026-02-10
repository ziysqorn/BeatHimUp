// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDComponent.h"
#include "../../CustomGameState/MainGameState.h"
#include "../../Controller/MainController/MainController.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Interface/CanUseItem.h"
#include "../../Interface/HaveHealthAttribute.h"
#include "../../Interface/HaveStaminaAttribute.h"


// Sets default values for this component's properties
UPlayerHUDComponent::UPlayerHUDComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UPlayerHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPlayerHUDComponent::SetupItemFrameList(const TArray<UUsableItem*>& inItemList)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(this->GetOwner())) {
		if (IsValid(MainHUD)) {
			if (const UPanelWidget* ItemFrameContainer = MainHUD->GetItemFrameContainer()) {
				int ListSizeDiff = FMath::Abs(inItemList.Num() - ItemFrameContainer->GetChildrenCount());
				if (inItemList.Num() > ItemFrameContainer->GetChildrenCount()) {
					for (int i = 0; i < ListSizeDiff; ++i) {
						if (UUsableItemFrame* UsableItemFrame = CreateWidget<UUsableItemFrame>(PlayerController, UsableItemFrameSubclass)) {
							MainHUD->AddItemFrameToContainer(UsableItemFrame);
						}
					}
				}
				else if (inItemList.Num() < ItemFrameContainer->GetChildrenCount()) {
					for (int i = 0; i < ListSizeDiff; ++i) {
						MainHUD->RemoveLastItemFrameFromContainer();
					}
				}

				for (int i = 0; i < inItemList.Num(); ++i) {
					if (IsValid(inItemList[i])) {
						if (UUsableItemFrame* UsableItemFrame = Cast<UUsableItemFrame>(MainHUD->GetItemFrame(i))) {
							UsableItemFrame->SetItemImage(inItemList[i]->GetItemImage());
							UsableItemFrame->SetQuantityText(FText::FromString(FString::FromInt(inItemList[i]->GetQuantity())));
						}
					}
				}
			}
		}
	}
}

void UPlayerHUDComponent::UpdateItemFrameQuantity(UUsableItem* Item)
{
	if (IsValid(Item)) {
		if (APlayerController* PlayerController = Cast<APlayerController>(this->GetOwner())) {
			if (ICanUseItem* CanUseItem = Cast<ICanUseItem>(PlayerController->GetPawn())) {
				if (UItemComponent* ItemComp = CanUseItem->GetItemComponent()) {
					const TArray<UUsableItem*>& ItemList = ItemComp->GetUsableItemList();
					for (int i = 0; i < ItemList.Num(); ++i) {
						if (IsValid(ItemList[i])) {
							if (ItemList[i]->GetItemName().IsEqual(Item->GetItemName())) {
								if (IsValid(MainHUD)) {
									if (UUsableItemFrame* UsableItemFrame = Cast<UUsableItemFrame>(MainHUD->GetItemFrame(i))) {
										/*FString PrintMessage = TEXT("");
											if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
												if (APlayerState* OwningPS = PC->GetPlayerState<APlayerState>()) {
													PrintMessage.Append(FString::Format(TEXT("OwningClient: {0}"), { OwningPS->GetPlayerName() }));
												}
											}
											PrintMessage.Append(FString::Format(TEXT(" --> Item: {0} - Quantity: {1}"), { Item->GetItemName().ToString(),  FString::FromInt(Item->GetQuantity())}));
											GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, PrintMessage);*/
										UsableItemFrame->SetQuantityText(FText::FromString(FString::FromInt(Item->GetQuantity())));
										return;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void UPlayerHUDComponent::Client_UpdateItemFrameQuantityByViewTarget_Implementation(UUsableItem* Item, AActor* CurrentViewTargetPawn)
{
	if (IsValid(Item)) {
		if (ICanUseItem* CanUseItem = Cast<ICanUseItem>(CurrentViewTargetPawn)) {
			if (UItemComponent* ItemComp = CanUseItem->GetItemComponent()) {
				const TArray<UUsableItem*>& ItemList = ItemComp->GetUsableItemList();
				for (int i = 0; i < ItemList.Num(); ++i) {
					if (IsValid(ItemList[i])) {
						if (ItemList[i]->GetItemName().IsEqual(Item->GetItemName())) {
							if (IsValid(MainHUD)) {
								if (UUsableItemFrame* UsableItemFrame = Cast<UUsableItemFrame>(MainHUD->GetItemFrame(i))) {
									UsableItemFrame->SetQuantityText(FText::FromString(FString::FromInt(Item->GetQuantity())));
									return;
								}
							}
						}
					}
				}
			}
		}
	}
}

void UPlayerHUDComponent::BindItemDelegates()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Controller bound !"));
	if (APlayerController* PlayerController = Cast<APlayerController>(this->GetOwner())) {
		if (ICanUseItem* CanUseItem = Cast<ICanUseItem>(PlayerController->GetPawn())) {
			if (UItemComponent* ItemComp = CanUseItem->GetItemComponent()) {
				ItemComp->OnRepUsableItemListDel.AddUObject(this, &UPlayerHUDComponent::SetupItemFrameList);
				ItemComp->OnItemQuantityChangedDel.AddUObject(this, &UPlayerHUDComponent::UpdateItemFrameQuantity);
			}
		}
	}
}

void UPlayerHUDComponent::UpdateBossInfoUI(AActor* BossRef)
{
	if (IsValid(MainHUD)) {
		MainHUD->SetBossName(FText::FromString(TEXT("Warrok")));
		if (IHaveAttributeSet* HaveAS = Cast<IHaveAttributeSet>(BossRef)) {
			if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAS->GetAttributeSet())) {
				MainHUD->BindBossHealthProgress(HaveAS->GetAttributeSet(), FName("GetHealthPercentage"));
			}
		}
	}
}

void UPlayerHUDComponent::Client_SetEnemyNameText_Implementation(const FText& inText)
{
	if (IsValid(MainHUD)) {
		MainHUD->SetBossName(inText);
	}
}

void UPlayerHUDComponent::Client_BindEnemyHealthBar_Implementation(AActor* Actor)
{
	if (IsValid(MainHUD)) {
		if (IHaveAttributeSet* HaveAS = Cast<IHaveAttributeSet>(Actor)) {
			if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAS->GetAttributeSet())) {
				MainHUD->BindBossHealthProgress(HaveAS->GetAttributeSet(), FName("GetHealthPercentage"));
			}
		}
	}
}

void UPlayerHUDComponent::Client_SetupBossHUDHealthbar_Implementation(AActor* Actor)
{
	if (MainHUDSubclass) {
		if (APlayerController* PlayerController = Cast<APlayerController>(this->GetOwner())) {
			if (!IsValid(MainHUD))
				MainHUD = CreateWidget<UCustomHUD>(PlayerController, MainHUDSubclass);
			if (IsValid(MainHUD)) {
				MainHUD->SetBossName(FText::FromString(TEXT("Warrok")));
				if (IHaveAttributeSet* HaveAS = Cast<IHaveAttributeSet>(Actor)) {
					if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAS->GetAttributeSet())) {
						MainHUD->BindBossHealthProgress(HaveAS->GetAttributeSet(), FName("GetHealthPercentage"));
					}
				}
				if (!MainHUD->IsInViewport()) {
					MainHUD->AddToViewport(0);
				}
			}
		}
	}
}

void UPlayerHUDComponent::ShowHUD()
{
	if (IsValid(MainHUD)) {
		MainHUD->AddToViewport(0);
	}
}

void UPlayerHUDComponent::HideHUD()
{
	if (IsValid(MainHUD)) {
		MainHUD->RemoveFromParent();
	}
}

void UPlayerHUDComponent::DisplayMatchStatusMessage(EMatchStatus inMatchStatus)
{
	if (AMainController* PlayerController = Cast<AMainController>(this->GetOwner())) {
		switch (inMatchStatus) {
		case EMatchStatus::MATCH_WON:
			if (!IsValid(OnScreenMessage)) {
				if (OnScreenMessageSubclass)
					OnScreenMessage = CreateWidget<UOnScreenMessage>(PlayerController, OnScreenMessageSubclass);
			}
			OnScreenMessage->SetMessageText(FText::FromString(TEXT("YOU WON !")));
			OnScreenMessage->SetMessageColor(TEXT("#0ec208"));
			OnScreenMessage->SetAfterFinishedFadeinDelay(2.0f);
			OnScreenMessage->AfterFinishedFadeinDel.BindUObject(PlayerController, &AMainController::EndGameProcess, inMatchStatus);
			OnScreenMessage->AddToViewport(99);
			OnScreenMessage->PlayMessageFadein();
			break;
		case EMatchStatus::MATCH_LOST:
			if (!IsValid(OnScreenMessage)) {
				if (OnScreenMessageSubclass)
					OnScreenMessage = CreateWidget<UOnScreenMessage>(PlayerController, OnScreenMessageSubclass);
			}
			OnScreenMessage->SetMessageText(FText::FromString(TEXT("YOU LOSE !")));
			OnScreenMessage->SetMessageColor(TEXT("#e60e0e"));
			OnScreenMessage->SetAfterFinishedFadeinDelay(2.0f);
			OnScreenMessage->AfterFinishedFadeinDel.BindUObject(PlayerController, &AMainController::EndGameProcess, inMatchStatus);
			OnScreenMessage->AddToViewport(99);
			OnScreenMessage->PlayMessageFadein();
			break;
		default:
			break;
		}
	}
}

void UPlayerHUDComponent::DisplayPauseUI()
{
	if (IsValid(DA_UI)) {
		if (UGameInstance* MyGameInstance = GetWorld()->GetGameInstance()) {
			if (UUIManagerSubsystem* UIManager = MyGameInstance->GetSubsystem<UUIManagerSubsystem>()) {
				if (TSubclassOf<UUserWidget>* UserSettingsWidgetSubclass = DA_UI->UISubclassMap.Find("UserSettingsWidget")) {
					UIManager->ShowUserSettingsWidget(*UserSettingsWidgetSubclass, 5);
				}
			}
		}
	}
}

void UPlayerHUDComponent::Client_AddHUD_Implementation()
{
	if (MainHUDSubclass) {
		if (APlayerController* PlayerController = Cast<APlayerController>(this->GetOwner())) {
			if (!IsValid(MainHUD))
				MainHUD = CreateWidget<UCustomHUD>(PlayerController, MainHUDSubclass);
			if (MainHUD) {
				if (IHaveAttributeSet* HaveAS = Cast<IHaveAttributeSet>(PlayerController->GetPawn())) {
					if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAS->GetAttributeSet())) {
						MainHUD->BindHealthProgress(HaveAS->GetAttributeSet(), FName("GetHealthPercentage"));
					}
					if (IHaveStaminaAttribute* HaveStaminaAttr = Cast<IHaveStaminaAttribute>(HaveAS->GetAttributeSet())) {
						MainHUD->BindStaminaProgress(HaveAS->GetAttributeSet(), FName("GetStaminaPercentage"));
					}
				}
				BindItemDelegates();
				if (ICanUseItem* CanUseItem = Cast<ICanUseItem>(PlayerController->GetPawn())) {
					if (UItemComponent* ItemComp = CanUseItem->GetItemComponent()) {
						ItemComp->Server_InitUsableItemList();
					}
				}
				if (AMainGameState* MainGS = GetWorld()->GetGameState<AMainGameState>()) {
					UpdateBossInfoUI(MainGS->GetBossRef());
					MainGS->OnRepBossRefDel.AddUObject(this, &UPlayerHUDComponent::UpdateBossInfoUI);
				}
				if (!MainHUD->IsInViewport()) {
					MainHUD->AddToViewport(0);
				}
			}
		}
	}
}

void UPlayerHUDComponent::ShowLoadingScreen()
{
	if (GetWorld() && GetWorld()->GetGameInstance()) {
		if (UUIManagerSubsystem* UISubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
			if (TSubclassOf<UUserWidget>* LoadingScreenSubclass = DA_UI->UISubclassMap.Find(FName("LoadingScreen"))) {
				UISubsystem->ShowLoadingScreen(*LoadingScreenSubclass, 10);
			}
		}
	}
}

