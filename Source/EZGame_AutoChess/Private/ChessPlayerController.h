// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChessPlayerController.generated.h"

class UGridNode;
class AGridMap;
class APawn;
class UAITask_MoveToNode;
class UAITask_TurnTo;

//Debug模式
UENUM(BlueprintType)
enum class EChessDebugMode : uint8
{
	None,
	DebugPath,
	DebugPawn,
	DebugTaskMove,
	DebugTaskRotate,
};

/**
 * 
 */
UCLASS()
class AChessPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//基础-绑定输入-覆盖
	virtual void SetupInputComponent() override;

public:
	//基础-构造函数
	AChessPlayerController();

	//基础-当前棋盘
	UPROPERTY(BlueprintReadWrite)
	AGridMap* BattleMap;

	//基础-已选中棋格
	UPROPERTY(BlueprintReadWrite)
	TArray<UGridNode*> SelectedNodes;

	//基础-已选中角色
	UPROPERTY(BlueprintReadWrite)
	APawn* SelectedPawn;

	//基础-Debug模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EChessDebugMode DebugMode = EChessDebugMode::None;

	//基础-获取棋盘
	UFUNCTION(BlueprintCallable)
	AGridMap* GetBattleMap();

	//基础-刷新选中棋格
	UFUNCTION()
	void FlushCheckNodes();

	//基础-获取鼠标对应棋盘的位置
	UFUNCTION()
	FVector GetMouseCursorPosition() const;

	//基础-左键事件
	UFUNCTION()
	void HandleLeftClick();

	//基础-右键事件
	UFUNCTION()
	void HandleRightClick();

	//基础-Debug寻路
	UFUNCTION()
	void HandleDebugPath();

	//基础-Debug角色
	UFUNCTION()
	void HandleDebugPawn();

	//基础-DebugMoveToTask
	UFUNCTION()
	void HandleDebugTaskMove();

	//当前MoveToTask
	UAITask_MoveToNode* NowMoveTask;

	//基础-DebugTurnToTask
	UFUNCTION()
	void HandleDebugTaskRotate();

	//当前TurnToTask
	UAITask_TurnTo* NowRotateTask;

};
