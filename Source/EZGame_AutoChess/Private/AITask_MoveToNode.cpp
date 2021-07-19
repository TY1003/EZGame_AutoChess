// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask_MoveToNode.h"
#include "AIController.h"
#include "GridNode.h"
#include "GridMap.h"
#include "ChessMovementComponent.h"

//����Task����-��̬
UAITask_MoveToNode* UAITask_MoveToNode::AIMoveTo(AAIController* Controller, UGridNode* GoalNode, AActor* GoalActor, int StopOnStep)
{
	if(!Controller)
		return nullptr;
	//����Task����
	UAITask_MoveToNode* MyTask =  UAITask::NewAITask<UAITask_MoveToNode>(*Controller, EAITaskPriority::High);
	//���û�������
	MyTask->ToNode = GoalNode;
	MyTask->GoalActor = GoalActor;
	MyTask->StopOnStep = StopOnStep;
	return MyTask;
}

//��������
void UAITask_MoveToNode::Activate()
{
	Super::Activate();
	//���������ʼ�ƶ�
	PerformMove();
}

//ִ���ƶ�����
void UAITask_MoveToNode::PerformMove()
{
	//��ȫ�ж�
	if (!OwnerController || !OwnerController->GetPawn())
	{
		RequestFailed();
		return;
	}
	//��ȡ��ǰ��ɫ���ƶ����
	APawn* OwnerPawn = OwnerController->GetPawn();
	UChessMovementComponent* ChessMovementComponent = OwnerPawn->FindComponentByClass<UChessMovementComponent>();
	if (!ChessMovementComponent)
	{
		RequestFailed();
		return;
	}
	//��ȡ��ǰ�����յ�
	if (GoalActor)
	{
		FromNode = GetGridNode(OwnerPawn);
		ToNode = GetGridNode(GoalActor, true);
	}
	else
	{
		FromNode = GetGridNode(OwnerPawn);
	}
	//�ж�����յ�״̬
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
	//����Ѱ·
	TArray<UGridNode*> Path;
	bool tResult = FromNode->GridMap->FindPath(Path, OwnerPawn, FromNode, ToNode, StopOnStep);
	//�ش�False��ֱ��ʧ��
	if (!tResult)
	{
		RequestFailed();
		return;
	}
	//�ش�True����·��������Ϊ0����Ϊ�ɹ�����ƶ�����
	if (Path.Num() == 0)
	{
		RequestSuccess();
		return;
	}
	//����·��
	ChessMovementComponent->SetMovePath(Path);
	//��ί�У���ÿ�ε����м�·�������ִ������
	if (!ChessMovementComponent->OnMoveCheckPoint.IsAlreadyBound(this, &UAITask_MoveToNode::PerformMove))
		ChessMovementComponent->OnMoveCheckPoint.AddDynamic(this, &UAITask_MoveToNode::PerformMove);
	//��ί�У������յ���������
	if (!ChessMovementComponent->OnMoveComplete.IsAlreadyBound(this, &UAITask_MoveToNode::RequestSuccess))
		ChessMovementComponent->OnMoveComplete.AddDynamic(this, &UAITask_MoveToNode::RequestSuccess);

}

//�ɹ��������
void UAITask_MoveToNode::RequestSuccess()
{
	if (!OwnerController || !OwnerController->GetPawn())
		return;
	//�Ƴ�ί��
	UChessMovementComponent* ChessMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChessMovementComponent>();
	if (ChessMovementComponent && ChessMovementComponent->OnMoveCheckPoint.IsAlreadyBound(this, &UAITask_MoveToNode::PerformMove))
		ChessMovementComponent->OnMoveCheckPoint.RemoveDynamic(this, &UAITask_MoveToNode::PerformMove);
	if (ChessMovementComponent && ChessMovementComponent->OnMoveComplete.IsAlreadyBound(this, &UAITask_MoveToNode::RequestSuccess))
		ChessMovementComponent->OnMoveComplete.RemoveDynamic(this, &UAITask_MoveToNode::RequestSuccess);
	ChessMovementComponent->StopMovementImmediately();
	OnRequestSuccess.Broadcast();
	EndTask();
}

//ʧ���������
void UAITask_MoveToNode::RequestFailed()
{
	if (!OwnerController || !OwnerController->GetPawn())
		return;
	//�Ƴ�ί��
	UChessMovementComponent* ChessMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChessMovementComponent>();
	if (ChessMovementComponent && ChessMovementComponent->OnMoveCheckPoint.IsAlreadyBound(this, &UAITask_MoveToNode::PerformMove))
		ChessMovementComponent->OnMoveCheckPoint.RemoveDynamic(this, &UAITask_MoveToNode::PerformMove);
	if (ChessMovementComponent && ChessMovementComponent->OnMoveComplete.IsAlreadyBound(this, &UAITask_MoveToNode::RequestSuccess))
		ChessMovementComponent->OnMoveComplete.RemoveDynamic(this, &UAITask_MoveToNode::RequestSuccess);
	ChessMovementComponent->StopMovementImmediately();
	OnRequestFailed.Broadcast();
	EndTask();
}

//��ȡ��ɫ��ǰ���
UGridNode* UAITask_MoveToNode::GetGridNode(AActor* InActor, bool bIncldeBookNode) const
{
	if (!InActor)
		return nullptr;
	//��ȡ�ƶ������NowNode
	UChessMovementComponent* ChessMovementComponent = InActor->FindComponentByClass<UChessMovementComponent>();
	if (!ChessMovementComponent)
		return nullptr;
	if (bIncldeBookNode && ChessMovementComponent->BookNode)
		return ChessMovementComponent->BookNode;
	else
		return ChessMovementComponent->NowNode;
}

//ǿ��ִ��
void UAITask_MoveToNode::ForceActiveTask()
{
	Activate();
}

//ǿ��ʧ��
void UAITask_MoveToNode::ForceEndTask()
{
	RequestSuccess();
}