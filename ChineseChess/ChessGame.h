#pragma once
//ChessGame���ṩ����Ϸ������������ͳ�ʼ������
//

#include "Chess.h"
#include "stdafx.h"

#define BLANK 0
#define HUMANSTEP 1
#define PCSTEP 2

class ChessGame 
{
public:
	ChessGame();
	virtual ~ChessGame();

public:
	void FindAvailablePlace(Chess *pChess, int direct); //�ӵ�ǰ����һ����factionChess
	void FindMoreAP(Chess *pChess); //��һ�����ӵ�factionChess
	void ChessFull(); //���п������ӵ�factionChess
	void AddBesidePlace(Chess *pChess); //�����ٽ��������ӵ�factionChess
	void Renovate(Chess *pChess1, Chess *pChess2); //���Ӻ������ͼ
	int Point2Number(CPoint point); //���������Ӧ���
	int Evaluate(int i, int alphaBeataMax1, int alphaBeataMin1, int alphaBeataMax2, int alphaBeataMin2, int alphaBeataMax3);

	int PCStep(); //���������
	void NextStep(); //����һ�����ӵ�����ѡ��
	void NewGame(); //��ʼ��������

	void SetXY(); //��ʼ�����Ӷ�ӦBMP����
	void SetPlace(); //��ʼ��������������
	void SetColor(); //��ʼ��������ɫ
	void SetnPRI(); //��ʼ��fullChess���� score;
	void SetNumber(); //��ʼ�����ӱ��
	void SetAround(); //��ʼ�����̣��������Ǹ�λ�ĸ�����
	void SetPos(); //��ʼ������pos�������
	int PreEvaluate(int player, int alternativeSocore[5], int alternativeChess[5][3]);
	int getMin(int *alternativeSocore, int length);
	void SetFactionChess();  //��ʼ��factionChess����;

public:
	Chess fullChess[122]; //ȫ������[1-121],[0]Ϊ��;
	Faction factionChess[6][10];
	//������������;
	Faction * pChess; //��ǰ��������;
	CWnd *m_cwnd; //��ǰ��ָ��
	int faction, fChessNumber; //factionChess�Ķ�ά�±�ȫ�ֱ���
	int playerType[6]; //���� 2,�� 1,�� 0;	
	int currentPlayer; //��ǰ����Ϊ��
	BOOL beginGame; //�Ƿ�ʼ����
	int count;
};