// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (PostLoginDel.IsBound()) {
		PostLoginDel.Broadcast(NewPlayer);
	}
}
