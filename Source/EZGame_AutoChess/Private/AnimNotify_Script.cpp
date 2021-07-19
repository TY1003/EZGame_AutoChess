// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Script.h"

void UAnimNotify_Script::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	//��ȫ�ж�
	if (!MeshComp)
		return;
	AActor* tActor = MeshComp->GetOwner();
	if (tActor)
	{
		//ʹ�÷��䣬���ݺ��������ú���
		UFunction* tFunc = tActor->FindFunction(FunctionName);
		if (tFunc)
			tActor->ProcessEvent(tFunc, nullptr);
	}
}