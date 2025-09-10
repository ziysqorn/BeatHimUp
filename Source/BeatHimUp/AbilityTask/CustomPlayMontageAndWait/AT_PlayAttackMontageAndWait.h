// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AT_CustomPlayMontageAndWait.h"
#include "AT_PlayAttackMontageAndWait.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UAT_PlayAttackMontageAndWait : public UAT_CustomPlayMontageAndWait
{
	GENERATED_BODY()
	
protected:
	void Activate() override;
};
