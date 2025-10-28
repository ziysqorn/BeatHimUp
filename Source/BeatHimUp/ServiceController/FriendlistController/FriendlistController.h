// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../MyStructs/MyStructs.h"
#include "FriendlistController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UFriendlistController : public UObject
{
	GENERATED_BODY()

public:
	UFriendlistController();

	void SendFriendlistMessage(FName Username);
};
