// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessMovementComponent.h"

//Tick函数
void UChessMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//安全判断
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
	//执行旋转
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
	//安全判断是否有路径
	if (MoveStack.Num() == 0)
	{
		return;
	}
	//拿出顶部，安全判断
	UGridNode* tNode = MoveStack.Top();
	if (!tNode)
	{
		MoveStack.Pop();
		return;
	}
	//如果要移动的棋格不是当前棋格，则预定这个棋格
	if (NowNode!=tNode && BookNode!=tNode)
	{
		SetBookNode(tNode);
	}
	//设置当前Tick移动参数
	FHitResult MoveHit(1.f);
	float tSpeed = FMath::Max(0.0f, MaxMoveSpeed);
	FRotator tRotation = UpdatedComponent->GetComponentRotation();
	FVector tDirection = tNode->Location - ActorOwner->GetActorLocation();
	float tDistance = tDirection.Size2D();
	tDirection.Normalize();
	FVector tMoveDelta;
	//设置MoveDelata
	if (tDistance < DeltaTime * tSpeed)
	{
		tMoveDelta = tNode->Location - ActorOwner->GetActorLocation();
	}
	else
	{
		tMoveDelta = tDirection * tSpeed * DeltaTime;
	}
	//设置RotDelta
	FRotator tRotDelta = FMath::RInterpConstantTo(tRotation, tMoveDelta.Rotation(), DeltaTime, MaxRotationRate);
	//设置当前速度
	Velocity = tMoveDelta / DeltaTime;
	//移动组件
	MoveUpdatedComponent(tMoveDelta, tRotDelta, false, &MoveHit, ETeleportType::None);
	//计算当前所在棋格
	CalcuNowNode();
	//到达中间路点
	if ((ActorOwner->GetActorLocation() - tNode->Location).IsNearlyZero())
	{
		//弹出顶栈
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

//组件销毁
void UChessMovementComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (BookNode)
		BookNode->CancelBookNode(GetOwner());
	if (NowNode)
		NowNode->LeaveNode(GetOwner());
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

//立刻停止
void UChessMovementComponent::StopMovementImmediately()
{
	if (BookNode)
		BookNode->CancelBookNode(GetOwner());
	MoveStack.Empty();
	RotateTarget.Reset();
	Super::StopMovementImmediately();
}

//设置当前棋格
void UChessMovementComponent::SetNowNode(UGridNode* InNode)
{
	//取消当前预定棋格
	if (BookNode)
		SetBookNode(nullptr);
	//离开当前棋格
	if (NowNode)
		NowNode->LeaveNode(GetOwner());
	//设置当前棋格
	NowNode = InNode;
	//进行进入
	if(NowNode)
		NowNode->EnterNode(GetOwner());
}

//设置预定棋格
void UChessMovementComponent::SetBookNode(UGridNode* InNode)
{
	//取消当前预定棋格
	if (BookNode)
		BookNode->CancelBookNode(GetOwner());
	//设置当前预定棋格
	BookNode = InNode;
	//进行预定
	if(BookNode)
		BookNode->BookNode(GetOwner());
}

//计算当前所在棋格
void UChessMovementComponent::CalcuNowNode()
{
	//安全判断
	AActor* ActorOwner = UpdatedComponent->GetOwner();
	if (!ActorOwner || (ActorOwner->IsPendingKill()))
		return;
	if (MoveStack.Num() == 0)
		return;
	UGridNode* tNode = MoveStack.Top();
	if (!tNode)
		return;
	//获取角色位置到当前和下一个棋格的位置
	float tDistance = FVector::Dist2D(ActorOwner->GetActorLocation(), tNode->Location);
	float uDistance = tNode->GetRealRadiusSize();
	//如果举一例位置小于实际半径的距离，则算进入了棋格
	if (tDistance <= uDistance)
	{
		SetNowNode(tNode);
	}

}

//设置移动路径
void UChessMovementComponent::SetMovePath(TArray<UGridNode*> InPath, bool bNeedReverse)
{
	if (bNeedReverse)
	{
		Algo::Reverse(InPath);
	}
	MoveStack = InPath;
	//开始移动，广播开始移动委托
	if (MoveStack.Num()>0)
	{
		SetBookNode(MoveStack.Top());
		OnMoveBegin.Broadcast();
	}
}


