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
} player, bull[BULLET_NUM], enemy[ENEMY_NUM];//��ң��л����ӵ��ṹ��
bool timer(int ms, int id)
{
	static DWORD t[10];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}

	return false;
}//����ʱ����
bool pointinrect(int x, int y, RECT& r)
{
	return(r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);
}//�ж�����λ��
void welcom()
{
	LPCTSTR title = _T("�ɻ���ս");
	LPCTSTR tplay = _T("��ʼ��Ϸ");
	LPCTSTR texit = _T("�˳���Ϸ");
	RECT tplayr, texitr;
	BeginBatchDraw();
	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, _T("����"));
	settextcolor(BLACK);//ȷ���������������ɫ
	outtextxy(WIDTH / 2 - textwidth(title) / 2, HEIGHT / 10, title);
	tplayr.left = WIDTH / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = HEIGHT / 5;
	tplayr.bottom = tplayr.top + textheight(tplay);
	texitr.left = WIDTH / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = HEIGHT / 5 * 2;//ȷ�������λ��
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
	}//�жϽ�����Ϸ�����˳���Ϸ
}
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}//������ͼƬ����͸������
void loadimg()
{
	initgraph(WIDTH, HEIGHT);
	loadimage(&bk, "background.png");
	loadimage(&img_player[0], "me1.png", 102, 126);
	loadimage(&img_player[1], "me2.png", 102, 126);
	loadimage(&img_bullet, "bullet.jpg");
	loadimage(&img_enemyPlane[0], "enemy1.png", 57, 43);
	loadimage(&img_enemyPlane[1], "enemy2.png", 69, 99);
}//������Ϸ����ң��л����ӵ�������ͼƬ
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
}//������������жϵл������࣬����Ѫ��
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
}//���ӵ����л�����ҵ����ݽ��г�ʼ��
void gamedrawn()
{
	putimage(0, 0, &bk);
	transparentimage(NULL, player.x, player.y, &img_player[1]);
	transparentimage(NULL, player.x, player.y, &img_player[1]);//����ҽ�����Ϸ��ͼ
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bull[i].live)
		{
			transparentimage(NULL, bull[i].x, bull[i].y, &img_bullet);
		}
	}//���ӵ�������Ϸ��ͼ
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
	}//�Եл�������Ϸ��ͼ
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
}//���ӵ��ĳ�ʼλ�ú�����״̬���г�ʼ��
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
}//�涨�ӵ����˶����ɺ��˶��߽�
void playermove(int speed)
{
	if (GetAsyncKeyState(VK_SPACE) && timer(150, 1))
	{
		createbullet();
	}//�����ӵ�
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
}//�涨��ҵ��ƶ����ɺ��ƶ��߽�
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
}//�Եл��ĳ�ʼλ�ú�����״̬���г�ʼ��
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
}//�Եл����ƶ����ɺ��ƶ��߽���й涨
void gameover()
{
	EndBatchDraw();
	_getch();
	closegraph();
	printf("Game over\n��ĵ÷�Ϊ%d", score);
}//��Ϸ��������ʾ�÷�
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
		}//�ӵ���л���ײ���
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
		}//�л��������ײ���
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
	mciSendString("open D:\\c���Դ���ҵ\\Project1\\�ɻ�����.mp3 alias bkmusic", NULL, 0, NULL);
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