// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessMovementComponent.h"

//Tick����
void UChessMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//��ȫ�ж�
	if (!UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}
	AActor* ActorOwner = UpdatedComponent->GetOwner();
	if (!ActorOwner || (ActorOwner->IsPendingKill()))
	{
		return;
	}
	if (UpdatedComponent->IsSimulatingPhysics())
	{
		return;
	}
	//ִ����ת
	if (RotateTarget.IsValid())
	{
		FHitResult RotHit(1.f);
		FRotator tFromRotation = UpdatedComponent->GetComponentRotation();
		FRotator tToRoation = RotateTarget.GetRotation(UpdatedComponent->GetComponentLocation());
		tFromRotation.Pitch = 0; tFromRotation.Roll = 0;
		tToRoation.Pitch = 0; tToRoation.Roll = 0;
		if (FMath::Abs(tFromRotation.Yaw - tToRoation.Yaw) <= FMath::Max(RotateTarget.StopOnDegree, 0.1f))
		{
			RotateTarget.Reset();
			OnRotateComplete.Broadcast();
		}
		else
		{
			FRotator tNewRotation = FMath::RInterpConstantTo(tFromRotation, tToRoation, DeltaTime, MaxRotationRate);
			MoveUpdatedComponent(FVector::ZeroVector, tNewRotation, false, &RotHit, ETeleportType::None);
		}
		return;
	}
	//��ȫ�ж��Ƿ���·��
	if (MoveStack.Num() == 0)
	{
		return;
	}
	//�ó���������ȫ�ж�
	UGridNode* tNode = MoveStack.Top();
	if (!tNode)
	{
		MoveStack.Pop();
		return;
	}
	//���Ҫ�ƶ�������ǵ�ǰ�����Ԥ��������
	if (NowNode!=tNode && BookNode!=tNode)
	{
		SetBookNode(tNode);
	}
	//���õ�ǰTick�ƶ�����
	FHitResult MoveHit(1.f);
	float tSpeed = FMath::Max(0.0f, MaxMoveSpeed);
	FRotator tRotation = UpdatedComponent->GetComponentRotation();
	FVector tDirection = tNode->Location - ActorOwner->GetActorLocation();
	float tDistance = tDirection.Size2D();
	tDirection.Normalize();
	FVector tMoveDelta;
	//����MoveDelata
	if (tDistance < DeltaTime * tSpeed)
	{
		tMoveDelta = tNode->Location - ActorOwner->GetActorLocation();
	}
	else
	{
		tMoveDelta = tDirection * tSpeed * DeltaTime;
	}
	//����RotDelta
	FRotator tRotDelta = FMath::RInterpConstantTo(tRotation, tMoveDelta.Rotation(), DeltaTime, MaxRotationRate);
	//���õ�ǰ�ٶ�
	Velocity = tMoveDelta / DeltaTime;
	//�ƶ����
	MoveUpdatedComponent(tMoveDelta, tRotDelta, false, &MoveHit, ETeleportType::None);
	//���㵱ǰ�������
	CalcuNowNode();
	//�����м�·��
	if ((ActorOwner->GetActorLocation() - tNode->Location).IsNearlyZero())
	{
		//������ջ
		MoveStack.Pop();
		if (MoveStack.Num() == 0)
		{
			Velocity = FVector::ZeroVector;
			OnMoveComplete.Broadcast();
			return;
		}
		else
		{
			OnMoveCheckPoint.Broadcast();
		}
	}
}

//�������
void UChessMovementComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (BookNode)
		BookNode->CancelBookNode(GetOwner());
	if (NowNode)
		NowNode->LeaveNode(GetOwner());
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

//����ֹͣ
void UChessMovementComponent::StopMovementImmediately()
{
	if (BookNode)
		BookNode->CancelBookNode(GetOwner());
	MoveStack.Empty();
	RotateTarget.Reset();
	Super::StopMovementImmediately();
}

//���õ�ǰ���
void UChessMovementComponent::SetNowNode(UGridNode* InNode)
{
	//ȡ����ǰԤ�����
	if (BookNode)
		SetBookNode(nullptr);
	//�뿪��ǰ���
	if (NowNode)
		NowNode->LeaveNode(GetOwner());
	//���õ�ǰ���
	NowNode = InNode;
	//���н���
	if(NowNode)
		NowNode->EnterNode(GetOwner());
}

//����Ԥ�����
void UChessMovementComponent::SetBookNode(UGridNode* InNode)
{
	//ȡ����ǰԤ�����
	if (BookNode)
		BookNode->CancelBookNode(GetOwner());
	//���õ�ǰԤ�����
	BookNode = InNode;
	//����Ԥ��
	if(BookNode)
		BookNode->BookNode(GetOwner());
}

//���㵱ǰ�������
void UChessMovementComponent::CalcuNowNode()
{
	//��ȫ�ж�
	AActor* ActorOwner = UpdatedComponent->GetOwner();
	if (!ActorOwner || (ActorOwner->IsPendingKill()))
		return;
	if (MoveStack.Num() == 0)
		return;
	UGridNode* tNode = MoveStack.Top();
	if (!tNode)
		return;
	//��ȡ��ɫλ�õ���ǰ����һ������λ��
	float tDistance = FVector::Dist2D(ActorOwner->GetActorLocation(), tNode->Location);
	float uDistance = tNode->GetRealRadiusSize();
	//�����һ��λ��С��ʵ�ʰ뾶�ľ��룬������������
	if (tDistance <= uDistance)
	{
		SetNowNode(tNode);
	}

}

//�����ƶ�·��
void UChessMovementComponent::SetMovePath(TArray<UGridNode*> InPath, bool bNeedReverse)
{
	if (bNeedReverse)
	{
		Algo::Reverse(InPath);
	}
	MoveStack = InPath;
	//��ʼ�ƶ����㲥��ʼ�ƶ�ί��
	if (MoveStack.Num()>0)
	{
		SetBookNode(MoveStack.Top());
		OnMoveBegin.Broadcast();
	}
}


