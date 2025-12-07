// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../AttributeSet/AS_AICharacter.h"
#include "../AttributeSet/AttributeSet_PlayableCharacter.h"
#include "ExecCalc_EnemyApplyDamage.generated.h"

/**
 * 
 */
USTRUCT()
struct FAICharApplyDamageCapture {
	GENERATED_BODY()

	DECLARE_ATTRIBUTE_CAPTUREDEF(AICharacterDamage)

	FAICharApplyDamageCapture() {
		AICharacterDamageDef.AttributeToCapture = UAS_AICharacter::GetDamageAttribute();
		AICharacterDamageDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
		AICharacterDamageDef.bSnapshot = false;

	}
};

static FAICharApplyDamageCapture& AICharApplyDamageCaptureStatics() {
	static FAICharApplyDamageCapture ApplyDamageCaptureIns;
	return ApplyDamageCaptureIns;
}
UCLASS()
class BEATHIMUP_API UExecCalc_EnemyApplyDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_EnemyApplyDamage();

protected:
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
