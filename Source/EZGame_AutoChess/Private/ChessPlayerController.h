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

//Debugģʽ
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
	//����-������-����
	virtual void SetupInputComponent() override;

public:
	//����-���캯��
	AChessPlayerController();

	//����-��ǰ����
	UPROPERTY(BlueprintReadWrite)
	AGridMap* BattleMap;

	//����-��ѡ�����
	UPROPERTY(BlueprintReadWrite)
	TArray<UGridNode*> SelectedNodes;

	//����-��ѡ�н�ɫ
	UPROPERTY(BlueprintReadWrite)
	APawn* SelectedPawn;

	//����-Debugģʽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EChessDebugMode DebugMode = EChessDebugMode::None;

	//����-��ȡ����
	UFUNCTION(BlueprintCallable)
	AGridMap* GetBattleMap();

	//����-ˢ��ѡ�����
	UFUNCTION()
	void FlushCheckNodes();

	//����-��ȡ����Ӧ���̵�λ��
	UFUNCTION()
	FVector GetMouseCursorPosition() const;

	//����-����¼�
	UFUNCTION()
	void HandleLeftClick();

	//����-�Ҽ��¼�
	UFUNCTION()
	void HandleRightClick();

	//����-DebugѰ·
	UFUNCTION()
	void HandleDebugPath();

	//����-Debug��ɫ
	UFUNCTION()
	void HandleDebugPawn();

	//����-DebugMoveToTask
	UFUNCTION()
	void HandleDebugTaskMove();

	//��ǰMoveToTask
	UAITask_MoveToNode* NowMoveTask;

	//����-DebugTurnToTask
	UFUNCTION()
	void HandleDebugTaskRotate();

	//��ǰTurnToTask
	UAITask_TurnTo* NowRotateTask;

};
