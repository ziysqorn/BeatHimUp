// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"

UHealthPotion::UHealthPotion()
{
	Quantity = 3;
	ItemName = FName("HealthPotion");
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
}

void UHealthPotion::OnRep_Quantity(int OldQuantity) 
{
	Super::OnRep_Quantity(OldQuantity);
}

void UHealthPotion::OnRep_MaxQuantity(int OldMaxQuantity) 
{
	Super::OnRep_MaxQuantity(OldMaxQuantity);
}
