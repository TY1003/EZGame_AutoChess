// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/AITask.h"
#include "AITask_TurnTo.generated.h"

class UGridNode;

/**
 * 
 */
UCLASS()
class EZGAME_AUTOCHESS_API UAITask_TurnTo : public UAITask
{
	GENERATED_BODY()

public:
	//创建Task对象-静态
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Controller", BlueprintInternalUseOnly = "TRUE", DisplayName = "Turn To"))
	static UAITask_TurnTo* AITurnTo(AAIController* Controller, AActor* GoalActor, UGridNode* GoalNode, FRotator GoalRotation, float StopOnDegree = 10);

	//旋转目标角色
	UPROPERTY()
	AActor* GoalActor;

	//旋转目标棋格
	UPROPERTY()
	UGridNode* GoalNode;

	//旋转目标值
	UPROPERTY()
	FRotator GoalRotation;

	//旋转容差值
	UPROPERTY()
	float StopOnDegree;

	//强制执行
	void ForceActiveTask();

	//强制失败
	void ForceEndTask();

protected:
	//委托-成功结束
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnRequestSuccess;

	//委托-失败结束
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnRequestFailed;

	//触发任务函数
	virtual void Activate() override;

	//执行旋转函数
	UFUNCTION()
	void PerformRotate();

	//成功完成任务函数
	UFUNCTION()
	void RequestSuccess();

	//失败完成任务函数
	UFUNCTION()
	void RequestFailed();
	
};
