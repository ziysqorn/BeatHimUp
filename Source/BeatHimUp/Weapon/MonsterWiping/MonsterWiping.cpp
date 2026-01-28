// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWiping.h"
#include "../Shield/Shield.h"
#include "../../Interface/Damageable.h"
#include "../../Character/BaseCharacter/BaseCharacter.h"

AMonsterWiping::AMonsterWiping()
{
}

void AMonsterWiping::BeginPlay()
{
	Super::BeginPlay();
	
	this->BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AMonsterWiping::BoxCompBeginOverlapped);
	this->BoxComp->OnComponentEndOverlap.AddDynamic(this, &AMonsterWiping::BoxCompEndOverlapped);
}

void AMonsterWiping::BoxCompBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (HasAuthority()) {
		AActor* OtherActorOwner = OtherActor->GetOwner();
		AActor* ThisOwner = this->GetOwner();
		if (IsValid(ThisOwner) && IsValid(OtherActorOwner)) {
			if (OtherActor && OtherActor != this && OtherActor != ThisOwner && OtherActorOwner != ThisOwner) {
				FGameplayTag TargetAttackedEventTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
				FGameplayEventData Payload;
				if (IInteractableWithWeapon* InteractableWithWeapon = Cast<IInteractableWithWeapon>(OtherActor)) {
					if (HandledActors.Contains(OtherActorOwner) || (ThisOwner->GetClass() == OtherActorOwner->GetClass())) {
						return;
					}
					HandledActors.Add(OtherActorOwner);
					InteractableWithWeapon->ResponseToAttackingWeapon(this);
				}
				else if (IDamageable* Damageable = Cast<IDamageable>(OtherActor)) {
					if (HandledActors.Contains(OtherActor) || (ThisOwner->GetClass() == OtherActor->GetClass())) {
						return;
					}
					HandledActors.Add(OtherActor);
					Payload.EventTag = TargetAttackedEventTag;
					Payload.Instigator = ThisOwner;
					Payload.Target = OtherActor;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ThisOwner, TargetAttackedEventTag, Payload);
				}
			}
		}
	}
}

void AMonsterWiping::BoxCompEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*if (HasAuthority()) {
		if (OtherActor && OtherActor != this && OtherActor != this->GetOwner() && OtherActor->GetOwner() != this->GetOwner()) {
			if (IInteractableWithWeapon* InteractableWithWeapon = Cast<IInteractableWithWeapon>(OtherActor)) {
				if (HandledActors.Contains(OtherActor->GetOwner())) {
					HandledActors.Remove(OtherActor->GetOwner());
				}
			}
			else if (IDamageable* Damageable = Cast<IDamageable>(OtherActor)) {
				if (HandledActors.Contains(OtherActor)) {
					HandledActors.Remove(OtherActor);
				}
			}
			else if (HandledActors.Contains(OtherActor)) {
				HandledActors.Remove(OtherActor);
			}
		}
	}*/
}

