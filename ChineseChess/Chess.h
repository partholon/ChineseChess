#pragma once
//结构体Chess为跳棋各棋子属性
//

#include <vector>
#define UP 1
#define DOWN 0

struct Chess {
	int place; //当前棋子位于区域
	int score[6]; //[0-5]当前棋子的各个方向的优先级，右上0
	int color; //棋子颜色
	Chess *around[6]; //[0-5]当前棋子的各个方向为第几棋子,逆时针，右上主第一方向0;
	int x; //棋子位图横坐标
	int	y; //棋子位图纵坐标
	int number; //棋子编号
	int pos1;
	int pos2; //棋子电机POS位置---相对POS-0
	int pos3;
};

struct Faction {
	int number; //棋子实例
	std::vector<int> availablePlace;//当前棋子可走位置
	int upOrDown; //棋子状态 up=1 down=0
};
