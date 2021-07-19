// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/AITask.h"
#include "AITask_MoveToNode.generated.h"

class AAIController;
class UGridNode;

/**
 * 
 */
UCLASS()
class UAITask_MoveToNode : public UAITask
{
	GENERATED_BODY()

public:
	//创建Task对象-静态
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Controller", BlueprintInternalUseOnly = "TRUE", DisplayName = "Move To Node"))
	static UAITask_MoveToNode* AIMoveTo(AAIController* Controller, UGridNode* GoalNode, AActor* GoalActor, int StopOnStep = 1);

	//目标角色
	UPROPERTY()
	AActor* GoalActor;

	//出发棋格
	UPROPERTY()
	UGridNode* FromNode;

	//终点棋格
	UPROPERTY()
	UGridNode* ToNode;

	//停止距离
	UPROPERTY()
	int StopOnStep;

	//强制开启任务
	void ForceActiveTask();

	//强制结束任务
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

	//执行移动函数
	UFUNCTION()
	void PerformMove();

	//成功完成任务函数
	UFUNCTION()
	void RequestSuccess();

	//失败完成任务函数
	UFUNCTION()
	void RequestFailed();

	//根据目标角色获取其棋格
	UGridNode* GetGridNode(AActor* InActor, bool bIncldeBookNode = false) const;

};
