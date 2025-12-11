// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

int32 UItem::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
    if (AActor* OuterActor = Cast<AActor>(GetOuter()))
    {
        return OuterActor->GetFunctionCallspace(Function, Stack);
    }

    return FunctionCallspace::Local;
}

bool UItem::CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack)
{
    if (AActor* OuterActor = Cast<AActor>(GetOuter()))
    {
        UNetDriver* NetDriver = OuterActor->GetNetDriver();
        if (NetDriver)
        {
            NetDriver->ProcessRemoteFunction(OuterActor, Function, Parameters, OutParms, Stack, this);
            return true;
        }
    }
    return false;
}
