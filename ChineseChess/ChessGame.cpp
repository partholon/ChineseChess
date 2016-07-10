// ChessGame.cpp : 实现文件
//

#include "stdafx.h"
#include "ChessGame.h"
#include "com_class.h"
#include <fstream>

extern vector<CString> gOrder;
extern CString gZero;

ChessGame::ChessGame()
{
	currentPlayer = 0;
	pChess = NULL;
	beginGame = FALSE;

	//初始化玩家棋盘
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {
			factionChess[i][j].number = 0;
			factionChess[i][j].upOrDown = DOWN;
		}
		playerType[i] = 0;
	}

	//将所有的棋盘上的位置置空
	memset(fullChess, 0, sizeof(fullChess));

	srand((unsigned)time(NULL));

	SetNumber();
	SetXY();
	SetAround();
	SetnPRI();
	SetFactionChess();
	SetPlace();
	SetPos();
	SetColor();
}

ChessGame::~ChessGame()
{
}

//初始化fullChess数组 x,y;
//初始化棋子对应BMP坐标
void ChessGame::SetXY()
{
	//定义棋盘左边界棋子编号
	int leftNumber[18] = { 1,2,4,7,11,24,36,47,57,66,76,87,99,112,116,119,121,122 };
	//定义棋盘左边界棋子坐标
	int xPostion[17] = { (int)right*0.344, (int)right*0.330, (int)right*0.316, (int)right*0.302,
		(int)right*0.174, (int)right*0.188, (int)right*0.202, (int)right*0.216, (int)right*0.230,
		(int)right*0.216, (int)right*0.202, (int)right*0.188, (int)right*0.174,
		(int)right*0.302, (int)right*0.316, (int)right*0.330, (int)right*0.344 };
	int firstNumber=0,  lastNumber= 1;//该行起始标号、终止标号
	int x, y;//1号棋子y坐标
	float xSpace = right*0.0285;
	float ySpace = bottom*0.0438;

	for (int i = 0; i <= 16; i++) {
		firstNumber = leftNumber[i];
		lastNumber = leftNumber[i + 1];
		y = (int)bottom*0.176 + (int)i*ySpace;
		int tmp = 0;
		for (int j = firstNumber; j < lastNumber; j++) {
			x = xPostion[i] + (int)tmp*xSpace;//两棋子中心间距
			fullChess[j].x = x;
			fullChess[j].y = y;
			tmp++;
		}
	}

	return;
}

//初始化fullChess数组 place;
//初始化棋子所在区域
void ChessGame::SetPlace()
{
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {
			fullChess[factionChess[i][j].number].place = i + 1;
		}
	}
	return;
}

//初始化fullChess数组 color;
//初始化棋子颜色
void ChessGame::SetColor()
{
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {
			fullChess[factionChess[i][j].number].color = i + 1;
		}
	}
	return;
}

//设置由摄像头传来的棋子位置
void ChessGame::SetFactionChess(int* Start_Pos,int faction)
{
	for (int i = 0; i < 10; i++) {
		fullChess[factionChess[faction][i].number].color = 0;
	}
	for (int i = 0; i < 10; i++) {
		factionChess[faction][i].number = 122 - Start_Pos[i];
	}
}

