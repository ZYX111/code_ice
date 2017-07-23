#include<iostream>
#include<windows.h>  
using namespace std;
struct tap//每一次走动都有tap数据来存储
{
	int x;//横坐标
	int y;//纵坐标
	int s;//从初始点到该点的总步数，以后代码简称步数
};
tap data_[100];//用来存储移动的数据
int map[10][10] = {//地图数据
	{ 1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,1 },
	{ 1,0,1,1,1,0,1,1,0,1 },
	{ 1,0,0,0,1,0,1,1,0,1 },
	{ 1,0,1,1,1,0,0,1,0,1 },
	{ 1,0,0,0,0,1,0,0,0,1 },
	{ 1,0,1,0,1,1,1,0,1,1 },
	{ 1,0,1,0,0,1,0,1,0,1 },
	{ 1,0,0,1,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1 }
};
tap dir[4] = { {0,1},{1,0},{-1,0},{0,-1} };//通过方向的不同，变换横纵坐标
void flash() {//刷新页面
	system("cls");
	for (int i = 0; i < 10; i++) {
		for (int a = 0; a < 10; a++) {
			if (map[i][a] == 1) {
				cout << "■";
			}
			if (map[i][a] == 0) {
				cout << "  ";
			}
			if (map[i][a] == -1) {
				cout << "煜";
			}
		}
		cout << endl;
	}
}
void fun(int x,int y,int s) {
	if (map[x][y] == 0) {
		map[x][y] = -1;
		flash();
		Sleep(500);
		for (int d = 0; d < 4; d++) {

			fun(x + dir[d].x, y + dir[d].y, s + 1);

		}
		map[x][y] = 0;
		flash();
		Sleep(500);
	}
}
void AI() {//AI展示
	fun(1, 1, 0);
}
int main() {
	flash();
	AI();
	return 0;
}
