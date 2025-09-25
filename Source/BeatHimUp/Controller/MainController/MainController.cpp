// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"

AMainController::AMainController()
{
}

void AMainController::BeginPlay()
{
	Super::BeginPlay();

}

void AMainController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	AddHUD();
}

void AMainController::AddHUD_Implementation()
{
	if (MainHUDSubclass) {
		MainHUD = CreateWidget<UCustomHUD>(this, MainHUDSubclass);
		if (MainHUD) {
			MainHUD->BindHealthProgress(this, FName("GetPawnHealthPercentage"));
			MainHUD->AddToViewport(0);
		}
	}
}
