#ifndef __TANK_DEFS_H__
#define __TANK_DEFS_H__
#define TANKMAPCELL			16				//���ظ���
#define TANKMAPWIDTH		40				//ʵ�ʿ�ȣ�40 * 16
#define TANKMAPHEIGHT		40				//ʵ�ʸ߶ȣ�40 * 16
#define GAMETOTALLEVEL		8				//��Ϸ����
#define MAXTANKSPEED		5				//̹���ٶ�
#define TANKWIDTH			32				//̹�˿��
#define TANKHEIGHT			32				//̹�˳���
#define BULLETWIDTH         8               //�ӵ����
#define BULLETHEIGHT        8               //�ӵ�����
#define SCREEN_WIDTH        640				//��Ļ���
#define SCREEN_HEIGHT       640				//��Ļ�߶�
#define TEMPSPEED           10				//�����ٶ�
//TIMER
#define WM_USERQUIT (WM_USER+100)
#define NUM_EXPLODES		8				//��ը����֡��

#define NUM_PLAYERTANKSKINS		2			//���̹�˵�Ƥ������
#define NUM_ENEMYTANKSKINS		9			//�з�̹�˵�Ƥ������

enum State{OUTRECT,INRECT,PRESSDOWN};               //��־��ʼ���水ť�����Ĺ�ϵ 
enum TankStyle{PLAYERONE,PLAYERTWO,MINITANK,LARGETANK,SUPERTANK};
enum TankDirection{UP,DOWN,LEFT,RIGHT};
////////////////////////////////////////////////////////
enum Cell{CT_EMPTY,CT_SOIL,CT_STONE,CT_STEEL,CT_WATER,CT_ICE,CT_TREE,
					CT_TANK,CT_BULLET,CT_EGG,CT_GENERAL,CT_BORDER};
/*
CT_Empty	�յ�
CT_Soil		��ǽ
CT_Stone	ʯͷ
CT_Steel	��ǽ
CT_Water	��ˮ
CT_Ice		��
CT_Tree		��
CT_Tank		̹��
CT_Bullet	�ڵ�
CT_Egg		�ʵ�
CT_General	����
*/
typedef struct _CellType
{
	Cell cell;
	int defence;
	BYTE mask;		//0:��ʾ���Ի���,1:��ʾ���ɻ���(����ͨ�ӵ�����)
	_CellType(){cell=CT_EMPTY;defence=0;mask=0;}
}CellType;

typedef struct _Position
{
	BYTE x;
	BYTE y;
	_Position(){x=0;y=0;}
}Position;

typedef struct _Result 	//��ʾǰ��1��3 cell������
{
	CellType type[3];
	Position position[3];
	BYTE num;
	BOOL bCanGo;
	_Result(){num=0;bCanGo=TRUE;}
}Result;

typedef struct _Obj   //HasObject()�����ķ�������
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
