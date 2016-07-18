#pragma once
//ChessGame类提供了游戏的评估、走棋和初始化方法
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
	void NewGame(); //初始所有数据
	void ChessFull(); //所有可下棋子到factionChess
	void NextStep(); //下下一步棋子的类型选择
	void SetBorder(int, int); //设置棋盘边界（位图像素大小)
	void SetColor(); //初始化棋子颜色
	void SetFactionChess(int*, int);
	void InitChess(); //棋局恢复成开局样式
	void SetMoreWalkPoint(int, int, std::vector<int>&); //获取更多路径点
	void SetWorkOrder(std::vector<int>&); //设置机械臂运行指令;

private:
	void FindStraightPlace(Chess * pChess, int direct); //查找所有一次跳跃点并生成有向图
	void FindAvailablePlace(Chess *pChess, int direct); //加当前棋子一方到factionChess
	void FindMoreAP(Chess *pChess); //加一个棋子到factionChess
	void AddBesidePlace(Chess *pChess); //添加临近可下棋子到factionChess
	void Renovate(Chess *pChess1, Chess *pChess2); //下子后更新视图
	int Point2Number(CPoint point); //返回坐标对应棋格
	int Evaluate(int i, int alphaBeataMax1, int alphaBeataMin1, int alphaBeataMax2, int alphaBeataMin2, int alphaBeataMax3);

	int PCStep(); //计算机下子

	void SetXY(); //初始化棋子对应BMP坐标
	void SetPlace(); //初始化棋子所在区域
	void SetnPRI(); //初始化fullChess数组 score;
	void SetNumber(); //初始化棋子编号
	void SetAround(); //初始化棋盘，棋子六角各位哪个棋子
	void SetPos(); //初始化棋子pos电机属性
	int PreEvaluate(int player, int alternativeSocore[5], int alternativeChess[5][3]);
	int getMin(int *alternativeSocore, int length);
	void SetFactionChess();  //初始化factionChess数组;
	void SetDigraph(int);

public:
	Chess fullChess[122]; //全部棋子[1-121],[0]为空;
	Faction factionChess[6][10];
	//六方棋子属性;
	Faction * pChess; //当前操作棋子;
	CWnd *m_cwnd; //当前视指针
	int faction, fChessNumber; //factionChess的二维下标全局变量
	int playerType[6]; //电脑 2,人 1,无 0;	
	int currentPlayer; //当前下子为几
	BOOL beginGame; //是否开始下子
	int count;
	int right, bottom; //背景边界像素
	int digraph[122][122];
};