// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayerController.h"
#include "EngineUtils.h"
#include "GridMap.h"
#include "ChessMovementComponent.h"
#include "AIController.h"
#include "AITask_MoveToNode.h"
#include "AITask_TurnTo.h"

//构造函数
AChessPlayerController::AChessPlayerController()
{
	this->bShowMouseCursor = true;
}

//绑定按键
void AChessPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AChessPlayerController::HandleLeftClick);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AChessPlayerController::HandleRightClick);
}

//获取棋盘
AGridMap* AChessPlayerController::GetBattleMap()
{
	if (BattleMap)
		return BattleMap;
	//使用迭代器遍历
	for (TActorIterator<AGridMap> It(GetWorld()); It; ++It)
	{
		if (!*It)
			continue;
		BattleMap = *It;
		break;
	}
	return BattleMap;
}

//刷新以点击棋格
void AChessPlayerController::FlushCheckNodes()
{
	if (!GetBattleMap())
		return;
	switch (DebugMode)
	{
	case EChessDebugMode::DebugPath:
		//超过2个选中棋格就重置材质
		if (SelectedNodes.Num() >= 2)
		{
			SelectedNodes.Reset();
			BattleMap->ResetNodeMaterialAll();
		}
		break;
	case EChessDebugMode::DebugPawn:
	case EChessDebugMode::DebugTaskMove:
	case EChessDebugMode::DebugTaskRotate:
		//超过1个选中棋格就重置材质
		if (SelectedNodes.Num() >= 1)
		{
			SelectedNodes.Reset();
			BattleMap->ResetNodeMaterialAll();
		}
		break;
	default:
		BattleMap->ResetNodeMaterialAll();
		break;
	}
}

//获取鼠标对应棋盘的位置
FVector AChessPlayerController::GetMouseCursorPosition() const
{
	if (!BattleMap)
		return FVector::ZeroVector;
	FVector WorldLocation, WorldDirection;
	//屏幕坐标转世界坐标，回传坐标和方向
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		//获取Z轴差值
		float tScale = BattleMap->GetActorLocation().Z - WorldLocation.Z;
		tScale /= WorldDirection.Z;
		WorldLocation += tScale * WorldDirection;
		return WorldLocation;
	}
	return FVector::ZeroVector;
}

//左键事件
void AChessPlayerController::HandleLeftClick()
{
	switch (DebugMode)
	{
	case EChessDebugMode::None:
		break;
	case EChessDebugMode::DebugPath:
		HandleDebugPath();
		break;
	case EChessDebugMode::DebugPawn:
		HandleDebugPawn();
	case EChessDebugMode::DebugTaskMove:
		HandleDebugTaskMove();
	case EChessDebugMode::DebugTaskRotate:
		HandleDebugTaskRotate();
	default:
		break;
	}
}

//右键事件
void AChessPlayerController::HandleRightClick()
{
	if (!GetBattleMap())
		return;
	FlushCheckNodes();
	//获取选中棋格
	UGridNode* tHitNode = BattleMap->CheckHitNode(GetMouseCursorPosition());
	if (tHitNode)
	{
		//通行状态切换
		tHitNode->PassFlag = tHitNode->PassFlag == ENodePassFlag::Pass ? ENodePassFlag::Block : ENodePassFlag::Pass;
		BattleMap->ResetNodeMaterial(tHitNode);
	}
}

//Debug寻路
void AChessPlayerController::HandleDebugPath()
{
	if (!GetBattleMap())
		return;
	FlushCheckNodes();
	//获取选中棋格
	UGridNode* tHitNode = BattleMap->CheckHitNode(GetMouseCursorPosition());
	//判断棋格通行状态
	if (tHitNode && tHitNode->PassFlag != ENodePassFlag::Block)
	{
		SelectedNodes.AddUnique(tHitNode);
		BattleMap->SetNodeMaterial(tHitNode, BattleMap->DebugMaterial1);
	}
	//如果选中棋格超过2个，则进行寻路
	if (SelectedNodes.Num() >= 2)
	{
		TArray<UGridNode*> tPath;
		bool bResult = BattleMap->FindPath(tPath, nullptr, SelectedNodes[0], SelectedNodes[1]);
		if (!bResult)
			return;
		for (auto a : tPath)
		{
			//跳过终点材质设置
			if (a == SelectedNodes[1])
				continue;
			//设置路径材质
			BattleMap->SetNodeMaterial(a, BattleMap->DebugMaterial2);
		}
	}
}