//自动将棋盘恢复到开始状态
void ChessGame::InitChess()
{
	int origin, target;
	int faction = currentPlayer;
	for (int i = 0; i < 10; i++) {
		if (faction+1 != fullChess[factionChess[faction][i].number].place) {
			for (int j = 1; j < 11; j++) {
				if (0 == fullChess[j].color) {
					//移动factionChess[faction][i].number]-》j
					origin = factionChess[faction][i].number;
					target = j;
					fullChess[origin].color = 0;
					factionChess[faction][i].number = target;
					fullChess[target].color = faction + 1;
					CString orderTemp;
					orderTemp.Format("gspd1500;adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
					gOrder.push_back(orderTemp);
					//orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[origin].pos1, fullChess[origin].pos2, fullChess[origin].pos3);
					orderTemp = "gspd800;gstp500;";
					gOrder.push_back(orderTemp);
					gOrder.push_back("x1c");
					//orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
					orderTemp = "gstp-500;";
					gOrder.push_back(orderTemp);
					orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd1500;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
					gOrder.push_back(orderTemp);
					//orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[target].pos1, fullChess[target].pos2, fullChess[target].pos3);
					orderTemp = "gstp500;";
					gOrder.push_back(orderTemp);
					gOrder.push_back("x3c");
					//orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
					orderTemp = "gstp-500;";
					gOrder.push_back(orderTemp);
					gOrder.push_back(gZero);
					break;
				}
			}
			if (faction + 1 == fullChess[factionChess[faction][i].number].place) {
				continue;
			}
			for (int j = 1; j < 11;j++){
				if (faction+1 != fullChess[j].color) {
					//移动j-》任意空白（优先对面空白）
					for (int k = 112; k < 122; k++) {
						if (0 == fullChess[k].color) {
							//移动j->k 任意空白
							//factionChess[faction][i].number->j;
							origin = j;
							target = k;
							fullChess[target].color = fullChess[origin].color;
							fullChess[origin].color = 0;
							for (int i = 0; i < 10; i++) {
								if (factionChess[faction + 3][i].number == origin) {
									factionChess[faction + 3][i].number = target;
									break;
								}
							}
							CString orderTemp;
							orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd1500;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
							gOrder.push_back(orderTemp);
							orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[origin].pos1, fullChess[origin].pos2, fullChess[origin].pos3);
							gOrder.push_back(orderTemp);
							gOrder.push_back("x1c");
							orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
							gOrder.push_back(orderTemp);
							orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd1500;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
							gOrder.push_back(orderTemp);
							orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[target].pos1, fullChess[target].pos2, fullChess[target].pos3);
							gOrder.push_back(orderTemp);
							gOrder.push_back("x3c");
							orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
							gOrder.push_back(orderTemp);
							gOrder.push_back(gZero);
							break;
						}
					}
					if (0 != fullChess[j].color) {
						for (int k = 11; k < 112; k++) {
							if (0 == fullChess[k].color) {
								//移动j->k 任意空白
								origin = j;
								target = k;
								fullChess[origin].color = 0;
								factionChess[faction][i].number = target;
								fullChess[target].color = faction + 1;;
								CString orderTemp;
								orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd1500;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
								gOrder.push_back(orderTemp);
								orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[origin].pos1, fullChess[origin].pos2, fullChess[origin].pos3);
								gOrder.push_back(orderTemp);
								gOrder.push_back("x1c");
								orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
								gOrder.push_back(orderTemp);
								orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd1500;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
								gOrder.push_back(orderTemp);
								orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[target].pos1, fullChess[target].pos2, fullChess[target].pos3);
								gOrder.push_back(orderTemp);
								gOrder.push_back("x3c");
								orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
								gOrder.push_back(orderTemp);
								gOrder.push_back(gZero);
								break;
							}
						}
					}
					if(0 == fullChess[j].color) {
						//移动factionChess[faction][i].number]-》j
						origin = factionChess[faction][i].number;
						target = j;
						fullChess[origin].color = 0;
						factionChess[faction][i].number = target;
						fullChess[target].color = faction + 1;;
						CString orderTemp;
						orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd800;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
						gOrder.push_back(orderTemp);
						orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[origin].pos1, fullChess[origin].pos2, fullChess[origin].pos3);
						gOrder.push_back(orderTemp);
						gOrder.push_back("x1c");
						orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
						gOrder.push_back(orderTemp);
						orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd800;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
						gOrder.push_back(orderTemp);
						orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[target].pos1, fullChess[target].pos2, fullChess[target].pos3);
						gOrder.push_back(orderTemp);
						gOrder.push_back("x3c");
						orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
						gOrder.push_back(orderTemp);
						gOrder.push_back(gZero);
						break;
					}
				}
			}
		}
	}

	faction = currentPlayer + 3;
	for (int i = 0; i < 10; i++) {
		if (faction + 1 != fullChess[factionChess[faction][i].number].place) {
			for (int j = 112; j < 122; j++) {
				if (0 == fullChess[j].color) {
					//移动factionChess[faction][i].number]->j
					origin = factionChess[faction][i].number;
					target = j;
					fullChess[origin].color = 0;
					factionChess[faction][i].number = target;
					fullChess[target].color = faction + 1;;
					CString orderTemp;
					orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd800;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
					gOrder.push_back(orderTemp);
					orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[origin].pos1, fullChess[origin].pos2, fullChess[origin].pos3);
					gOrder.push_back(orderTemp);
					gOrder.push_back("x1c");
					orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
					gOrder.push_back(orderTemp);
					orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd800;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
					gOrder.push_back(orderTemp);
					orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[target].pos1, fullChess[target].pos2, fullChess[target].pos3);
					gOrder.push_back(orderTemp);
					gOrder.push_back("x3c");
					orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
					gOrder.push_back(orderTemp);
					gOrder.push_back(gZero);
					break;
				}
			}
		}
	}
}

//初始化fullChess数组 score;
//分数是以到各个顶点的距离算的
void ChessGame::SetnPRI()
{
	Chess *pChessData;
	Chess *pChessTemp;
	int peak[6] = { 1,11,99,121,111,23 };//六角顶点，逆时针
	for (int i = 0; i < 3; i++) {
		pChessData = &fullChess[peak[i]];
		for (int j = 1; j < 14; j++) {
			pChessTemp = pChessData;
			pChessTemp->score[i] = j;
			pChessTemp->score[i + 3] = 18 - j;
			//向左寻找
			while (pChessTemp->around[i + 2]) {
				pChessTemp = pChessTemp->around[i + 2];
				pChessTemp->score[i] = j;
				pChessTemp->score[i + 3] = 18 - j;
			}
			pChessTemp = pChessData;
			//向右寻找
			while (pChessTemp->around[(i + 5) % 6]) {
				pChessTemp = pChessTemp->around[(i + 5) % 6];
				pChessTemp->score[i] = j;
				pChessTemp->score[i + 3] = 18 - j;
			}
			//找到下一列
			pChessData = pChessData->around[i + 3];
		}

		pChessData = &fullChess[peak[i + 3]];
		for (int j = 1; j < 5; j++) {
			pChessTemp = pChessData;
			pChessTemp->score[i + 3] = j;
			pChessTemp->score[i] = 18 - j;
			while (pChessTemp->around[i + 2]) {
				pChessTemp = pChessTemp->around[i + 2];
				pChessTemp->score[i + 3] = j;
				pChessTemp->score[i] = 18 - j;
			}
			pChessTemp = pChessData;
			while (pChessTemp->around[(i + 5) % 6]) {
				pChessTemp = pChessTemp->around[(i + 5) % 6];
				pChessTemp->score[i + 3] = j;
				pChessTemp->score[i] = 18 - j;
			}
			pChessData = pChessData->around[i];
		}
	}

	return;
}

