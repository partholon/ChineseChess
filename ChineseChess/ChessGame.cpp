// ChessGame.cpp : ʵ���ļ�
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

	//��ʼ���������
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {
			factionChess[i][j].number = 0;
			factionChess[i][j].upOrDown = DOWN;
		}
		playerType[i] = 0;
	}

	//�����е������ϵ�λ���ÿ�
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

//��ʼ��fullChess���� x,y;
//��ʼ�����Ӷ�ӦBMP����
void ChessGame::SetXY()
{
	//����������߽����ӱ��
	int leftNumber[18] = { 1,2,4,7,11,24,36,47,57,66,76,87,99,112,116,119,121,122 };
	//����������߽���������
	int xPostion[17] = { (int)right*0.344, (int)right*0.330, (int)right*0.316, (int)right*0.302,
		(int)right*0.174, (int)right*0.188, (int)right*0.202, (int)right*0.216, (int)right*0.230,
		(int)right*0.216, (int)right*0.202, (int)right*0.188, (int)right*0.174,
		(int)right*0.302, (int)right*0.316, (int)right*0.330, (int)right*0.344 };
	int firstNumber=0,  lastNumber= 1;//������ʼ��š���ֹ���
	int x, y;//1������y����
	float xSpace = right*0.0285;
	float ySpace = bottom*0.0438;

	for (int i = 0; i <= 16; i++) {
		firstNumber = leftNumber[i];
		lastNumber = leftNumber[i + 1];
		y = (int)bottom*0.176 + (int)i*ySpace;
		int tmp = 0;
		for (int j = firstNumber; j < lastNumber; j++) {
			x = xPostion[i] + (int)tmp*xSpace;//���������ļ��
			fullChess[j].x = x;
			fullChess[j].y = y;
			tmp++;
		}
	}

	return;
}

//��ʼ��fullChess���� place;
//��ʼ��������������
void ChessGame::SetPlace()
{
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {
			fullChess[factionChess[i][j].number].place = i + 1;
		}
	}
	return;
}

//��ʼ��fullChess���� color;
//��ʼ��������ɫ
void ChessGame::SetColor()
{
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {
			fullChess[factionChess[i][j].number].color = i + 1;
		}
	}
	return;
}

//����������ͷ����������λ��
void ChessGame::SetFactionChess(int* Start_Pos,int faction)
{
	for (int i = 0; i < 10; i++) {
		fullChess[factionChess[faction][i].number].color = 0;
	}
	for (int i = 0; i < 10; i++) {
		factionChess[faction][i].number = 122 - Start_Pos[i];
	}
}

