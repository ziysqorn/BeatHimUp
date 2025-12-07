// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_ApplyDamage.h"

UExecCalc_ApplyDamage::UExecCalc_ApplyDamage()
{
	this->RelevantAttributesToCapture.Add(ApplyDamageCaptureStatics().MainCharacterDamageDef);
}

void UExecCalc_ApplyDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FAggregatorEvaluateParameters EvaluateParams;

	float RawMainCharacterDamage = 0.f, FinalMainCharacterDamage = 0.f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ApplyDamageCaptureStatics().MainCharacterDamageDef, EvaluateParams, RawMainCharacterDamage);

	FGameplayEffectSpec GESpec = ExecutionParams.GetOwningSpec();
	float ShieldDef = GESpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.ShieldDef")));

	FinalMainCharacterDamage = RawMainCharacterDamage - ShieldDef;

	FGameplayModifierEvaluatedData DataApplyToAICharacter(UAS_AICharacter::GetHealthAttribute(), EGameplayModOp::Additive, -FinalMainCharacterDamage);

	OutExecutionOutput.AddOutputModifier(DataApplyToAICharacter);
}
