#ifndef __TANK_DEFS_H__
#define __TANK_DEFS_H__
#define TANKMAPCELL			16				//象素个数
#define TANKMAPWIDTH		40				//实际宽度＝40 * 16
#define TANKMAPHEIGHT		40				//实际高度＝40 * 16
#define GAMETOTALLEVEL		8				//游戏关数
#define MAXTANKSPEED		5				//坦克速度
#define TANKWIDTH			32				//坦克宽度
#define TANKHEIGHT			32				//坦克长度
#define BULLETWIDTH         8               //子弹宽度
#define BULLETHEIGHT        8               //子弹长度
#define SCREEN_WIDTH        640				//屏幕宽度
#define SCREEN_HEIGHT       640				//屏幕高度
#define TEMPSPEED           10				//测试速度
//TIMER
#define WM_USERQUIT (WM_USER+100)
#define NUM_EXPLODES		8				//爆炸动画帧数

#define NUM_PLAYERTANKSKINS		2			//玩家坦克的皮肤种类
#define NUM_ENEMYTANKSKINS		9			//敌方坦克的皮肤种类

enum State{OUTRECT,INRECT,PRESSDOWN};               //标志开始界面按钮与鼠标的关系 
enum TankStyle{PLAYERONE,PLAYERTWO,MINITANK,LARGETANK,SUPERTANK};
enum TankDirection{UP,DOWN,LEFT,RIGHT};
////////////////////////////////////////////////////////
enum Cell{CT_EMPTY,CT_SOIL,CT_STONE,CT_STEEL,CT_WATER,CT_ICE,CT_TREE,
					CT_TANK,CT_BULLET,CT_EGG,CT_GENERAL,CT_BORDER};
/*
CT_Empty	空地
CT_Soil		土墙
CT_Stone	石头
CT_Steel	铁墙
CT_Water	河水
CT_Ice		冰
CT_Tree		树
CT_Tank		坦克
CT_Bullet	炮弹
CT_Egg		彩蛋
CT_General	将军
*/
typedef struct _CellType
{
	Cell cell;
	int defence;
	BYTE mask;		//0:表示可以击穿,1:表示不可击穿(就普通子弹而言)
	_CellType(){cell=CT_EMPTY;defence=0;mask=0;}
}CellType;

typedef struct _Position
{
	BYTE x;
	BYTE y;
	_Position(){x=0;y=0;}
}Position;

typedef struct _Result 	//表示前方1～3 cell的内容
{
	CellType type[3];
	Position position[3];
	BYTE num;
	BOOL bCanGo;
	_Result(){num=0;bCanGo=TRUE;}
}Result;

typedef struct _Obj   //HasObject()函数的返回类型
{
	BYTE sort;       //0: Bullet ,1: player,2:enemy;
	POSITION position;
	BOOL has;
	_Obj(){sort=-1;has=FALSE;position=NULL;}
}Obj;

////////////////////////////////////////////////////////

enum GameState{GAMEBEGIN,GAMEPAUSE,GAMEACTIVE,GAMEWIN,GAMEOVER,GAMENEXT,GAMEMENU};
enum RateDirection{sequence,reversion,perversion};

/******************************************************/
#define TANKLIFE		100   //PLAYER'S TANK
#define ENEMYLIFE		150    //NPC
#define BULLETDAMAGE     50 
#endif								