//�Զ������ָ̻�����ʼ״̬
void ChessGame::InitChess()
{
	int origin, target;
	int faction = currentPlayer;
	for (int i = 0; i < 10; i++) {
		if (faction+1 != fullChess[factionChess[faction][i].number].place) {
			for (int j = 1; j < 11; j++) {
				if (0 == fullChess[j].color) {
					//�ƶ�factionChess[faction][i].number]-��j
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
					//�ƶ�j-������հף����ȶ���հף�
					for (int k = 112; k < 122; k++) {
						if (0 == fullChess[k].color) {
							//�ƶ�j->k ����հ�
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
								//�ƶ�j->k ����հ�
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
						//�ƶ�factionChess[faction][i].number]-��j
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
					//�ƶ�factionChess[faction][i].number]->j
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

//��ʼ��fullChess���� score;
//�������Ե���������ľ������
void ChessGame::SetnPRI()
{
	Chess *pChessData;
	Chess *pChessTemp;
	int peak[6] = { 1,11,99,121,111,23 };//���Ƕ��㣬��ʱ��
	for (int i = 0; i < 3; i++) {
		pChessData = &fullChess[peak[i]];
		for (int j = 1; j < 14; j++) {
			pChessTemp = pChessData;
			pChessTemp->score[i] = j;
			pChessTemp->score[i + 3] = 18 - j;
			//����Ѱ��
			while (pChessTemp->around[i + 2]) {
				pChessTemp = pChessTemp->around[i + 2];
				pChessTemp->score[i] = j;
				pChessTemp->score[i + 3] = 18 - j;
			}
			pChessTemp = pChessData;
			//����Ѱ��
			while (pChessTemp->around[(i + 5) % 6]) {
				pChessTemp = pChessTemp->around[(i + 5) % 6];
				pChessTemp->score[i] = j;
				pChessTemp->score[i + 3] = 18 - j;
			}
			//�ҵ���һ��
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

//��ʼ��fullChess���� number;
//��ʼ�����ӱ��
void ChessGame::SetNumber()
{
	for (int i = 0; i < 122; i++) {
		fullChess[i].number = i;
	}
	return;
}

//��ʼ��fullChess���� around;
//��ʼ�����̣��������Ǹ�λ�ĸ�����
void ChessGame::SetAround()
{
	//�������������ϵ����ӣ������Ϸ���ʼ������ʱ�뷽����ת
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

//��ʼ��fullChess���� pos;
//��ʼ������pos�������
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

//��ʼ��factionChess����;
void ChessGame::SetFactionChess()
{
	//����ҵ�ʮ������
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

//�ҵ�����direct������Զ�ɵ�λ�ã�����availablePlace[]
//����Ϊ�Ⱦ���
void ChessGame::FindAvailablePlace(Chess * pChess, int direct)
{
	int piece = 0, space = 0;
	Chess *point = pChess;

	//�����ո�space++, ��������piece = 1
	//��piece = 1ʱ����else�����ո�ʱspace--��ֱ��������������λ��
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
					//���벢��pointΪĿ�����Ѱ��
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

//������㣬�ݹ���Ҹ���ɵ����
void ChessGame::FindMoreAP(Chess * pChess)
{
	Chess* point = pChess;
	int nLen = factionChess[faction][fChessNumber].availablePlace.size();

	for (int i = 0; i < nLen; i++) {
		//�����ظ����
		if (point->number == factionChess[faction][fChessNumber].availablePlace[i]) {
			return;
		}
	}
	factionChess[faction][fChessNumber].availablePlace.push_back(point->number);

	//������ʼ�����Ѱ��
	for (int i = 0; i < 6; i++) {
		this->FindAvailablePlace(point, i);
	}

	return;
}

//���������������ӿ���λ��	
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

//����ٽ�����
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

//��һ��������������ת
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

//��ʼ����Ϸ����ʼ��
void ChessGame::NewGame()
{
	pChess = NULL;
	beginGame = FALSE;

	//��ʼ���������
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {
			factionChess[i][j].number = 0;
			factionChess[i][j].upOrDown = DOWN;
		}
		playerType[i] = 0;
	}	

	//�����е������ϵ�λ���ÿ�
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

//������ѡ���ӱ��
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

//���Ӻ������ͼ
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
			notice.Format("���%d", currentPlayer + 1);
			notice.Append("������");
			m_cwnd->MessageBox(notice, "����");
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

//���������Ԥ���岽
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
	//Ԥ��5��
	if (i == 18)
	{
		faction = currentPlayer;
		ChessFull();

		for (j = 0; j<10; j++)
		{
			for (i = 0; i<factionChess[currentPlayer][j].availablePlace.size(); i++)
			{
				target = factionChess[currentPlayer][j].availablePlace[i];//targetֵΪ��ǰ���ӿ����ߵ�һ��λ��
				if (fullChess[target].score[currentPlayer] - fullChess[factionChess[currentPlayer][j].number].score[currentPlayer] >tempScore)
				{
					if ((fullChess[target].place == currentPlayer + 1)
						|| (fullChess[target].place == 0)
						|| (fullChess[target].place == ((currentPlayer + 3 >= 6) ? (currentPlayer - 3) : (currentPlayer + 3)) + 1))
					{
						tempScore = fullChess[factionChess[currentPlayer][j].availablePlace[i]].score[currentPlayer] - fullChess[factionChess[currentPlayer][j].number].score[currentPlayer];
						//tempScore�������жϵ������ڷ����ϵľ����
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
			//��ʤ
			//this->playerType[currentPlayer]=0;
			return 99;
		}
		else{
			nou = PreEvaluate(currentPlayer, alternativeScore, alternativeChess);
			for (int k = 0; k<nou; k++)
			{
				if (fullChess[alternativeChess[k][2]].score[currentPlayer] - fullChess[alternativeChess[k][1]].score[currentPlayer]>-1)
				{
					origin = alternativeChess[k][1];//Ҫ�����ӵ��������ϵı��
					target = alternativeChess[k][2];//����Ҫ�ߵ�����λ�õı��
					j = alternativeChess[k][0];//�ڼ�������

					factionChess[currentPlayer][j].number = alternativeChess[k][2];

					//�������ߵĲ����ѡ�������ӣ���������
					fullChess[alternativeChess[k][2]].color = fullChess[alternativeChess[k][1]].color;
					//ԭ���ط�����ɫΪ0����û����ɫ
					fullChess[alternativeChess[k][1]].color = 0;
					beta = Evaluate(i, alphaBeataMax1, alphaBeataMin1, alphaBeataMax2, alphaBeataMin2, alphaBeataMax3);
					if (beta >alphaBeataMax2)
					{
						alphaBeataMax2 = beta;
					}
					factionChess[nextPlayer][j].number = origin;
					fullChess[alternativeChess[k][1]].color = fullChess[alternativeChess[k][2]].color;
					//ԭ���ط�����ɫΪ0����û����ɫ
					fullChess[alternativeChess[k][2]].color = 0;
					//��֦
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
				origin = alternativeChess[k][1];//Ҫ�����ӵ��������ϵı��
				target = alternativeChess[k][2];//����Ҫ�ߵ�����λ�õı��
				j = alternativeChess[k][0];//�ڼ�������
				factionChess[currentPlayer][j].number = alternativeChess[k][2];
				fullChess[alternativeChess[k][2]].color = fullChess[alternativeChess[k][1]].color;
				//ԭ���ط�����ɫΪ0����û����ɫ
				fullChess[alternativeChess[k][1]].color = 0;
				beta = Evaluate(i, alphaBeataMax1, alphaBeataMin1, alphaBeataMax2, alphaBeataMin2, alphaBeataMax3);
				if (beta >alphaBeataMax3)
				{
					alphaBeataMax3 = beta;
				}
				factionChess[nextPlayer][j].number = origin;
				fullChess[alternativeChess[k][1]].color = fullChess[alternativeChess[k][2]].color;
				//ԭ���ط�����ɫΪ0����û����ɫ
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

		//��̬����������õ�5��
		nou = PreEvaluate(nextPlayer, alternativeScore, alternativeChess);
		for (int k = 0; k<nou; k++)
		{
			origin = alternativeChess[k][1];//Ҫ�����ӵ��������ϵı��
			target = alternativeChess[k][2];//����Ҫ�ߵ�����λ�õı��
			j = alternativeChess[k][0];//�ڼ�������
			factionChess[nextPlayer][j].number = alternativeChess[k][2];
			//�������ߵĲ����ѡ�������ӣ���������
			fullChess[alternativeChess[k][2]].color = fullChess[alternativeChess[k][1]].color;
			//ԭ���ط�����ɫΪ0����û����ɫ
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
			//ԭ���ط�����ɫΪ0����û����ɫ
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
			origin = alternativeChess[k][1];//alternativeChess[k][1];//Ҫ�����ӵ��������ϵı��
			target = alternativeChess[k][2];// alternativeChess[k][2];//����Ҫ�ߵ�����λ�õı��
			j = alternativeChess[k][0]; //�ڼ�������
			factionChess[nextPlayer][j].number = alternativeChess[k][2];
			fullChess[alternativeChess[k][2]].color = fullChess[alternativeChess[k][1]].color;
			//ԭ���ط�����ɫΪ0����û����ɫ
			fullChess[alternativeChess[k][1]].color = 0;
			alpha = Evaluate(i, alphaBeataMax1, alphaBeataMin1, alphaBeataMax2, alphaBeataMin2, alphaBeataMax3);
			if (alpha <alphaBeataMin2)
			{
				alphaBeataMin2 = alpha;
			}
			fullChess[alternativeChess[k][1]].color = fullChess[alternativeChess[k][2]].color;
			//ԭ���ط�����ɫΪ0����û����ɫ
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
			origin = alternativeChess[k][1];//Ҫ�����ӵ��������ϵı��
			target = alternativeChess[k][2];//����Ҫ�ߵ�����λ�õı��
			j = alternativeChess[k][0];//�ڼ�������
			factionChess[nextPlayer][j].number = alternativeChess[k][2];
			//�������ߵĲ����ѡ�������ӣ���������
			fullChess[alternativeChess[k][2]].color = fullChess[alternativeChess[k][1]].color;
			//ԭ���ط�����ɫΪ0����û����ɫ
			fullChess[alternativeChess[k][1]].color = 0;
			alpha = Evaluate(i, alphaBeataMax1, alphaBeataMin1, alphaBeataMax2, alphaBeataMin2, alphaBeataMax3);
			if (alpha < alphaBeataMin3)
			{
				alphaBeataMin3 = alpha;
			}
			factionChess[nextPlayer][j].number = origin;
			fullChess[alternativeChess[k][1]].color = fullChess[alternativeChess[k][2]].color;
			//ԭ���ط�����ɫΪ0����û����ɫ
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

	int alternativeChess[5][3];//Ԥ��Ŀ�꣬0��ŵ������Ǹ÷��ڼ������ӣ�1����ţ�2Ŀ����
	int alternativeScore[5] = { -3,-3,-3,-3,-3 };

	faction = currentPlayer;
	this->ChessFull();

	//��̬����������õ�5��
	int nou = this->PreEvaluate(currentPlayer, alternativeScore, alternativeChess);

	//�Ծ�̬����������5�������������ɣ���̬����
	for (int i = 0; i < nou; i++) {
		if (fullChess[alternativeChess[i][2]].score[currentPlayer] - fullChess[alternativeChess[i][1]].score[currentPlayer] > -1) {
			origin = alternativeChess[i][1]; //����λ��
			target = alternativeChess[i][2]; //Ŀ��λ��
			j = alternativeChess[i][0]; //���ӱ��
			//����ò�����
			factionChess[currentPlayer][j].number = alternativeChess[i][2];
			fullChess[alternativeChess[i][2]].color = fullChess[alternativeChess[i][1]].color;
			fullChess[alternativeChess[i][1]].color = 0;
			beta = Evaluate(0, alpha, alphabetaMin1, alphabetaMax2, alphabetaMin2, alphabetaMax3);//��������
			if (beta == 99) {
				result = i; //�����е�i��
				fullChess[alternativeChess[i][1]].color = fullChess[alternativeChess[i][2]].color;
				fullChess[alternativeChess[i][2]].color = 0;//����
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
			fullChess[alternativeChess[i][2]].color = 0;//����
			factionChess[currentPlayer][j].number = alternativeChess[i][1];
		}
	}


	//������޸�-����
	fullChess[alternativeChess[result][2]].color = fullChess[alternativeChess[result][1]].color;
	fullChess[alternativeChess[result][1]].color = 0;
	//����ȷ���ط���������͸�ĳ�����е�һ������
	factionChess[currentPlayer][alternativeChess[result][0]].number = alternativeChess[result][2];

	//�������ӻ�е���ƶ�
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

	//�ػ�����
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

	//�ж���Ϸ����
	bool flag = true;
	int destination = (currentPlayer + 3>5) ? (currentPlayer - 3) : (currentPlayer + 3);
	for (int j = 0; j < 10; j++) {
		if (fullChess[factionChess[currentPlayer][j].number].place != destination + 1)
			flag = false;
	}
	if (flag) {
		CString notice;
		notice.Format("���%d", currentPlayer + 1);
		notice.Append("������");
		m_cwnd->MessageBox(notice, "����");
		playerType[currentPlayer] = BLANK;
	}

	int restPlayer = 0;
	for (int i = 0; i < 6; i++) {
		restPlayer += playerType[i];
	}
	if (0 == restPlayer) {
		//��ʣ����ң���Ϸ����
		return 0;
	}

	//����һ�����
	do {
		currentPlayer++;
		currentPlayer %= 6;
	} while (0 == playerType[currentPlayer]);

	//��һ�����ˢ�¿���
	faction = currentPlayer;
	this->ChessFull();
	this->NextStep();

	return 0;
}

//��̬�������ó���̬���5��
int ChessGame::PreEvaluate(int player,int alternativeSocore[5], int alternativeChess[5][3])
{
	int tempTarget, minChess, score=-10;
	int nou = 0;

	//���㱾���������ӿ����ߵ����в���ĸ���
	for (int j = 0; j < 10; j++) {
		count += factionChess[player][j].availablePlace.size();
	}

	//���ݿ����ߵĲ��������ô��ڵĴ�С
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
			minChess = this->getMin(alternativeSocore, nou);//��������е���С���ĸ�����
			score = fullChess[tempTarget].score[player] - fullChess[factionChess[player][j].number].score[player];
			if (score > alternativeSocore[minChess]) {
				if ((fullChess[tempTarget].place == player + 1)
					||(fullChess[tempTarget].place == 0)
						||(fullChess[tempTarget].place == ((player + 3 >= 6) ? (player - 3) : (player + 3)) + 1)) {
					alternativeSocore[minChess] = score;
					alternativeChess[minChess][0] = j;//����ǵڼ�������
					alternativeChess[minChess][1] = factionChess[player][j].number;//���Ҫ�ߵ������������ϵı��
					alternativeChess[minChess][2] = factionChess[player][j].availablePlace[i];//����ߵ�����λ�õı��
				}
			}

		}
	}

	return nou;
}

//��������е���С���ĸ�����
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

