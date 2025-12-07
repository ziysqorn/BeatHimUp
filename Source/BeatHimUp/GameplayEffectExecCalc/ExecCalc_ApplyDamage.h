// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../AttributeSet/AttributeSet_PlayableCharacter.h"
#include "../AttributeSet/AS_AICharacter.h"
#include "ExecCalc_ApplyDamage.generated.h"

/**
 * 
 */
USTRUCT()
struct FApplyDamageCapture {
	GENERATED_BODY()

	DECLARE_ATTRIBUTE_CAPTUREDEF(MainCharacterDamage)

	FApplyDamageCapture() {
		MainCharacterDamageDef.AttributeToCapture = UAttributeSet_PlayableCharacter::GetDamageAttribute();
		MainCharacterDamageDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
		MainCharacterDamageDef.bSnapshot = false;
	}
};

static FApplyDamageCapture& ApplyDamageCaptureStatics() {
	static FApplyDamageCapture ApplyDamageCaptureIns;
	return ApplyDamageCaptureIns;
}

UCLASS()
class BEATHIMUP_API UExecCalc_ApplyDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_ApplyDamage();

protected:
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
