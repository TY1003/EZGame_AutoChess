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
	//调用函数名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FunctionName;

	//触发通知时调用
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