//初始化fullChess数组 number;
//初始化棋子编号
void ChessGame::SetNumber()
{
	for (int i = 0; i < 122; i++) {
		fullChess[i].number = i;
	}
	return;
}

//初始化fullChess数组 around;
//初始化棋盘，棋子六角各位哪个棋子
void ChessGame::SetAround()
{
	//棋子六个方向上的棋子，从右上方开始，以逆时针方向旋转
	int temp[6][121] = {
		0,
		1, 0,
		2, 3, 0,
		4, 5, 6, 0,
		0, 0, 0, 0, 7, 8, 9, 10, 0, 0, 0, 0, 0,
		12,13,14,15,16,17,18,19,20,21,22,23,
		25,26,27,28,29,30,31,32,33,34,35,
		37,38,39,40,41,42,43,44,45,46,
		48,49,50,51,52,53,54,55,56,
		57,58,59,60,61,62,63,64,65,0,
		66,67,68,69,70,71,72,73,74,75,0,
		76,77,78,79,80,81,82,83,84,85,86,0,
		87,88,89,90,91,92,93,94,95,96,97,98,0,
		104,105,106,107,
		113,114,115,
		117,118,
		120,

		0,
		0,1,
		0,2,3,
		0,4,5,6,
		0,0,0,0,0,7,8,9,10,0,0,0,0,
		11,12,13,14,15,16,17,18,19,20,21,22,
		24,25,26,27,28,29,30,31,32,33,34,
		36,37,38,39,40,41,42,43,44,45,
		47,48,49,50,51,52,53,54,55,
		0,57,58,59,60,61,62,63,64,65,
		0,66,67,68,69,70,71,72,73,74,75,
		0,76,77,78,79,80,81,82,83,84,85,86,
		0,87,88,89,90,91,92,93,94,95,96,97,98,
		103,104,105,106,
		112,113,114,
		116,117,
		119,

		0,
		0,2,
		0,4,5,
		0,7,8,9,
		0,11,12,13,14,15,16,17,18,19,20,21,22,
		0,24,25,26,27,28,29,30,31,32,33,34,
		0,36,37,38,39,40,41,42,43,44,45,
		0,47,48,49,50,51,52,53,54,55,
		0,57,58,59,60,61,62,63,64,
		0,66,67,68,69,70,71,72,73,74,
		0,76,77,78,79,80,81,82,83,84,85,
		0,87,88,89,90,91,92,93,94,95,96,97,
		0,99,100,101,102,103,104,105,106,107,108,109,110,
		0,112,113,114,
		0,116,117,
		0,119,
		0,

		2,
		4,5,
		7,8,9,
		15,16,17,18,
		0,24,25,26,27,28,29,30,31,32,33,34,35,
		0,36,37,38,39,40,41,42,43,44,45,46,
		0,47,48,49,50,51,52,53,54,55,56,
		0,57,58,59,60,61,62,63,64,65,
		66,67,68,69,70,71,72,73,74,
		76,77,78,79,80,81,82,83,84,85,
		87,88,89,90,91,92,93,94,95,96,97,
		99,100,101,102,103,104,105,106,107,108,109,110,
		0,0,0,0,0,112,113,114,115,0,0,0,0,
		0,116,117,118,
		0,119,120,
		0,121,
		0,

		3,
		5,6,
		8,9,10,
		16,17,18,19,
		24,25,26,27,28,29,30,31,32,33,34,35,0,
		36,37,38,39,40,41,42,43,44,45,46,0,
		47,48,49,50,51,52,53,54,55,56,0,
		57,58,59,60,61,62,63,64,65,0,
		67,68,69,70,71,72,73,74,75,
		77,78,79,80,81,82,83,84,85,86,
		88,89,90,91,92,93,94,95,96,97,98,
		100,101,102,103,104,105,106,107,108,109,110,111,
		0,0,0,0,112,113,114,115,0,0,0,0,0,
		116,117,118,0,
		119,120,0,
		121,0,
		0,

		0,
		3,0,
		5,6,0,
		8,9,10,0,
		12,13,14,15,16,17,18,19,20,21,22,23,0,
		25,26,27,28,29,30,31,32,33,34,35,0,
		37,38,39,40,41,42,43,44,45,46,0,
		48,49,50,51,52,53,54,55,56,0,
		58,59,60,61,62,63,64,65,0,
		67,68,69,70,71,72,73,74,75,0,
		77,78,79,80,81,82,83,84,85,86,0,
		88,89,90,91,92,93,94,95,96,97,98,0,
		100,101,102,103,104,105,106,107,108,109,110,111,0,
		113,114,115,0,
		117,118,0,
		120,0,
		0 };

		for (int i = 1; i < 122; i++) {
			for (int direct = 0; direct < 6; direct++) {
				if (temp[direct][i-1]) {
					fullChess[i].around[direct] = &fullChess[temp[direct][i-1]];
				}
			}
		}

		return;
}

