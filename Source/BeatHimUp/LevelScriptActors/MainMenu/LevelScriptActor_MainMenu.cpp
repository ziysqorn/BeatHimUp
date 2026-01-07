// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActor_MainMenu.h"
#include "../../CustomGameInstance/MyGameInstance.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"

void ALevelScriptActor_MainMenu::BeginPlay()
{
	Super::BeginPlay();
	
	SetupView();

	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		MyGameInstance->OnLobbyUpdateDel.AddUObject(this, &ALevelScriptActor_MainMenu::UpdatePlayerPreviewerList);
	}
}

void ALevelScriptActor_MainMenu::UpdatePlayerPreviewerList(const FLobbyInfo& LobbyInfo)
{
	int CurrentPreviewerIdx = 1;
	for (int i = 0; i < PlayerPreviewerList.Num(); ++i) {
		if (IsValid(PlayerPreviewerList[i])) {
			PlayerPreviewerList[i]->SetActorHiddenInGame(true);
		}
	}
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
			UIManager->HideCtxMenu();
		}
		for (int i = 0; i < LobbyInfo.Members.Num(); ++i) {
			int ToUseIdx = CurrentPreviewerIdx;
			if (LobbyInfo.Members[i].Username.IsEqual(MyGameInstance->GetPlayerInfo().Username)) {
				ToUseIdx = 0;
			}
			if (PlayerPreviewerList.IsValidIndex(ToUseIdx)) {
				if (IsValid(PlayerPreviewerList[ToUseIdx])) {
					PlayerPreviewerList[ToUseIdx]->SetActorHiddenInGame(false);
					/*if (USkeletalMeshComponent* ModelComp = PlayerPreviewerList[i]->GetModel()) {
						ModelComp->SetVisibility(true);
					}*/
					if (UPlayerPreviewerWidgetComponent* PlayerPreviewerWidgetComp = PlayerPreviewerList[ToUseIdx]->GetPlayerPreviewerWidgetComp()) {
						PlayerPreviewerWidgetComp->SetUsernameText(LobbyInfo.Members[i].Username.ToString());
						if (LobbyInfo.Members[i].Username.IsEqual(LobbyInfo.Leader_Username)) {
							PlayerPreviewerWidgetComp->ShowCaptainIcon();
						}
						else {
							PlayerPreviewerWidgetComp->HideCaptainIcon();
						}
					}
				}
			}
			CurrentPreviewerIdx = ToUseIdx + 1;
		}
	}
}

void ALevelScriptActor_MainMenu::ResetPlayerPreviewerListRotation()
{
	for (auto PlayerPreviewer : PlayerPreviewerList) {
		if(IsValid(PlayerPreviewer)) PlayerPreviewer->ResetModelRotation();
	}
}

void ALevelScriptActor_MainMenu::ResetPlayerPreviewerRotationAt(int idx)
{
	if (PlayerPreviewerList.IsValidIndex(idx)) {
		PlayerPreviewerList[idx]->ResetModelRotation();
	}
}

void ALevelScriptActor_MainMenu::SetupView()
{
	for (int i = 0; i < PlayerPreviewerList.Num(); ++i) {
		if (PlayerPreviewerList[i]) {
			/*if (USkeletalMeshComponent* ModelComp = PlayerPreviewerList[i]->GetModel()) {
				ModelComp->SetVisibility(false);
			}*/
			PlayerPreviewerList[i]->SetActorHiddenInGame(true);
		}
	}
	if (CineCamera) {
		if (APlayerController* MainMenuController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0))) {
			MainMenuController->SetViewTargetWithBlend(CineCamera);
		}
	}
}