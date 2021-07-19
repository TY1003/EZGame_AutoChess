// Fill out your copyright notice in the Description page of Project Settings.


#include "GridNode.h"
#include "GridMap.h"

//初始化函数
void UGridNode::InitNode(AGridMap* InGridMap, FVector InLocation, FGridVector InCoordinate, float InSize)
{
	GridMap = InGridMap;
	Location = InLocation;
	Coordinate = InCoordinate;
	Size = InSize;
}

//获取相邻棋格
TArray<UGridNode*> UGridNode::GetNeighbors()
{
	TArray<UGridNode*> Nodes;
	return Nodes;
}

//判断棋格是否能通行
bool UGridNode::CanPass(AActor* InActor) const
{
	if (PassFlag >= ENodePassFlag::Block)
		return false;
	//判断是否有其他棋子站在当前棋格上
	if (NodeActors.Num() > 0 && InActor)
	{
		if (NodeActors.Contains(InActor))
			return true;
		else
			return false;
	}
	return true;
}

//预定棋格
void UGridNode::BookNode(AActor* InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);
		GridMap->ResetNodeMaterial(this);
	}
}

//取消预定棋格
void UGridNode::CancelBookNode(AActor* InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);
		GridMap->ResetNodeMaterial(this);
	}
}

//进入棋格
void UGridNode::EnterNode(AActor* InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);
		GridMap->ResetNodeMaterial(this);
	}
}

//离开棋格
void UGridNode::LeaveNode(AActor* InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);
		GridMap->ResetNodeMaterial(this);
	}
}
