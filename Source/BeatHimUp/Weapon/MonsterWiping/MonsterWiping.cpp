// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWiping.h"

AMonsterWiping::AMonsterWiping()
{
}

void AMonsterWiping::BeginPlay()
{
	Super::BeginPlay();
	
	this->BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AMonsterWiping::BoxCompBeginOverlapped);
}

void AMonsterWiping::BoxCompBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor != this && OtherActor != this->GetOwner()) {
		if (ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor)) {
			FGameplayTag eventTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
			FGameplayEventData Payload;
			Payload.EventTag = eventTag;
			Payload.Instigator = this->GetOwner();
			Payload.Target = OtherCharacter;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this->GetOwner(), eventTag, Payload);
		}
	}
}