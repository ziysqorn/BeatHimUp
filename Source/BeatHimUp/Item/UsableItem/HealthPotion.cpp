// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"

UHealthPotion::UHealthPotion()
{
	Quantity = 3;
}

void UHealthPotion::Use_Implementation()
{
	if (Quantity <= 0) return;
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(this->GetOuter())) {
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			ContextHandle.AddInstigator(Cast<AActor>(this->GetOuter()), Cast<AActor>(this->GetOuter()));
			ContextHandle.AddSourceObject(this);
			
			FGameplayEffectSpecHandle GESpecHandle = ASC->MakeOutgoingSpec(GESubclass_IncreaseHealth, Level, ContextHandle);

			if(GESpecHandle.IsValid())
				ASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data.Get());
		}
	}
	--Quantity;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Format(TEXT("HEALTH POTION USED ! - CurrentQuantity: {0}"), {Quantity}));
}
