#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>
#include<windows.h>
#include<time.h>
#pragma comment(lib,"Winmm.lib")
#pragma comment( lib, "MSIMG32.LIB")
enum My
{
	WIDTH = 480,
	HEIGHT = 700,
	BULLET_NUM = 15,
	ENEMY_NUM = 12,
	BIG,
	SMALL
};
IMAGE bk;
IMAGE img_player[2];
IMAGE img_enemyPlane[2];
IMAGE img_bullet;
int score = 0;
struct plane
{
	int x;
	int y;
	bool live;
	int width;
	int height;
	int hp;
	int type;
} player, bull[BULLET_NUM], enemy[ENEMY_NUM];//玩家，敌机，子弹结构体
bool timer(int ms, int id)
{
	static DWORD t[10];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}

	return false;
}//倒计时函数
bool pointinrect(int x, int y, RECT& r)
{
	return(r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);
}//判断鼠标的位置
void welcom()
{
	LPCTSTR title = _T("飞机大战");
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR texit = _T("退出游戏");
	RECT tplayr, texitr;
	BeginBatchDraw();
	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, _T("黑体"));
	settextcolor(BLACK);//确定标题的字体与颜色
	outtextxy(WIDTH / 2 - textwidth(title) / 2, HEIGHT / 10, title);
	tplayr.left = WIDTH / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = HEIGHT / 5;
	tplayr.bottom = tplayr.top + textheight(tplay);
	texitr.left = WIDTH / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = HEIGHT / 5 * 2;//确定标题的位置
	texitr.bottom = texitr.top + textheight(texit);
	outtextxy(tplayr.left, tplayr.top, tplay);
	outtextxy(texitr.left, texitr.top, texit);
	EndBatchDraw();
	while (true)
	{
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton)
		{
			if (pointinrect(mess.x, mess.y, tplayr))
			{
				return;
			}
			else if (pointinrect(mess.x, mess.y, texitr))
			{
				exit(0);
			}
		}
	}//判断进入游戏还是退出游戏
}
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}//对载入图片进行透明处理
void loadimg()
{
	initgraph(WIDTH, HEIGHT);
	loadimage(&bk, "background.png");
	loadimage(&img_player[0], "me1.png", 102, 126);
	loadimage(&img_player[1], "me2.png", 102, 126);
	loadimage(&img_bullet, "bullet.jpg");
	loadimage(&img_enemyPlane[0], "enemy1.png", 57, 43);
	loadimage(&img_enemyPlane[1], "enemy2.png", 69, 99);
}//载入游戏中玩家，敌机，子弹，背景图片
void enemyHp(int i)
{
	int flag = rand() % 10;
	if (flag <= 2)
	{
		enemy[i].type = BIG;
		enemy[i].hp = 3;
		enemy[i].width = 69;
		enemy[i].height = 99;
	}
	else
	{
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = 57;
		enemy[i].height = 43;
	}
}//生成随机数，判断敌机的种类，决定血量
void gameinit()
{
	loadimg();
	player.x = WIDTH / 2;
	player.y = HEIGHT - 120;
	player.live = true;
	player.hp = 1;
	score = 0;
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].live = false;
	}
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].live = true;
		enemyHp(i);
	}
}//对子弹，敌机，玩家的数据进行初始化
void gamedrawn()
{
	putimage(0, 0, &bk);
	transparentimage(NULL, player.x, player.y, &img_player[1]);
	transparentimage(NULL, player.x, player.y, &img_player[1]);//对玩家进行游戏绘图
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bull[i].live)
		{
			transparentimage(NULL, bull[i].x, bull[i].y, &img_bullet);
		}
	}//对子弹进行游戏绘图
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			if (enemy[i].type = BIG)
			{
				transparentimage(NULL, enemy[i].x, enemy[i].y, &img_enemyPlane[0]);
			}
			if (enemy[i].type = SMALL)
			{
				transparentimage(NULL, enemy[i].x, enemy[i].y, &img_enemyPlane[1]);
			}
		}
	}//对敌机进行游戏绘图
}
void createbullet()
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bull[i].live)
		{
			bull[i].x = player.x + 60;
			bull[i].y = player.y;
			bull[i].live = true;
			break;
		}
	}
}//对子弹的初始位置和生存状态进行初始化
void bulletmove()
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bull[i].live)
		{
			bull[i].y -= 10;
			if (bull[i].y < 0)
			{
				bull[i].live = false;
			}
		}
	}
}//规定子弹的运动规律和运动边界
void playermove(int speed)
{
	if (GetAsyncKeyState(VK_SPACE) && timer(150, 1))
	{
		createbullet();
	}//发射子弹
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))
	{
		if (player.y > 0)
		{
			player.y -= speed;
		}
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
	{
		if (player.y + 60 < HEIGHT)
		{
			player.y += speed;
		}
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
	{
		if (player.x + 60 > 0)
		{
			player.x -= speed;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
	{
		if (player.x + 50 < WIDTH)
		{
			player.x += speed;
		}
	}
}//规定玩家的移动规律和移动边界
void createenemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			enemy[i].x = rand() % (WIDTH - 60);
			enemy[i].y = 0;
			enemyHp(i);
			break;
		}
	}
}//对敌机的初始位置和生存状态进行初始化
void enemymove(int speed)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			enemy[i].y += speed;
			if (enemy[i].y > HEIGHT)
			{
				enemy[i].live = false;
			}
		}
	}
}//对敌机的移动规律和移动边界进行规定
void gameover()
{
	EndBatchDraw();
	_getch();
	closegraph();
	printf("Game over\n你的得分为%d", score);
}//游戏结束，显示得分
void playplane()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
			continue;
		for (int k = 0; k < BULLET_NUM; k++)
		{
			if (!bull[k].live)
				continue;
			if (bull[k].x > enemy[i].x && bull[k].x < enemy[i].x + enemy[i].width + 30 && bull[k].y>enemy[i].y && bull[k].y < enemy[i].y + enemy[i].height)
			{
				bull[k].live = false;
				enemy[i].hp--;
			}
		}//子弹与敌机碰撞检测
		if (!player.live)
			continue;
		else
		{
			for (int k = 0; k < ENEMY_NUM; k++)
			{
				if (player.x > enemy[k].x + 15 && player.x < enemy[k].x + enemy[i].width - 10 && player.y>enemy[k].y && player.y < enemy[k].y + enemy[k].height - 10)
				{
					player.hp--;
					break;
				}
			}
		}//敌机与玩家碰撞检测
		if (enemy[i].hp <= 0)
		{
			enemy[i].live = false;
			score++;
		}
		if (player.hp <= 0)
		{
			player.live = false;
			Sleep(300);
			gameover();
		}
	}
}
int main()
{
	initgraph(WIDTH, HEIGHT, SHOWCONSOLE);
	gameinit();
	loadimg();
	mciSendString("open D:\\c语言大作业\\Project1\\飞机音乐.mp3 alias bkmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);
	welcom();
	
	while (1)
	{
		BeginBatchDraw();
		gamedrawn();
		FlushBatchDraw();
    if (GetAsyncKeyState('Q'))
	  {
		system("pause");
	  }
		static DWORD t1, t2;
		if (timer(500, 0))
		{
			createenemy();
		}
		if (timer(12, 2))
		{
			enemymove(3);
		}
		playermove(1);
		bulletmove();
		playplane();
		Sleep(1);
	}
	EndBatchDraw();
	return 0;
}