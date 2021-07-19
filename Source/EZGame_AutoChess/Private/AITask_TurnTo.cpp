// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask_TurnTo.h"
#include "AIController.h"
#include "ChessMovementComponent.h"

//����Task����-��̬
UAITask_TurnTo* UAITask_TurnTo::AITurnTo(AAIController* Controller, AActor* GoalActor, UGridNode* GoalNode, FRotator GoalRotation, float StopOnDegree)
{
	if (!Controller)
		return nullptr;
	//����Task����
	UAITask_TurnTo* MyTask = UAITask::NewAITask<UAITask_TurnTo>(*Controller, EAITaskPriority::High);
	//���û�������
	MyTask->GoalActor = GoalActor;
	MyTask->GoalNode = GoalNode;
	MyTask->GoalRotation = GoalRotation;
	MyTask->StopOnDegree = StopOnDegree;
	return MyTask;
}

//��������
void UAITask_TurnTo::Activate()
{
	Super::Activate();
	//���������ʼ��ת
	PerformRotate();
}

//ִ����ת����
void UAITask_TurnTo::PerformRotate()
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
	//�趨��תĿ��
	if (GoalActor)
		ChessMovementComponent->RotateTarget = FRotateTarget(GoalActor, StopOnDegree);
	else if (GoalNode)
		ChessMovementComponent->RotateTarget = FRotateTarget(GoalNode, StopOnDegree);
	else
		ChessMovementComponent->RotateTarget = FRotateTarget(GoalRotation, StopOnDegree);
	//��ί�У���ת��ɺ��������
	if (!ChessMovementComponent->OnRotateComplete.IsAlreadyBound(this, &UAITask_TurnTo::RequestSuccess))
		ChessMovementComponent->OnRotateComplete.AddDynamic(this, &UAITask_TurnTo::RequestSuccess);
}

//�ɹ��������
void UAITask_TurnTo::RequestSuccess()
{
	if (!OwnerController || !OwnerController->GetPawn())
		return;
	//�Ƴ�ί��
	UChessMovementComponent* ChessMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChessMovementComponent>();
	if (ChessMovementComponent && ChessMovementComponent->OnRotateComplete.IsAlreadyBound(this, &UAITask_TurnTo::RequestSuccess))
		ChessMovementComponent->OnRotateComplete.RemoveDynamic(this, &UAITask_TurnTo::RequestSuccess);
	OnRequestSuccess.Broadcast();
	EndTask();
}

//ʧ���������
void UAITask_TurnTo::RequestFailed()
{
	if (!OwnerController || !OwnerController->GetPawn())
		return;
	//�Ƴ�ί��
	UChessMovementComponent* ChessMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChessMovementComponent>();
	if (ChessMovementComponent && ChessMovementComponent->OnRotateComplete.IsAlreadyBound(this, &UAITask_TurnTo::RequestSuccess))
		ChessMovementComponent->OnRotateComplete.RemoveDynamic(this, &UAITask_TurnTo::RequestSuccess);
	OnRequestFailed.Broadcast();
	EndTask();
}

//ǿ��ִ��
void UAITask_TurnTo::ForceActiveTask()
{
	Activate();
}

//ǿ��ʧ��
void UAITask_TurnTo::ForceEndTask()
{
	RequestSuccess();
}