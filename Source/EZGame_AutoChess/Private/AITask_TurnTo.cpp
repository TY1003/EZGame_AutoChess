// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask_TurnTo.h"
#include "AIController.h"
#include "ChessMovementComponent.h"

//创建Task对象-静态
UAITask_TurnTo* UAITask_TurnTo::AITurnTo(AAIController* Controller, AActor* GoalActor, UGridNode* GoalNode, FRotator GoalRotation, float StopOnDegree)
{
	if (!Controller)
		return nullptr;
	//创建Task对象
	UAITask_TurnTo* MyTask = UAITask::NewAITask<UAITask_TurnTo>(*Controller, EAITaskPriority::High);
	//设置基本属性
	MyTask->GoalActor = GoalActor;
	MyTask->GoalNode = GoalNode;
	MyTask->GoalRotation = GoalRotation;
	MyTask->StopOnDegree = StopOnDegree;
	return MyTask;
}

//触发任务
void UAITask_TurnTo::Activate()
{
	Super::Activate();
	//触发任务后开始旋转
	PerformRotate();
}

//执行旋转任务
void UAITask_TurnTo::PerformRotate()
{
	//安全判断
	if (!OwnerController || !OwnerController->GetPawn())
	{
		RequestFailed();
		return;
	}
	//获取当前角色和移动组件
	APawn* OwnerPawn = OwnerController->GetPawn();
	UChessMovementComponent* ChessMovementComponent = OwnerPawn->FindComponentByClass<UChessMovementComponent>();
	if (!ChessMovementComponent)
	{
		RequestFailed();
		return;
	}
	//设定旋转目标
	if (GoalActor)
		ChessMovementComponent->RotateTarget = FRotateTarget(GoalActor, StopOnDegree);
	else if (GoalNode)
		ChessMovementComponent->RotateTarget = FRotateTarget(GoalNode, StopOnDegree);
	else
		ChessMovementComponent->RotateTarget = FRotateTarget(GoalRotation, StopOnDegree);
	//绑定委托，旋转完成后，完成任务
	if (!ChessMovementComponent->OnRotateComplete.IsAlreadyBound(this, &UAITask_TurnTo::RequestSuccess))
		ChessMovementComponent->OnRotateComplete.AddDynamic(this, &UAITask_TurnTo::RequestSuccess);
}

//成功完成任务
void UAITask_TurnTo::RequestSuccess()
{
	if (!OwnerController || !OwnerController->GetPawn())
		return;
	//移除委托
	UChessMovementComponent* ChessMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChessMovementComponent>();
	if (ChessMovementComponent && ChessMovementComponent->OnRotateComplete.IsAlreadyBound(this, &UAITask_TurnTo::RequestSuccess))
		ChessMovementComponent->OnRotateComplete.RemoveDynamic(this, &UAITask_TurnTo::RequestSuccess);
	OnRequestSuccess.Broadcast();
	EndTask();
}

//失败完成任务
void UAITask_TurnTo::RequestFailed()
{
	if (!OwnerController || !OwnerController->GetPawn())
		return;
	//移除委托
	UChessMovementComponent* ChessMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChessMovementComponent>();
	if (ChessMovementComponent && ChessMovementComponent->OnRotateComplete.IsAlreadyBound(this, &UAITask_TurnTo::RequestSuccess))
		ChessMovementComponent->OnRotateComplete.RemoveDynamic(this, &UAITask_TurnTo::RequestSuccess);
	OnRequestFailed.Broadcast();
	EndTask();
}

//强制执行
void UAITask_TurnTo::ForceActiveTask()
{
	Activate();
}

//强制失败
void UAITask_TurnTo::ForceEndTask()
{
	RequestSuccess();
}