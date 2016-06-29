#pragma once
//�ṹ��ChessΪ�������������
//

#include <vector>
#define UP 1
#define DOWN 0

struct Chess {
	int place; //��ǰ����λ������
	int score[6]; //[0-5]��ǰ���ӵĸ�����������ȼ�������0
	int color; //������ɫ
	Chess *around[6]; //[0-5]��ǰ���ӵĸ�������Ϊ�ڼ�����,��ʱ�룬��������һ����0;
	int x; //����λͼ������
	int	y; //����λͼ������
	int number; //���ӱ��
	int pos1;
	int pos2; //���ӵ��POSλ��---���POS-0
	int pos3;
};

struct Faction {
	int number; //����ʵ��
	std::vector<int> availablePlace;//��ǰ���ӿ���λ��
	int upOrDown; //����״̬ up=1 down=0
};
