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
tap dir[4] = { {0,1},{0,-1},{1,0},{-1,0} };//通过方向的不同，变换横纵坐标
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
void AI() {//AI展示
	int d = 0;//方向
	int x;//试探点x坐标
	int y;//试探点y坐标
	int top = 0;//队列头
	int last = 0;//队列尾
	data_[0] = { 1,1,0 };//设置出发点
	map[1][1] = -1;//表明已走过
	for (int i = 0;; i++) {//每循环一层该循环就走一步i
		for (int a = top; a <= last; a++) {//把当前步数i中每个点的移动可能都列举出来
			if (data_[a].s == i) {
				d = 0;
				while (d < 4) {
					x = data_[a].x + dir[d].x;//试探d方向的点
					y = data_[a].y + dir[d].y;
					if (map[x][y] == 0) {//判断该点是否能走
						map[x][y] = -1;//记录改点能走过
						last++;
						data_[last].x = x;
						data_[last].y = y;
						data_[last].s = i + 1;//添加到队列中，并为步数+1
					}
					d++;
				}
			}
		}
		for (int a = top; a < 100; a++) {//改变队列的头，使队列变成下一步的所有点的队列
			if (data_[a].s == i+1) {
				top = a;
				break;
			}
		}
		Sleep(1000);//延时以观察移动路径
		flash();//每次移动就刷新页面
	}
}
int main() {
	flash();
	AI();
	return 0;
}
