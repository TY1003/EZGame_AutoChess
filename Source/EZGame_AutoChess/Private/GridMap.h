// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridNode.h"
#include "GridMap.generated.h"

class UProceduralMeshComponent;
class UMaterial;

UCLASS(hideCategories = (Materials))
class AGridMap : public AActor
{
	GENERATED_BODY()

protected:
	//����-BeginPlay-����
	virtual void BeginPlay() override;

	//����-��ʼ������-����
	virtual void PostInitProperties() override;

#if WITH_EDITOR
	//����-��Editor���޸�����-����
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	//����-���캯��
	AGridMap();

	//����-������״
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGridType MapType = EGridType::None;

	//����-���̵�λ��С
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Size;

	//����-��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Roll;

	//����-��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Colume;

	//����-���̵����
	UPROPERTY()
	TMap<FGridVector, UGridNode*> NodeMap;

	//����-��������
	UFUNCTION(BlueprintCallable)
	void GenerateGridMap();

	//����-����������
	UFUNCTION()
	void GenerateNodes(float InSize, int InRoll, int InColume);

	//����-�������������
	UFUNCTION()
	void GenerateHexNodes(float InHexSize, int InRoll, int InColume);

	//����-��ʼ��������
	UFUNCTION()
	void InitNodes();

	//����-��ʼ�����������
	UFUNCTION()
	void InitHexNodes();

	//����-��ȡ�������
	UFUNCTION(BlueprintCallable)
	UGridNode* GetNode(FGridVector InCoord) const;

	//Ѱ·-A*Ѱ��·��
	UFUNCTION(BlueprintCallable)
	bool FindPath(TArray<UGridNode*>& Path, AActor* InActor, UGridNode* FromNode, UGridNode* ToNode, int StopSteps = 0);

	//Ѱ·-�Ƿ����·��
	UFUNCTION(BlueprintCallable)
	bool IsPathExist(AActor* InActor, UGridNode* FromNode, UGridNode* ToNode, int StopSteps = 0);

	//Ѱ·-��ȡĿ��㷶Χ�ڵ����
	UFUNCTION()
	TArray<UGridNode*> GetNodeNeighbors(UGridNode* InNode, int InStep = 0) const;

	//ģ��-����ģ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Mesh;

	//ģ��-ͨ�в���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* PassMaterial;

	//ģ��-�赲����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BlockMaterial;

	//ģ��-�赲����1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DebugMaterial1;

	//ģ��-�赲����2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DebugMaterial2;

	//ģ��-��������ģ��
	UFUNCTION(BlueprintCallable)
	void GenerateMapMesh();

	//ģ��-����������
	UFUNCTION(BlueprintCallable)
	void SetNodeMaterial(UGridNode* InNode, UMaterialInterface* InMaterial);

	//ģ��-����������
	UFUNCTION(BlueprintCallable)
	void ResetNodeMaterial(UGridNode* InNode);

	//ģ��-��������������
	UFUNCTION(BlueprintCallable)
	void ResetNodeMaterialAll();

	//����-�ж��Ƿ��������
	UFUNCTION(BlueprintCallable)
	UGridNode* CheckHitNode(FVector InPosition);

	//����-�ж��Ƿ��������������
	UFUNCTION()
	UGridNode* CheckHitHexNode(FVector InPosition);
};