//初始化fullChess数组 pos;
//初始化棋子pos电机属性
void ChessGame::SetPos()
{
	fstream fin;
	fin.open("./pair.txt", ios::_Nocreate);
	if (!fin) {
		AfxMessageBox("open file failed");
		return;
	}

	int i = 1;
	while (i<122) {
		fin >> fullChess[i].pos1 >> fullChess[i].pos2 >> fullChess[i].pos3;
		i++;
	}
	fin.close();

	return;
}

//初始化factionChess数组;
void ChessGame::SetFactionChess()
{
	//各玩家的十个棋子
	int temp[6][10] = {
		1,2,3,4,5,6,7,8,9,10,
		11,12,13,14,24,25,26,36,37,47,
		66,76,77,87,88,89,99,100,101,102,
		112,113,114,115,116,117,118,119,120,121,
		75,85,86,96,97,98,108,109,110,111,
		20,21,22,23,33,34,35,45,46,56};

	for (int faction = 0; faction < 6; faction++) {
		for (int j = 0; j < 10; j++) {
			factionChess[faction][j].number = temp[faction][j];
		}
	}

	return;
}

//找到棋子direct方向最远可到位置，加入availablePlace[]
//规则为等距跳
void ChessGame::FindAvailablePlace(Chess * pChess, int direct)
{
	int piece = 0, space = 0;
	Chess *point = pChess;

	//遇到空格space++, 遇到棋子piece = 1
	//当piece = 1时进入else，当空格时space--，直到最后可以跳到的位置
	while (point->around[direct]) {
		point = point->around[direct];
		if (0 == piece) {
			if (0 == point->color) {
				space++;
			}
			else {
				piece = 1;
			}
		}
		else {
			if (0 == space) {
				if (0 == point->color) {
					//加入并以point为目标继续寻找
					this->FindMoreAP(point);
					return;
				}
				else {
					return;
				}
			}
			else {
				if (0 == point->color) {
					space--;
				}
				else {
					return;
				}
			}
		}
	}

	return;
}

//更换起点，递归查找更多可到达点
void ChessGame::FindMoreAP(Chess * pChess)
{
	Chess* point = pChess;
	int nLen = factionChess[faction][fChessNumber].availablePlace.size();

	for (int i = 0; i < nLen; i++) {
		//避免重复添加
		if (point->number == factionChess[faction][fChessNumber].availablePlace[i]) {
			return;
		}
	}
	factionChess[faction][fChessNumber].availablePlace.push_back(point->number);

	//更换起始点继续寻找
	for (int i = 0; i < 6; i++) {
		this->FindAvailablePlace(point, i);
	}

	return;
}

//更新棋盘所有棋子可下位置	
void ChessGame::ChessFull()
{
	for (fChessNumber = 0; fChessNumber < 10; fChessNumber++) {
		factionChess[faction][fChessNumber].availablePlace.clear();
		for (int i = 0; i < 6; i++) {
			this->FindAvailablePlace(&fullChess[factionChess[faction][fChessNumber].number], i);
		}
		this->AddBesidePlace(&fullChess[factionChess[faction][fChessNumber].number]);
	}

	return;
}

//添加临近棋子
void ChessGame::AddBesidePlace(Chess * pChess)
{
	for (int i = 0; i < 6; i++) {
		if (NULL != pChess->around[i]) {
			if (0 == pChess->around[i]->color) {
				factionChess[faction][fChessNumber].availablePlace.push_back(pChess->around[i]->number);
			}
		}
	}

	return;
}

//下一步走棋玩家类别跳转
void ChessGame::NextStep()
{
	if (0 != playerType[currentPlayer]) {
		switch (playerType[currentPlayer])
		{
		case 2:
			this->PCStep();
			break;
			
		default:
			break;
		}
	}

	return;
}

void ChessGame::SetBorder(int right, int bottom)
{
	this->right = right;
	this->bottom = bottom;
}

//开始新游戏、初始化
void ChessGame::NewGame()
{
	pChess = NULL;
	beginGame = FALSE;

	//初始化玩家棋盘
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {
			factionChess[i][j].number = 0;
			factionChess[i][j].upOrDown = DOWN;
		}
		playerType[i] = 0;
	}	

	//将所有的棋盘上的位置置空
	memset(fullChess, 0, sizeof(fullChess));

	srand((unsigned)time(NULL));

	SetNumber();
	SetXY();
	SetAround();
	SetnPRI();
	SetFactionChess();
	SetPlace();
	SetPos();
	SetColor();

	for (faction = 0; faction<6; faction++)
		ChessFull();
	fChessNumber = 0;
	count = 0;

	return;
}

