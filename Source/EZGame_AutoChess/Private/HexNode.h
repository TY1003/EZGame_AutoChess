// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNode.h"
#include "HexNode.generated.h"

/**
 * 
 */
UCLASS()
class UHexNode : public UGridNode
{
	GENERATED_BODY()
	
public:
	//基础-构造函数
	UHexNode();

	//基础-右上棋格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* RightUpNode;

	//基础-右棋格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* RightNode;

	//基础-右下棋格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* RightDownNode;

	//基础-左下棋格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* LeftDownNode;

	//基础-左棋格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* LeftNode;

	//基础-左上棋格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* LeftUpNode;

	//基础-获取相邻棋格-覆盖
	virtual TArray<UGridNode*> GetNeighbors() override;

	//寻路-获取两两棋格的实际距离-覆盖
	virtual float GetRealRadiusSize() const override { return Size * FMath::Sqrt(3) * 0.5; };

	//模型-绘制模型-覆盖
	virtual void DrawNode(TArray<FVector>& InVertices,
		TArray<int32>& InIndecies,
		TArray<FVector>& InNormals,
		TArray<FVector2D>& InUV,
		TArray<FColor>& InVertexColors,
		TArray<FVector>& InTangents,
		FVector InOffset) override;

};
