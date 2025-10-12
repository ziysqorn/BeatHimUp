// Fill out your copyright notice in the Description page of Project Settings.


#include "ServiceControllerSubsystem.h"

UServiceControllerSubsystem::UServiceControllerSubsystem()
{
	UserAccountController = NewObject<UUserAccountController>(this, FName("UserAccountController"));
}