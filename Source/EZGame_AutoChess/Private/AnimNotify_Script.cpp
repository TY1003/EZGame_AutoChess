// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Script.h"

void UAnimNotify_Script::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	//安全判断
	if (!MeshComp)
		return;
	AActor* tActor = MeshComp->GetOwner();
	if (tActor)
	{
		//使用反射，根据函数名调用函数
		UFunction* tFunc = tActor->FindFunction(FunctionName);
		if (tFunc)
			tActor->ProcessEvent(tFunc, nullptr);
	}
}