//返回所选棋子编号
int ChessGame::Point2Number(CPoint point)
{
	CRect rgn;
	for (int i = 0; i < 122; i++) {
		rgn = CRect(
			fullChess[i].x - 19,
			fullChess[i].y - 19,
			fullChess[i].x + 19,
			fullChess[i].y + 19);

		if(rgn.PtInRect(point)){
			return i;
		}
	}

	return 0;
}

//下子后更新视图
void ChessGame::Renovate(Chess* pChess1, Chess* pChess2)
{
	m_cwnd->InvalidateRect(
		CRect(
			pChess1->x - 19,
			pChess1->y - 19,
			pChess1->x + 20,
			pChess1->y + 22), 0);

	m_cwnd->InvalidateRect(
		CRect(
			pChess2->x - 19,
			pChess2->y - 19,
			pChess2->x + 19,
			pChess2->y + 19), 0);

		bool flag = true;
		int destination = (currentPlayer + 3>5) ? (currentPlayer - 3) : (currentPlayer + 3);
		for (int j = 0; j < 10; j++) {
			if (fullChess[factionChess[currentPlayer][j].number].place != destination + 1)
				flag = false;
		}
		if (flag) {
			CString notice;
			notice.Format("玩家%d", currentPlayer + 1);
			notice.Append("已下完");
			m_cwnd->MessageBox(notice, "跳棋");
			playerType[currentPlayer] = BLANK;
		}

		for (faction = 0; faction < 6; faction++) {
			this->ChessFull();
		}
		do {
			currentPlayer++;
			currentPlayer %= 6;
		} while (0 == playerType[currentPlayer]);
		this->NextStep();
}

