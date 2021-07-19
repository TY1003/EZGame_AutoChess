// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask_MoveToNode.h"
#include "AIController.h"
#include "GridNode.h"
#include "GridMap.h"
#include "ChessMovementComponent.h"

//创建Task对象-静态
UAITask_MoveToNode* UAITask_MoveToNode::AIMoveTo(AAIController* Controller, UGridNode* GoalNode, AActor* GoalActor, int StopOnStep)
{
	if(!Controller)
		return nullptr;
	//创建Task对象
	UAITask_MoveToNode* MyTask =  UAITask::NewAITask<UAITask_MoveToNode>(*Controller, EAITaskPriority::High);
	//设置基本属性
	MyTask->ToNode = GoalNode;
	MyTask->GoalActor = GoalActor;
	MyTask->StopOnStep = StopOnStep;
	return MyTask;
}

//触发任务
void UAITask_MoveToNode::Activate()
{
	Super::Activate();
	//触发任务后开始移动
	PerformMove();
}

//执行移动任务
void UAITask_MoveToNode::PerformMove()
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
	//获取当前起点和终点
	if (GoalActor)
	{
		FromNode = GetGridNode(OwnerPawn);
		ToNode = GetGridNode(GoalActor, true);
	}
	else
	{
		FromNode = GetGridNode(OwnerPawn);
	}
	//判断起点终点状态
	if (!FromNode || !ToNode)
	{
		RequestFailed();
		return;
	}
	if (FromNode == ToNode)
	{
		RequestSuccess();
		return;
	}
	//进行寻路
	TArray<UGridNode*> Path;
	bool tResult = FromNode->GridMap->FindPath(Path, OwnerPawn, FromNode, ToNode, StopOnStep);
	//回传False，直接失败
	if (!tResult)
	{
		RequestFailed();
		return;
	}
	//回传True，并路径点数量为0，则为成功完成移动任务
	if (Path.Num() == 0)
	{
		RequestSuccess();
		return;
	}
	//设置路径
	ChessMovementComponent->SetMovePath(Path);
	//绑定委托，在每次到达中间路点后，重新执行任务
	if (!ChessMovementComponent->OnMoveCheckPoint.IsAlreadyBound(this, &UAITask_MoveToNode::PerformMove))
		ChessMovementComponent->OnMoveCheckPoint.AddDynamic(this, &UAITask_MoveToNode::PerformMove);
	//绑定委托，到达终点后，完成任务
	if (!ChessMovementComponent->OnMoveComplete.IsAlreadyBound(this, &UAITask_MoveToNode::RequestSuccess))
		ChessMovementComponent->OnMoveComplete.AddDynamic(this, &UAITask_MoveToNode::RequestSuccess);

}

//成功完成任务
void UAITask_MoveToNode::RequestSuccess()
{
	if (!OwnerController || !OwnerController->GetPawn())
		return;
	//移除委托
	UChessMovementComponent* ChessMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChessMovementComponent>();
	if (ChessMovementComponent && ChessMovementComponent->OnMoveCheckPoint.IsAlreadyBound(this, &UAITask_MoveToNode::PerformMove))
		ChessMovementComponent->OnMoveCheckPoint.RemoveDynamic(this, &UAITask_MoveToNode::PerformMove);
	if (ChessMovementComponent && ChessMovementComponent->OnMoveComplete.IsAlreadyBound(this, &UAITask_MoveToNode::RequestSuccess))
		ChessMovementComponent->OnMoveComplete.RemoveDynamic(this, &UAITask_MoveToNode::RequestSuccess);
	ChessMovementComponent->StopMovementImmediately();
	OnRequestSuccess.Broadcast();
	EndTask();
}

//失败完成任务
void UAITask_MoveToNode::RequestFailed()
{
	if (!OwnerController || !OwnerController->GetPawn())
		return;
	//移除委托
	UChessMovementComponent* ChessMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChessMovementComponent>();
	if (ChessMovementComponent && ChessMovementComponent->OnMoveCheckPoint.IsAlreadyBound(this, &UAITask_MoveToNode::PerformMove))
		ChessMovementComponent->OnMoveCheckPoint.RemoveDynamic(this, &UAITask_MoveToNode::PerformMove);
	if (ChessMovementComponent && ChessMovementComponent->OnMoveComplete.IsAlreadyBound(this, &UAITask_MoveToNode::RequestSuccess))
		ChessMovementComponent->OnMoveComplete.RemoveDynamic(this, &UAITask_MoveToNode::RequestSuccess);
	ChessMovementComponent->StopMovementImmediately();
	OnRequestFailed.Broadcast();
	EndTask();
}

//获取角色当前棋格
UGridNode* UAITask_MoveToNode::GetGridNode(AActor* InActor, bool bIncldeBookNode) const
{
	if (!InActor)
		return nullptr;
	//获取移动组件的NowNode
	UChessMovementComponent* ChessMovementComponent = InActor->FindComponentByClass<UChessMovementComponent>();
	if (!ChessMovementComponent)
		return nullptr;
	if (bIncldeBookNode && ChessMovementComponent->BookNode)
		return ChessMovementComponent->BookNode;
	else
		return ChessMovementComponent->NowNode;
}

//强制执行
void UAITask_MoveToNode::ForceActiveTask()
{
	Activate();
}

//强制失败
void UAITask_MoveToNode::ForceEndTask()
{
	RequestSuccess();
}