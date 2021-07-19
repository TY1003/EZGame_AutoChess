// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Script.generated.h"

/**
 * 
 */
UCLASS()
class UAnimNotify_Script : public UAnimNotify
{
	GENERATED_BODY()

public:
	//���ú�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FunctionName;

	//����֪ͨʱ����
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