//棋局评估、预测五步
int ChessGame::Evaluate(int i, int alphaBeataMax1, int alphaBeataMin1, int alphaBeataMax2, int alphaBeataMin2, int alphaBeataMax3)
{
	int tempScore=-18, alphaBeataMin3 = 18;
	int origin, target, beta, alpha, j;
	beta = -10;
	alpha = 10;

	int alternativeChess[5][3];
	int alternativeScore[5] = { -3,-3,-3,-3,-3 };
	int nou;

	int playerList[19] = { currentPlayer,(currentPlayer + 1) % 6,(currentPlayer + 2) % 6,(currentPlayer + 3) % 6,(currentPlayer + 4) % 6,(currentPlayer + 5) % 6,
						currentPlayer,(currentPlayer + 1) % 6,(currentPlayer + 2) % 6,(currentPlayer + 3) % 6,(currentPlayer + 4) % 6,(currentPlayer + 5) % 6,
						currentPlayer,(currentPlayer + 1) % 6,(currentPlayer + 2) % 6,(currentPlayer + 3) % 6,(currentPlayer + 4) % 6,(currentPlayer + 5) % 6,currentPlayer };

	int nextPlayer;

	i++;

	if (i<19)
	{
		while (playerType[playerList[i]] == 0 && i<19)
			i++;
	}
	//预测5步
	if (i == 18)
	{
		faction = currentPlayer;
		ChessFull();

		for (j = 0; j<10; j++)
		{
			for (i = 0; i<factionChess[currentPlayer][j].availablePlace.size(); i++)
			{
				target = factionChess[currentPlayer][j].availablePlace[i];//target值为当前棋子可以走的一个位置
				if (fullChess[target].score[currentPlayer] - fullChess[factionChess[currentPlayer][j].number].score[currentPlayer] >tempScore)
				{
					if ((fullChess[target].place == currentPlayer + 1)
						|| (fullChess[target].place == 0)
						|| (fullChess[target].place == ((currentPlayer + 3 >= 6) ? (currentPlayer - 3) : (currentPlayer + 3)) + 1))
					{
						tempScore = fullChess[factionChess[currentPlayer][j].availablePlace[i]].score[currentPlayer] - fullChess[factionChess[currentPlayer][j].number].score[currentPlayer];
						//tempScore是上面判断的两个在方向上的距离差
					}
				}
			}
		}
		if (tempScore<0)
			return 0;
		else
			return tempScore;
	}

	if (i == 6)
	{
		faction = currentPlayer;
		ChessFull();

		bool flag = true;
		nextPlayer = currentPlayer;
		int destination = (currentPlayer + 3>5) ? (currentPlayer - 3) : (currentPlayer + 3);
		for (int j = 0; j<10; j++){
			if (fullChess[factionChess[currentPlayer][j].number].place != destination + 1)
				flag = false;
		}
		if (flag){
			//获胜
			//this->playerType[currentPlayer]=0;
			return 99;
		}
		else{
			nou = PreEvaluate(currentPlayer, alternativeScore, alternativeChess);
			for (int k = 0; k<nou; k++)
			{
				if (fullChess[alternativeChess[k][2]].score[currentPlayer] - fullChess[alternativeChess[k][1]].score[currentPlayer]>-1)
				{
					origin = alternativeChess[k][1];//要走棋子的在棋盘上的标号
					target = alternativeChess[k][2];//旗子要走到的新位置的标号
					j = alternativeChess[k][0];//第几个棋子

					factionChess[currentPlayer][j].number = alternativeChess[k][2];

					//将可以走的步骤给选定的棋子，假设已走
					fullChess[alternativeChess[k][2]].color = fullChess[alternativeChess[k][1]].color;
					//原来地方的颜色为0，既没有颜色
					fullChess[alternativeChess[k][1]].color = 0;
					beta = Evaluate(i, alphaBeataMax1, alphaBeataMin1, alphaBeataMax2, alphaBeataMin2, alphaBeataMax3);
					if (beta >alphaBeataMax2)
					{
						alphaBeataMax2 = beta;
					}
					factionChess[nextPlayer][j].number = origin;
					fullChess[alternativeChess[k][1]].color = fullChess[alternativeChess[k][2]].color;
					//原来地方的颜色为0，既没有颜色
					fullChess[alternativeChess[k][2]].color = 0;
					//剪枝
					if (alphaBeataMax2 >= alphaBeataMin1)
						break;
				}
			}
			return alphaBeataMax2;
		}
	}

	if (i == 12)
	{
		nextPlayer = playerList[i];
		faction = nextPlayer;
		ChessFull();
		nou = PreEvaluate(currentPlayer, alternativeScore, alternativeChess);
		for (int k = 0; k<nou; k++)
		{
			if (fullChess[alternativeChess[k][2]].score[currentPlayer] - fullChess[alternativeChess[k][1]].score[currentPlayer]>-1)
			{
				origin = alternativeChess[k][1];//要走棋子的在棋盘上的标号
				target = alternativeChess[k][2];//旗子要走到的新位置的标号
				j = alternativeChess[k][0];//第几个棋子
				factionChess[currentPlayer][j].number = alternativeChess[k][2];
				fullChess[alternativeChess[k][2]].color = fullChess[alternativeChess[k][1]].color;
				//原来地方的颜色为0，既没有颜色
				fullChess[alternativeChess[k][1]].color = 0;
				beta = Evaluate(i, alphaBeataMax1, alphaBeataMin1, alphaBeataMax2, alphaBeataMin2, alphaBeataMax3);
				if (beta >alphaBeataMax3)
				{
					alphaBeataMax3 = beta;
				}
				factionChess[nextPlayer][j].number = origin;
				fullChess[alternativeChess[k][1]].color = fullChess[alternativeChess[k][2]].color;
				//原来地方的颜色为0，既没有颜色
				fullChess[alternativeChess[k][2]].color = 0;
				if (alphaBeataMax3 >= alphaBeataMin2)
					break;
			}
			return alphaBeataMax3;
		}
	}
	if (i>1 && i<6)
	{
		nextPlayer = playerList[i];
		faction = nextPlayer;
		ChessFull();

		//静态的评估出最好的5步
		nou = PreEvaluate(nextPlayer, alternativeScore, alternativeChess);
		for (int k = 0; k<nou; k++)
		{
			origin = alternativeChess[k][1];//要走棋子的在棋盘上的标号
			target = alternativeChess[k][2];//旗子要走到的新位置的标号
			j = alternativeChess[k][0];//第几个棋子
			factionChess[nextPlayer][j].number = alternativeChess[k][2];
			//将可以走的步骤给选定的棋子，假设已走
			fullChess[alternativeChess[k][2]].color = fullChess[alternativeChess[k][1]].color;
			//原来地方的颜色为0，既没有颜色
			fullChess[alternativeChess[k][1]].color = 0;
			alpha = Evaluate(i, alphaBeataMax1, alphaBeataMin1, alphaBeataMax2, alphaBeataMin2, alphaBeataMax3);
			if (alpha <alphaBeataMin1)
			{
				alphaBeataMin1 = alpha;
			}
			if (alpha == 99)
			{
				alphaBeataMin1 = 99;;
				break;
			}
			fullChess[alternativeChess[k][1]].color = fullChess[alternativeChess[k][2]].color;
			//原来地方的颜色为0，既没有颜色
			fullChess[alternativeChess[k][2]].color = 0;
			factionChess[nextPlayer][j].number = origin;
			if (alphaBeataMin1 <= alphaBeataMax1)
				break;
		}
		return alphaBeataMin1;
	}
	if (i>6 && i<12)
	{
		nextPlayer = playerList[i];
		faction = nextPlayer;
		ChessFull();
		nou = PreEvaluate(nextPlayer, alternativeScore, alternativeChess);
		for (int k = 0; k<nou; k++)
		{
			origin = alternativeChess[k][1];//alternativeChess[k][1];//要走棋子的在棋盘上的标号
			target = alternativeChess[k][2];// alternativeChess[k][2];//旗子要走到的新位置的标号
			j = alternativeChess[k][0]; //第几个棋子
			factionChess[nextPlayer][j].number = alternativeChess[k][2];
			fullChess[alternativeChess[k][2]].color = fullChess[alternativeChess[k][1]].color;
			//原来地方的颜色为0，既没有颜色
			fullChess[alternativeChess[k][1]].color = 0;
			alpha = Evaluate(i, alphaBeataMax1, alphaBeataMin1, alphaBeataMax2, alphaBeataMin2, alphaBeataMax3);
			if (alpha <alphaBeataMin2)
			{
				alphaBeataMin2 = alpha;
			}
			fullChess[alternativeChess[k][1]].color = fullChess[alternativeChess[k][2]].color;
			//原来地方的颜色为0，既没有颜色
			fullChess[alternativeChess[k][2]].color = 0;
			factionChess[nextPlayer][j].number = origin;
			if (alphaBeataMin2 <= alphaBeataMax2)
				break;
		}
		return alphaBeataMin2;
	}

	if (i>12 && i<18)
	{
		nextPlayer = playerList[i];
		faction = nextPlayer;
		ChessFull();
		nou = PreEvaluate(nextPlayer, alternativeScore, alternativeChess);
		for (int k = 0; k<nou; k++)
		{
			origin = alternativeChess[k][1];//要走棋子的在棋盘上的标号
			target = alternativeChess[k][2];//旗子要走到的新位置的标号
			j = alternativeChess[k][0];//第几个棋子
			factionChess[nextPlayer][j].number = alternativeChess[k][2];
			//将可以走的步骤给选定的棋子，假设已走
			fullChess[alternativeChess[k][2]].color = fullChess[alternativeChess[k][1]].color;
			//原来地方的颜色为0，既没有颜色
			fullChess[alternativeChess[k][1]].color = 0;
			alpha = Evaluate(i, alphaBeataMax1, alphaBeataMin1, alphaBeataMax2, alphaBeataMin2, alphaBeataMax3);
			if (alpha < alphaBeataMin3)
			{
				alphaBeataMin3 = alpha;
			}
			factionChess[nextPlayer][j].number = origin;
			fullChess[alternativeChess[k][1]].color = fullChess[alternativeChess[k][2]].color;
			//原来地方的颜色为0，既没有颜色
			fullChess[alternativeChess[k][2]].color = 0;
			if (alphaBeataMin3 <= alphaBeataMax3)
				break;
		}
		return alphaBeataMin3;
	}

	return 0;
}

