// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridNode.generated.h"

class AGridMap;
class AActor;

/*棋格坐标*/
USTRUCT(BlueprintType)
struct  FGridVector
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int X = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Y = 0;

	FGridVector() {};
	FGridVector(int InX, int InY)
	{
		this->X = InX;
		this->Y = InY;
	}

	//重载运算符==，满足TMap中Key的规范
	FORCEINLINE friend bool operator==(const FGridVector& Lhs, const FGridVector& Rhs)
	{
		return (Lhs.X == Rhs.X) && (Lhs.Y == Rhs.Y);
	}

	//重载运算符+，满足基本加法运算
	FGridVector operator+ (const FGridVector& F)
	{
		return FGridVector(this->X + F.X, this->Y + F.Y);
	}

};

//GetTypeHash，满足TMap中Key的规范
FORCEINLINE uint32 GetTypeHash(const FGridVector& Key)
{
	return HashCombine(GetTypeHash(Key.X), GetTypeHash(Key.Y));
}

/*棋格形状*/
UENUM(BlueprintType)
enum class EGridType : uint8
{
	None,
	Hex,
};

/*棋格通行状态*/
UENUM(BlueprintType)
enum class ENodePassFlag : uint8
{
	Pass,
	Block,
};

UCLASS()
class UGridNode : public UObject
{
	GENERATED_BODY()

public:
	//基础-棋格大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Size;

	//基础-世界空间坐标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	//基础-棋盘空间坐标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGridVector Coordinate;

	//基础-棋格形状
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGridType NodeType = EGridType::None;

	//基础-所属棋盘
	UPROPERTY(BlueprintReadWrite)
	AGridMap* GridMap;

	//基础-初始化函数
	UFUNCTION(BlueprintCallable)
	virtual void InitNode(AGridMap* InGridMap, FVector InLocation, FGridVector InCoordinate, float InSize);

	//基础-获取相邻棋格
	UFUNCTION(BlueprintCallable)
	virtual TArray<UGridNode*> GetNeighbors();

	//寻路-通行状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation")
	ENodePassFlag PassFlag = ENodePassFlag::Pass;

	//寻路-寻路缓存
	UGridNode* PreNode;

	//寻路-判断棋格是否能通行
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual bool CanPass(AActor* InActor) const;

	//寻路-棋格中的单位
	UPROPERTY(BlueprintReadWrite, Category = "Navigation")
	TArray<AActor*> NodeActors;

	//寻路-预定棋格
	UFUNCTION(BlueprintCallable)
	virtual void BookNode(AActor* InActor);

	//寻路-取消预定棋格
	UFUNCTION(BlueprintCallable)
	virtual void CancelBookNode(AActor* InActor);

	//寻路-进入棋格
	UFUNCTION(BlueprintCallable)
	virtual void EnterNode(AActor* InActor);

	//寻路-离开棋格
	UFUNCTION(BlueprintCallable)
	virtual void LeaveNode(AActor* InActor);

	//寻路-获取两两棋格的实际距离
	UFUNCTION(BlueprintCallable)
	virtual float GetRealRadiusSize() const { return Size; };

	//模型-棋格模型索引
	UPROPERTY()
	int MeshIndex;

	//模型-绘制模型
	UFUNCTION()
	virtual void DrawNode(TArray<FVector>& InVertices,
			TArray<int32>& InIndecies,
			TArray<FVector>& InNormals,
			TArray<FVector2D>& InUV,
			TArray<FColor>& InVertexColors,
			TArray<FVector>& InTangents,
			FVector InOffset) {};
	
};