//Debug角色
void AChessPlayerController::HandleDebugPawn()
{
	FVector WorldLocation, WorldDirection;
	//获取鼠标点击的角色
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		//射线检测
		FVector Start, End;
		Start = WorldLocation;
		End = Start + WorldDirection * 2000;
		FCollisionQueryParams Params;
		FCollisionObjectQueryParams ObjectParams;
		//碰撞类型选择pawn
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		FHitResult OutHit;
		UWorld* World = GetWorld();
		if (World)
		{
			if (World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParams, Params))
			{
				//设定当前选中角色
				SelectedPawn = Cast<APawn>(OutHit.GetActor());
				return;
			}
		}
	}

	if (SelectedPawn && GetBattleMap())
	{
		FlushCheckNodes();
		//获取移动组件
		UChessMovementComponent* ChessMovementComponent = SelectedPawn->FindComponentByClass<UChessMovementComponent>();
		if (!ChessMovementComponent || !ChessMovementComponent->NowNode)
			return;
		//获取起点，终点
		UGridNode* FromNode = ChessMovementComponent->NowNode;
		UGridNode* ToNode = BattleMap->CheckHitNode(GetMouseCursorPosition());
		if (!FromNode || !ToNode)
			return;
		if (ToNode->PassFlag == ENodePassFlag::Block)
			return;
		SelectedNodes.AddUnique(ToNode);
		//设置材质
		BattleMap->SetNodeMaterial(ToNode, BattleMap->DebugMaterial1);
		TArray<UGridNode*> tPath;
		//进行寻路
		bool bResult = BattleMap->FindPath(tPath, SelectedPawn, FromNode, ToNode);
		if (!bResult)
			return;
		//进行
		ChessMovementComponent->SetMovePath(tPath);

	}
}

//DebugMoveToTask
void AChessPlayerController::HandleDebugTaskMove()
{
	FVector WorldLocation, WorldDirection;
	//获取鼠标点击的角色
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		//射线检测
		FVector Start, End;
		Start = WorldLocation;
		End = Start + WorldDirection * 2000;
		FCollisionQueryParams Params;
		FCollisionObjectQueryParams ObjectParams;
		//碰撞类型选择pawn
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		FHitResult OutHit;
		UWorld* World = GetWorld();
		if (World)
		{
			if (World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParams, Params))
			{
				SelectedPawn = Cast<APawn>(OutHit.GetActor());
				return;
			}
		}
	}

	if (SelectedPawn && GetBattleMap())
	{
		FlushCheckNodes();
		//获取终点
		UGridNode* ToNode = BattleMap->CheckHitNode(GetMouseCursorPosition());
		//获取AIController
		AAIController* AIController = Cast<AAIController>(SelectedPawn->GetController());
		if (!ToNode)
			return;
		if (!AIController)
			return;
		if (ToNode->PassFlag == ENodePassFlag::Block)
			return;
		SelectedNodes.AddUnique(ToNode);
		BattleMap->SetNodeMaterial(ToNode, BattleMap->DebugMaterial1);
		if (NowMoveTask)
		{
			//强制取消上一个Task
			NowMoveTask->ForceEndTask();
		}
		//创建Task
		NowMoveTask = UAITask_MoveToNode::AIMoveTo(AIController, ToNode, nullptr, 0);
		//执行Task
		NowMoveTask->ForceActiveTask();
	}
}

void AChessPlayerController::HandleDebugTaskRotate()
{
	//获取鼠标点击的角色
	FVector WorldLocation, WorldDirection;
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		//射线检测
		FVector Start, End;
		Start = WorldLocation;
		End = Start + WorldDirection * 2000;
		FCollisionQueryParams Params;
		FCollisionObjectQueryParams ObjectParams;
		//碰撞类型选择pawn
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		FHitResult OutHit;
		UWorld* World = GetWorld();
		if (World)
		{
			if (World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParams, Params))
			{
				SelectedPawn = Cast<APawn>(OutHit.GetActor());
				return;
			}
		}
	}

	if (SelectedPawn && GetBattleMap())
	{
		FlushCheckNodes();
		//获取目标点
		UGridNode* ToNode = BattleMap->CheckHitNode(GetMouseCursorPosition());
		AAIController* AIController = Cast<AAIController>(SelectedPawn->GetController());
		//安全判断
		if (!ToNode)
			return;
		if (!AIController)
			return;
		if (ToNode->PassFlag == ENodePassFlag::Block)
			return;
		SelectedNodes.AddUnique(ToNode);
		BattleMap->SetNodeMaterial(ToNode, BattleMap->DebugMaterial1);
		if (NowRotateTask)
		{
			//强制取消上一个Task
			NowRotateTask->ForceEndTask();
		}
		//创建Task
		NowRotateTask = UAITask_TurnTo::AITurnTo(AIController, nullptr, ToNode, FRotator::ZeroRotator, 0);
		//执行Task
		NowRotateTask->ForceActiveTask();
	}
}