int ChessGame::PCStep()
{
	int origin,target,j;
	int result = 0;
	int beta=-10, alpha=10, value = -11;
	int alphabetaMin1, alphabetaMax2, alphabetaMin2, alphabetaMax3;
	alphabetaMin1 = 18;
	alphabetaMax2 = -18;
	alphabetaMin2 = 18;
	alphabetaMax3 = -18;

	int alternativeChess[5][3];//预计目标，0存放的棋子是该方第几个棋子，1起点标号，2目标标号
	int alternativeScore[5] = { -3,-3,-3,-3,-3 };

	faction = currentPlayer;
	this->ChessFull();

	//静态的评估出最好的5步
	int nou = this->PreEvaluate(currentPlayer, alternativeScore, alternativeChess);

	//对静态评估出来的5步进行树的生成，动态评估
	for (int i = 0; i < nou; i++) {
		if (fullChess[alternativeChess[i][2]].score[currentPlayer] - fullChess[alternativeChess[i][1]].score[currentPlayer] > -1) {
			origin = alternativeChess[i][1]; //操作位置
			target = alternativeChess[i][2]; //目标位置
			j = alternativeChess[i][0]; //棋子标号
			//假设该步已走
			factionChess[currentPlayer][j].number = alternativeChess[i][2];
			fullChess[alternativeChess[i][2]].color = fullChess[alternativeChess[i][1]].color;
			fullChess[alternativeChess[i][1]].color = 0;
			beta = Evaluate(0, alpha, alphabetaMin1, alphabetaMax2, alphabetaMin2, alphabetaMax3);//评估函数
			if (beta == 99) {
				result = i; //队列中第i个
				fullChess[alternativeChess[i][1]].color = fullChess[alternativeChess[i][2]].color;
				fullChess[alternativeChess[i][2]].color = 0;//回溯
				break;
			}

			alpha = beta;
			beta = beta + 3 * (fullChess[alternativeChess[i][2]].score[currentPlayer] - fullChess[alternativeChess[i][1]].score[currentPlayer]);
			if (beta > value) {
				value = beta;
				result = i;
			}
			else if(beta == value){
				char crand = (char)rand();
				if (crand > 0) {
					result = i;
				}
			}
			
			fullChess[alternativeChess[i][1]].color = fullChess[alternativeChess[i][2]].color;
			fullChess[alternativeChess[i][2]].color = 0;//回溯
			factionChess[currentPlayer][j].number = alternativeChess[i][1];
		}
	}


	//新棋格修改-走棋
	fullChess[alternativeChess[result][2]].color = fullChess[alternativeChess[result][1]].color;
	fullChess[alternativeChess[result][1]].color = 0;
	//将新确定地方的棋格标号送给某队列中的一个棋子
	factionChess[currentPlayer][alternativeChess[result][0]].number = alternativeChess[result][2];

	//电脑落子机械臂移动
	origin = fullChess[alternativeChess[result][1]].number;
	target = fullChess[alternativeChess[result][2]].number;
	CString orderTemp;
	orderTemp.Format("gspd800;adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
	gOrder.push_back(orderTemp);
	//orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[origin].pos1, fullChess[origin].pos2, fullChess[origin].pos3);
	orderTemp = "gstp500;";
	gOrder.push_back(orderTemp);
	gOrder.push_back("x1c");
	//orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[origin].pos1 - 500, fullChess[origin].pos2 - 500, fullChess[origin].pos3 - 500);
	orderTemp = "gstp-500;";
	gOrder.push_back(orderTemp);
	orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
	gOrder.push_back(orderTemp);
	//orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", fullChess[target].pos1, fullChess[target].pos2, fullChess[target].pos3);
	orderTemp = "gstp500;";
	gOrder.push_back(orderTemp);
	gOrder.push_back("x3c");
	//orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", fullChess[target].pos1 - 500, fullChess[target].pos2 - 500, fullChess[target].pos3 - 500);
	orderTemp = "gstp-500;";
	gOrder.push_back(orderTemp);
	gOrder.push_back(gZero);

	//重绘棋子
	m_cwnd->InvalidateRect(
		CRect(
			fullChess[alternativeChess[result][2]].x - 19,
			fullChess[alternativeChess[result][2]].y - 19,
			fullChess[alternativeChess[result][2]].x + 19,
			fullChess[alternativeChess[result][2]].y + 19), 0);
	m_cwnd->InvalidateRect(
		CRect(
			fullChess[alternativeChess[result][1]].x - 19,
			fullChess[alternativeChess[result][1]].y - 19,
			fullChess[alternativeChess[result][1]].x + 19,
			fullChess[alternativeChess[result][1]].y + 19), 0);

	//判断游戏结束
	bool flag = true;
	int destination = (currentPlayer + 3>5) ? (currentPlayer - 3) : (currentPlayer + 3);
	for (int j = 0; j < 10; j++) {
		if (fullChess[factionChess[currentPlayer][j].number].place != destination + 1)
			flag = false;
	}
	if (flag) {
		CString notice;
		notice.Format("玩家%d", currentPlayer + 1);
		notice.Append("已下完");
		m_cwnd->MessageBox(notice, "跳棋");
		playerType[currentPlayer] = BLANK;
	}

	int restPlayer = 0;
	for (int i = 0; i < 6; i++) {
		restPlayer += playerType[i];
	}
	if (0 == restPlayer) {
		//无剩余玩家，游戏结束
		return 0;
	}

	//找下一个玩家
	do {
		currentPlayer++;
		currentPlayer %= 6;
	} while (0 == playerType[currentPlayer]);

	//下一个玩家刷新可走
	faction = currentPlayer;
	this->ChessFull();
	this->NextStep();

	return 0;
}

//静态评估，得出静态最佳5步
int ChessGame::PreEvaluate(int player,int alternativeSocore[5], int alternativeChess[5][3])
{
	int tempTarget, minChess, score=-10;
	int nou = 0;

	//计算本方所有棋子可以走的所有步骤的个数
	for (int j = 0; j < 10; j++) {
		count += factionChess[player][j].availablePlace.size();
	}

	//根据可以走的步数来设置窗口的大小
	if (count < 50) {
		nou = 3;
	}
	else if (count <= 300) {
		nou = 2;
	}
	else {
		nou = 4;
	}
	

	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < factionChess[player][j].availablePlace.size(); i++) {
			tempTarget = factionChess[player][j].availablePlace[i];
			minChess = this->getMin(alternativeSocore, nou);//挑出五个中的最小的哪个棋子
			score = fullChess[tempTarget].score[player] - fullChess[factionChess[player][j].number].score[player];
			if (score > alternativeSocore[minChess]) {
				if ((fullChess[tempTarget].place == player + 1)
					||(fullChess[tempTarget].place == 0)
						||(fullChess[tempTarget].place == ((player + 3 >= 6) ? (player - 3) : (player + 3)) + 1)) {
					alternativeSocore[minChess] = score;
					alternativeChess[minChess][0] = j;//存放是第几个棋子
					alternativeChess[minChess][1] = factionChess[player][j].number;//存放要走的棋子在棋盘上的标号
					alternativeChess[minChess][2] = factionChess[player][j].availablePlace[i];//存放走到的新位置的标号
				}
			}

		}
	}

	return nou;
}

//挑出五个中的最小的哪个棋子
int ChessGame::getMin(int * alternativeSocore,int length)//)
{
	int tmp, result = 0;
	tmp = alternativeSocore[0];
	for (int i = 0; i < length; i++) {
		if (alternativeSocore[i] < tmp) {
			tmp = alternativeSocore[i];
			result = i;
		}
	}

	return result;
}

