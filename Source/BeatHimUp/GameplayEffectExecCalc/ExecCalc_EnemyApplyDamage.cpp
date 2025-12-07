// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_EnemyApplyDamage.h"

UExecCalc_EnemyApplyDamage::UExecCalc_EnemyApplyDamage()
{
	this->RelevantAttributesToCapture.Add(AICharApplyDamageCaptureStatics().AICharacterDamageDef);
}

void UExecCalc_EnemyApplyDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FAggregatorEvaluateParameters EvaluateParams;

	float RawAICharacterDamage = 0.f, FinalAICharacterDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AICharApplyDamageCaptureStatics().AICharacterDamageDef, EvaluateParams, RawAICharacterDamage);

	FGameplayEffectSpec GESpec = ExecutionParams.GetOwningSpec();
	float ShieldDef = GESpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.ShieldDef")));

	FinalAICharacterDamage = RawAICharacterDamage - ShieldDef;

	FGameplayModifierEvaluatedData DataApplyToMainCharacter(UAttributeSet_PlayableCharacter::GetHealthAttribute(), EGameplayModOp::Additive, -FinalAICharacterDamage);

	OutExecutionOutput.AddOutputModifier(DataApplyToMainCharacter);
}
