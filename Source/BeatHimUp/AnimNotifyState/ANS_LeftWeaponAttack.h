// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "ANS_LeftWeaponAttack.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UANS_LeftWeaponAttack : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
