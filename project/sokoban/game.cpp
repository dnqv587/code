#include "game.h"

/*-------------------------------------------游戏主体-----------------------------------------------------------------*/
void currentMap()//提取当前使用地图
{
	for (int a = 0; a <= point[level].x; a++)
	{
		for (int b = 0; b <= point[level].y; b++)
		{
			map[a][b] = maps[level][a][b];
		}
	}
}

void loadResoure()//初始化IMAGE
{
	for (int i = 0; i < 9; i++)
	{
		char filename[20] = "";
		sprintf_s(filename, "./image/%d.png", imgIndex[i]);
		loadimage(img + i, filename);
	}

}

void drawMap()//加载地图画面
{
	int centreX = (screenX - 400 - point[level].y * 64) / 2;//当前地图居中横坐标
	int centreY = (screenY - point[level].x * 64) / 2; //当前地图居中纵坐标
	for (int i = 0; i < point[level].x; i++)
	{
		for (int j = 0; j < point[level].y; j++)
		{
			int x = j * 64 + centreX;
			int y = i * 64 + centreY;
			switch (map[i][j])//遍历地图
			{
			case 0:
				putimage(x, y, img + 0);
				break;
			case 1:
				putimage(x, y, img + 1);
				break;
			case 3:
				putimage(x, y, img + 3);
				break;
			case 4:
				putimage(x, y, img + 4);
				break;
			case 5:
			case 8:
				switch (userkey)//人物朝向
				{
				case 'w':
				case 'W':
				case 72:
					putimage(x, y, img + 8);
					break;
				case 's':
				case 'S':
				case 80:
				case 27:
				case 8:
				case ' ':
				case 'r':
				case 'R':
					putimage(x, y, img + 5);
					break;
				case 't':
				case 'T':
					putimage(x, y, img + 5);
					break;
				case 'a':
				case 'A':
				case 75:
					putimage(x, y, img + 2);
					break;
				case 'd':
				case 'D':
				case 77:
					putimage(x, y, img + 6);
					break;
				}
				break;
			case 7:
				putimage(x, y, img + 7);
				break;
			}

		}
	}
}

void autoDir()//自动寻路
{
	BFSdir();
	int n = 0;
	while (1)
	{
		if (dir[n] == '\0') break;
		userkey = dir[n];
		Sleep(500);
		keyDown();
		drawMap();
		n++;
	}
}

void keyDown()//按键交互
{
	int row, column;//人物坐标
	char BGM_Name[40];//存储音乐开关命令
	for (int i = 0; i < point[level].x; i++)//寻找人物位置
	{
		for (int j = 0; j < point[level].y; j++)
		{
			if (map[i][j] == 5 || map[i][j] == 8)
			{
				row = i;
				column = j;
			}
		}

	}

	switch (userkey)
	{
	case 'w':
	case 'W':
	case 72:
		if (map[row - 1][column] == 0 || map[row - 1][column] == 3)//若移动位置为空地或目的地
		{
			saveState(row, column, userkey);//记录当前状态至栈
			map[row][column] -= 5;
			map[row - 1][column] += 5;
			step++;//增加步数
		}
		else if ((map[row - 1][column] == 4 || map[row - 1][column] == 7) && (map[row - 2][column] == 0 || map[row - 2][column] == 3))
			//若移动位置为箱子或在目的地的箱子以及箱子的前方为空地或目的地
		{

			saveState(row, column, userkey);
			map[row - 1][column] += 1;
			map[row - 2][column] += 4;
			map[row][column] -= 5;
			step++;

		}
		else//如果人物无法前进则播放撞击声
		{
			mciSendString("close ./music/hit.mp3", 0, 0, 0);
			mciSendString("open ./music/hit.mp3 ", 0, 0, 0);
			mciSendString("play ./music/hit.mp3 ", 0, 0, 0);
		}
		break;
	case 's':
	case 'S':
	case 80:
		if (map[row + 1][column] == 0 || map[row + 1][column] == 3)
		{
			saveState(row, column, userkey);
			map[row][column] -= 5;
			map[row + 1][column] += 5;
			step++;
		}
		else if ((map[row + 1][column] == 4 || map[row + 1][column] == 7) && (map[row + 2][column] == 0 || map[row + 2][column] == 3))
		{
			saveState(row, column, userkey);
			map[row + 1][column] += 1;
			map[row + 2][column] += 4;
			map[row][column] -= 5;
			step++;

		}
		else
		{
			mciSendString("close ./music/hit.mp3", 0, 0, 0);
			mciSendString("open ./music/hit.mp3 ", 0, 0, 0);
			mciSendString("play ./music/hit.mp3 ", 0, 0, 0);
		}
		break;
	case 'a':
	case 'A':
	case 75:
		if (map[row][column - 1] == 0 || map[row][column - 1] == 3)
		{
			saveState(row, column, userkey);
			map[row][column] -= 5;
			map[row][column - 1] += 5;
			step++;
		}
		else if ((map[row][column - 1] == 4 || map[row][column - 1] == 7) && (map[row][column - 2] == 0 || map[row][column - 2] == 3))
		{
			saveState(row, column, userkey);
			map[row][column - 1] += 1;
			map[row][column - 2] += 4;
			map[row][column] -= 5;
			step++;


		}
		else
		{
			mciSendString("close ./music/hit.mp3", 0, 0, 0);
			mciSendString("open ./music/hit.mp3 ", 0, 0, 0);
			mciSendString("play ./music/hit.mp3 ", 0, 0, 0);
		}
		break;
	case 'd':
	case 'D':
	case 77:
		if (map[row][column + 1] == 0 || map[row][column + 1] == 3)
		{
			saveState(row, column, userkey);
			map[row][column] -= 5;
			map[row][column + 1] += 5;
			step++;
		}
		else if ((map[row][column + 1] == 4 || map[row][column + 1] == 7) && (map[row][column + 2] == 0 || map[row][column + 2] == 3))
		{
			saveState(row, column, userkey);
			map[row][column + 1] += 1;
			map[row][column + 2] += 4;
			map[row][column] -= 5;
			step++;


		}
		else
		{
			mciSendString("close ./music/hit.mp3", 0, 0, 0);
			mciSendString("open ./music/hit.mp3 ", 0, 0, 0);
			mciSendString("play ./music/hit.mp3 ", 0, 0, 0);
		}

		break;
	case 'r':
	case 'R':
		if (MessageBox(GetHWnd(), "重置游戏？", "提示", MB_OKCANCEL) == IDOK)
		{
			hour = 0, minute = 0, second = 1;//时间清零
			step = 0;//步数清零
			clearStack(ss);//清空栈
			currentMap();
			drawMap();//重置地图
		}
		break;
	case 27:
		if (MessageBox(GetHWnd(), "是否退出游戏？", "提示", MB_OKCANCEL) == IDOK)//弹出对话框是否”退出游戏“
		{
			timerFlag = 0;
			labelFlag = 0;
			destroyStack(ss);//销毁栈
			initgraph(800, 600);
			window();//ESC退回主界面
		}
		break;
	case 8:
		if (step > 0) step--;//步数-1
		rollBack();//回退
		break;
	case 13://打开/关闭音乐
		if (musicFlag == 1)
		{
			musicFlag = 0;
			sprintf_s(BGM_Name, "close ./music/BGM%d.mp3", BGM_N);//关闭音乐
			mciSendString(BGM_Name, 0, 0, 0);
		}
		else
		{
			musicFlag = 1;
			sprintf_s(BGM_Name, "open ./music/BGM%d.mp3", BGM_N);//打开音乐
			mciSendString(BGM_Name, 0, 0, 0);
			setVolume();
			sprintf_s(BGM_Name, "play ./music/BGM%d.mp3", BGM_N);
			mciSendString(BGM_Name, 0, 0, 0);
		}
		break;
	case 9://更换音乐
		if (musicFlag == 1)
		{
			sprintf_s(BGM_Name, "close ./music/BGM%d.mp3", BGM_N);
			mciSendString(BGM_Name, 0, 0, 0);
			if (BGM_N < 6) BGM_N++; else BGM_N = 1;
			sprintf_s(BGM_Name, "open ./music/BGM%d.mp3", BGM_N);
			mciSendString(BGM_Name, 0, 0, 0);
			setVolume();
			sprintf_s(BGM_Name, "play ./music/BGM%d.mp3", BGM_N);
			mciSendString(BGM_Name, 0, 0, 0);
		}
		break;
	case 'j'://BFS算法自动寻路
	case 'J':
	{
		if (MessageBox(GetHWnd(), "是否开启自动寻路？", "提示", MB_OKCANCEL) == IDOK)
		{
			autoDir();//自动寻路
			flushStdin();//清除输入缓冲
		}
		break;
	}
	case 't':
	case 'T':
		IMAGE background1;
		loadimage(&background1, "./image/background1.jpg", screenX, screenY);
		BeginBatchDraw();
		putimage(0, 0, &background1);
		drawMap();
		label();
		EndBatchDraw();
		break;
	}
}



int gameOver()//游戏结束判断
{
	int flag = 1;
	for (int i = 0; i < point[level].x; i++)
	{
		for (int j = 0; j < point[level].y; j++)
		{
			if (map[i][j] == 4)
			{
				flag = 0;
				break;

			}
		}
	}
	return flag;
}

void flushStdin()//清除getch输入缓冲
{
	int k;
	while (1)
	{
		if (_kbhit()) //有按键
		{
			k = _getch();
			if (0 == k || 0xE0 == k) k = k << 8 | _getch();
		}
		else break;
	}
}

void saveState(int r, int c, char key)//存储上一次地图的状态
{
	selemType back;
	memset(&back, 0, sizeof(selemType)); //初始化内存为0
	switch (key)
	{
	case'w':
	case'W':
	case 72:
		for (int i = 0; i < 3; i++)
		{
			back.pos[i].r = r - i;
			back.pos[i].c = c;
			back.pos[i].data = map[back.pos[i].r][back.pos[i].c];
		}
		break;
	case's':
	case'S':
	case 80:
		for (int i = 0; i < 3; i++)
		{
			back.pos[i].r = r + i;
			back.pos[i].c = c;
			back.pos[i].data = map[back.pos[i].r][back.pos[i].c];
		}
		break;
	case'a':
	case'A':
	case 75:
		for (int i = 0; i < 3; i++)
		{
			back.pos[i].r = r;
			back.pos[i].c = c - i;
			back.pos[i].data = map[back.pos[i].r][back.pos[i].c];
		}
		break;
	case'd':
	case'D':
	case 77:
		for (int i = 0; i < 3; i++)
		{
			back.pos[i].r = r;
			back.pos[i].c = c + i;
			back.pos[i].data = map[back.pos[i].r][back.pos[i].c];
		}
		break;
	}
	push(ss, back);//
}

void rollBack()//回退用户操作
{
	if (stackEmpty(ss))
	{
		return;
	}
	selemType s = getTop(ss);
	for (int i = 0; i < 3; i++)
	{
		map[s.pos[i].r][s.pos[i].c] = s.pos[i].data;//返回上一步地图
	}
	pop(ss, s);//出栈
}

void label()//提示标签
{
	if (dispplayFlag)
	{
		drawLabel(screenX - 400, 275, WHITE, "<重置游戏>请按<R>键", 35);
		drawLabel(screenX - 400, 325, WHITE, "<退出游戏>请按<ESC>键", 35);
		drawLabel(screenX - 400, 375, WHITE, "<撤销操作>请按<退格>键", 35);
		drawLabel(screenX - 400, 425, WHITE, "<自动寻路>请按<J>键", 35);
		drawLabel(screenX - 400, 460, WHITE, "（功能完善中...", 25);
		drawLabel(screenX - 400, 485, WHITE, "现只可推最近的一个箱子）", 25);
		drawLabel(screenX - 400, 550, YELLOW, "W键/↑键：向上移动", 35);
		drawLabel(screenX - 400, 600, YELLOW, "S键/↓键：向下移动", 35);
		drawLabel(screenX - 400, 650, YELLOW, "A键/←键：向左移动", 35);
		drawLabel(screenX - 400, 700, YELLOW, "D键/→键：向右移动", 35);
		drawLabel(screenX - 400, 750, BLACK, "<关闭/打开音乐>请按<回车>键", 28);
		drawLabel(screenX - 400, 800, BLACK, "<更换音乐>请按<TAB>键", 28);
		dispplayFlag = 0;
	}
	else
	{
		drawLabel(screenX - 400, 425, RED, "<按键提示>请按<T>键", 35);
		dispplayFlag = 1;
	}
}

DWORD WINAPI variLabel(void* labelFlag)//刷新标签子线程
{
	
	IMAGE background5;
	loadimage(&background5, "./image/background5.jpg", 400, 250);
	label();
	while (*(int*)labelFlag)
	{
		Sleep(50);
		BeginBatchDraw();
		putimage(screenX - 400, 0, &background5);
		sprintf_s(putLevel, "第%d关", level + 1);
		drawLabel(screenX - 280, 50, BLACK, putLevel, 50);
		sprintf_s(putStep, "当前步数：%d ", step);
		drawLabel(screenX - 350, 125, RED, putStep, 35);
		drawLabel(screenX - 350, 200, RED, times, 35);
		
		EndBatchDraw();
	} 
	return 0;
}


void gameMain()//游戏主函数
{
	dispplayFlag = 0;
	flushStdin();//清除输入缓冲
	IMAGE background1;
	loadimage(&background1, "./image/background1.jpg", screenX, screenY);
	currentMap();//取出当前地图
	loadResoure();//加载图片
	putimage(0, 0, &background1);
	step = 0;//步数清零
	timerFlag = 1;//计时器标识
	labelFlag = 1;//标签刷新标识
	
	CreateThread(NULL, 0, timer, &timerFlag, 0, NULL);//打开计时器子线程
	CreateThread(NULL, 0, variLabel, &labelFlag, 0, NULL);//打开标签子线程
	while (1)
	{
		drawMap();
		userkey = _getch();//读取用户输入
		keyDown();
		if (musicFlag) music();//音乐播放控制
		if (gameOver())
		{
			drawMap();
			clearStack(ss);//清空栈
			timerFlag = 0;//关闭计时器
			labelFlag = 0;//关闭标签刷新
			Sleep(300);
			level++;
			currentMap();
			settlementWindow();//打开结算窗口
		}
		system("cls");
	}
	closegraph();
}

/*-------------------------------------------------菜单界面-------------------------------------------------------*/

void windowsMax()//窗口最大化
{
	initgraph(screenX, screenY);//创建一个屏幕大小的窗口
	HWND hwnd = GetHWnd(); //获得窗口句柄
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) - WS_CAPTION);//设置窗口风格
	SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CXSCREEN), SWP_SHOWWINDOW);
	//closegraph();
}

struct attribute* storageAttribute(int x, int y, int width, int height, COLORREF color, const char* pText)//存储按钮属性
{
	struct attribute* pB = (struct attribute*)malloc(sizeof(struct attribute));
	pB->x = x;
	pB->y = y;
	pB->width = width;
	pB->height = height;
	pB->color = color;
	pB->pText = (char*)malloc(strlen(pText) + 1);
	strcpy(pB->pText, pText);
	return pB;
}

void drawLabel(int x, int y, COLORREF color, const char* text, int size)//设置并加载标签图形样式
{
	settextstyle(size, 0, "楷体");//字体样式
	setlinecolor(BLACK);//按钮边框颜色
	settextcolor(color);//字体颜色
	setbkmode(TRANSPARENT);//字体背景
	outtextxy(x, y, text);//按钮标签
}

void drawButton(struct attribute* pB)//设置并加载按钮样式
{
	setfillcolor(pB->color);//设置画布画笔填充颜色
	settextstyle(35, 0, "楷体");//字体大小和字体样式
	setlinecolor(BLACK);//设置当前画线颜色
	settextcolor(BLACK);//字体颜色
	setbkmode(TRANSPARENT);//字体背景
	fillrectangle(pB->x, pB->y, pB->x + pB->width, pB->y + pB->height);//按钮大小位置
	outtextxy(pB->x + 30, pB->y + 10, pB->pText);//按钮标签
}

int mouseInButton(struct attribute* pB, MOUSEMSG m)//判断光标是否在按钮内部
{
	if (pB->x <= m.x && m.x <= pB->x + pB->width && pB->y <= m.y && m.y <= pB->height + pB->y)
	{
		pB->color = RED;
		return 1;
	}
	pB->color = YELLOW;
	return 0;
}


int clickButton(struct attribute* pB, MOUSEMSG m)//判断是否点击按钮
{
	if (mouseInButton(pB, m) && m.uMsg == WM_LBUTTONDOWN)
	{
		mciSendString("close ./music/click.mp3", 0, 0, 0);
		mciSendString("open ./music/click.mp3 ", 0, 0, 0);
		mciSendString("play ./music/click.mp3 ", 0, 0, 0);
		return 1;
	}
	return 0;
}

void window()//开始菜单
{
	initStack(ss);//初始化栈
	IMAGE background2;
	loadimage(&background2, "./image/background2.jpg", 800, 600);
	struct attribute* play = storageAttribute(300, 250, 200, 50, YELLOW, "开始游戏");
	struct attribute* select = storageAttribute(300, 310, 200, 50, YELLOW, "选择关卡");
	struct attribute* musicButton = storageAttribute(300, 370, 200, 50, YELLOW, "音乐设置");
	struct attribute* introductionButton = storageAttribute(300, 430, 200, 50, YELLOW, "游戏介绍");
	struct attribute* exitButton = storageAttribute(300, 490, 200, 50, YELLOW, "退出游戏");
	changeLable();
	while (1)//鼠标交互
	{
		BeginBatchDraw();//开启批量绘图模式
		putimage(0, 0, &background2);
		drawButton(play);
		drawButton(select);
		drawButton(musicButton);
		drawButton(introductionButton);
		drawButton(exitButton);
		EndBatchDraw();
		BeginBatchDraw();
		if (musicFlag) music();//音乐播放控制
		MOUSEMSG m = GetMouseMsg();//获取鼠标消息
		if (clickButton(play, m))
		{
			EndBatchDraw();
			level = 0;//地图重置
			windowsMax();
			gameMain();
			break;
		}

		if (clickButton(select, m))
		{
			selectMap();
			break;
		}
		if (clickButton(musicButton, m))
		{
			setMusic();
			break;
		}
		if (clickButton(introductionButton, m))
		{
			introduction();
			break;
		}
		if (clickButton(exitButton, m))
		{
			if (MessageBox(GetHWnd(), "是否退出游戏并返回桌面？", "提示", MB_OKCANCEL) == IDOK)
			{
				exit(0);
			}
		}
		EndBatchDraw();
	}
	closegraph();

}

void introduction()//游戏介绍界面
{
	IMAGE background3;
	loadimage(&background3, "./image/background3.jpg", 800, 600);
	struct attribute* backButton = storageAttribute(325, 525, 140, 50, YELLOW, "返回");
	while (1)
	{
		BeginBatchDraw();
		putimage(0, 0, &background3);
		drawLabel(50, 300, WHITE, "按键介绍：", 25);
		drawLabel(100, 330, WHITE, "<重置游戏>请按<R>键", 25);
		drawLabel(100, 360, WHITE, "<退出游戏>请按<ESC>键", 25);
		drawLabel(100, 390, WHITE, "<撤销操作>请按<退格>键", 25);
		drawLabel(100, 420, WHITE, "<自动寻路>请按<J>键", 25);
		drawLabel(450, 330, WHITE, "W键/↑键：向上移动", 25);
		drawLabel(450, 360, WHITE, "S键/↓键：向下移动", 25);
		drawLabel(450, 390, WHITE, "A键/←键：向左移动", 25);
		drawLabel(450, 420, WHITE, "D键/→键：向右移动", 25);
		drawLabel(450, 450, WHITE, "<关闭/打开音乐>请按<回车>键", 25);
		drawLabel(450, 480, WHITE, "<更换音乐>请按<TAB>键", 25);
		drawLabel(50, 50, YELLOW, "简介：", 25);
		drawLabel(100, 80, YELLOW, "推箱子是非常经典的解谜游戏，源自日本，到现在已经", 25);
		drawLabel(100, 110, YELLOW, "接近40年的历史了，英文名叫Sokoban（日文倉庫番）。", 25);
		drawLabel(100, 140, YELLOW, "推箱子的游戏规则，则是扮演工人的玩家，以“推”的", 25);
		drawLabel(100, 170, YELLOW, "方式，推动箱子。可以在没有阻碍物（如墙壁等的阻碍", 25);
		drawLabel(100, 200, YELLOW, "物）的情况下，向左、右、上、下的地方移动。将箱子", 25);
		drawLabel(100, 230, YELLOW, "移动到指定点，达到指定数量，即可过关。", 25);
		drawButton(backButton);
		MOUSEMSG m = GetMouseMsg();//获取鼠标消息
		if (clickButton(backButton, m))
		{
			window();
			break;
		}
		EndBatchDraw();
	}

}

void selectMap()//选择地图界面
{
	IMAGE background3;
	loadimage(&background3, "./image/background3.jpg", 800, 600);
	char buttonLable[40] = "";//按钮标签
	struct attribute* buttonName[40];//按钮名称
	for (int i = 0; i < 40; i++)
	{
		sprintf_s(buttonLable, "第%d关", i + 1);
		buttonName[i] = storageAttribute((i % 5) * 160, (i / 5) * 65, 150, 50, YELLOW, buttonLable);//创建关卡按钮
	}
	struct attribute* backButton = storageAttribute(325, 525, 140, 50, YELLOW, "返回");//创建返回按钮
	while (1)//鼠标交互
	{
		BeginBatchDraw();//开启批量绘图模式
		putimage(0, 0, &background3);
		if (musicFlag) music();//音乐播放控制
		MOUSEMSG m = GetMouseMsg();//获取鼠标消息
		drawButton(backButton);
		for (int i = 0; i < 40; i++)//加载关卡按钮
		{
			drawButton(buttonName[i]);
		}
		if (clickButton(backButton, m))//点击返回按钮
		{
			EndBatchDraw();
			window();
			break;
		}
		for (int i = 0; i < 40; i++)
		{
			if (clickButton(buttonName[i], m))//点击关卡按钮
			{
				EndBatchDraw();
				level = i;//赋值选择关卡给level
				windowsMax();
				gameMain();
				break;
			}
		}
		EndBatchDraw();//关闭批量绘图并输出
	}
	closegraph();

}

DWORD WINAPI timer(void* timerFlag)//子线程timer，计时器
{
	hour = 0, minute = 0, second = 1;
	while (1)
	{
		BeginBatchDraw();
		if (*(int*)timerFlag == 0)
		{
			EndBatchDraw();
			break;
		}
		if (second == 59)
		{
			second = 0;
			minute++;
		}
		if (minute == 59)
		{
			minute = 0;
			hour++;
		}
		sprintf_s(times, "用时：%d时%d分%d秒 ", hour, minute, second++);
		EndBatchDraw();
		Sleep(1000);
	}
	return 0;
}

void settlementWindow()//结算窗口
{
	IMAGE win;
	IMAGE gameover;
	loadimage(&win, "./image/win.png", screenX, screenY);
	loadimage(&gameover, "./image/gameover.png", screenX, screenY);
	mciSendString("close ./music/win.mp3", 0, 0, 0);
	mciSendString("open ./music/win.mp3 ", 0, 0, 0);
	mciSendString("setaudio ./music/win.mp3 volume to 100", 0, 0, NULL);//调低音量
	mciSendString("play ./music/win.mp3 ", 0, 0, 0);//播放胜利的音乐
	const char* nextLalel;
	if (level < 40) nextLalel = "下一关"; else nextLalel = "重新开始";
	struct attribute* next = storageAttribute(400, screenY - 200, 200, 50, YELLOW, nextLalel);
	struct attribute* exit = storageAttribute(screenX - 600, screenY - 200, 200, 50, YELLOW, "退出游戏");
	while (1)//鼠标交互
	{
		BeginBatchDraw();//开启批量绘图模式
		if (level < 40) putimage(0, 0, &win); else putimage(0, 0, &gameover);
		if (level < 40) sprintf_s(putLevel, "恭喜你！完成第%d关。", level); else strcpy_s(putLevel, "恭喜你！完成所有关卡。");
		outtextxy(screenX / 2 - 300, screenY / 2 - 100, putLevel);//输出所完成关卡
		outtextxy(screenX / 2 - 300, screenY / 2, times);//输出所用时间
		sprintf_s(putStep, "所用步数：%d", step);
		outtextxy(screenX / 2 - 300, screenY / 2 + 100, putStep);//输出所用步数
		drawButton(next);
		drawButton(exit);
		if (musicFlag) music();//音乐播放控制
		MOUSEMSG m = GetMouseMsg();//获取鼠标消息
		if (clickButton(next, m))
		{
			if (level == 40) level = 0;//重置level
			EndBatchDraw();
			gameMain();
			break;
		}

		if (clickButton(exit, m))
		{
			destroyStack(ss);//销毁栈
			initgraph(800, 600);
			window();
			break;
		}
		EndBatchDraw();
	}
	closegraph();
}

void changeLable()//修改窗口标签
{
	HWND hnd = GetHWnd();//获取窗口句柄
	SetWindowText(hnd, "推箱子游戏  by 戴");
}


void setVolume()//设置音量
{
	char volume[50]; // 存储音量命令
	sprintf_s(volume, "setaudio ./music/BGM%d.mp3 volume to %i", BGM_N, nVolume);// 生成媒体命令，设置音量增减
	mciSendString(volume, 0, 0, NULL); // 执行媒体命令
}

void music()//顺序循环播放音乐
{
	char BGM_Name[40];//存储音乐开关命令
	char musicBuffer[10];//存储音乐播放状态
	char getStatus[40];// 存储音乐状态命令
	sprintf_s(getStatus, "status ./music/BGM%d.mp3 mode", BGM_N);
	mciSendString(getStatus, musicBuffer, 10, 0);//获取的音乐播放状态
	if (strstr(musicBuffer, "stopped"))//判断音乐是否停止
	{
		sprintf_s(BGM_Name, "close ./music/BGM%d.mp3", BGM_N);
		mciSendString(BGM_Name, 0, 0, 0);
		if (BGM_N < 6) BGM_N++; else BGM_N = 1;
		sprintf_s(BGM_Name, "open ./music/BGM%d.mp3", BGM_N);
		mciSendString(BGM_Name, 0, 0, 0);
		setVolume();
		sprintf_s(BGM_Name, "play ./music/BGM%d.mp3", BGM_N);
		mciSendString(BGM_Name, 0, 0, 0);
	}
}

void setMusic()//设置音乐
{
	IMAGE background3;
	loadimage(&background3, "./image/background3.jpg", 800, 600);
	struct attribute* closeMusic = storageAttribute(300, 150, 200, 50, YELLOW, "关闭音乐");
	struct attribute* playMusic = storageAttribute(300, 210, 200, 50, YELLOW, "打开音乐");
	struct attribute* changeMusic = storageAttribute(300, 270, 200, 50, YELLOW, "更换音乐");
	struct attribute* reduceVolume = storageAttribute(250, 410, 100, 50, YELLOW, "减");
	struct attribute* increaseVolume = storageAttribute(450, 410, 100, 50, YELLOW, "增");
	struct attribute* backButton = storageAttribute(325, 525, 140, 50, YELLOW, "返回");
	char text[40];//存储当前音量
	char BGM_Name[40];//存储音乐开关命令
	while (1)//鼠标交互
	{
		BeginBatchDraw();//开启批量绘图模式
		putimage(0, 0, &background3);
		drawButton(closeMusic);
		drawButton(playMusic);
		drawButton(changeMusic);
		drawButton(reduceVolume);
		drawButton(increaseVolume);
		drawButton(backButton);
		if (musicFlag) music();//音乐播放控制
		sprintf_s(text, "当前音量为：%d%%", nVolume / 10);//显示当前音量
		drawLabel(270, 350, WHITE, text, 35);
		MOUSEMSG m = GetMouseMsg();//获取鼠标消息
		if (clickButton(closeMusic, m))
		{
			musicFlag = 0;
			sprintf_s(BGM_Name, "close ./music/BGM%d.mp3", BGM_N);//关闭音乐
			mciSendString(BGM_Name, 0, 0, 0);
		}

		if (clickButton(playMusic, m))
		{
			musicFlag = 1;
			sprintf_s(BGM_Name, "open ./music/BGM%d.mp3", BGM_N);//打开音乐
			mciSendString(BGM_Name, 0, 0, 0);
			setVolume();
			sprintf_s(BGM_Name, "play ./music/BGM%d.mp3", BGM_N);
			mciSendString(BGM_Name, 0, 0, 0);
		}
		if (clickButton(changeMusic, m))//更换音乐
		{
			musicFlag = 1;
			sprintf_s(BGM_Name, "close ./music/BGM%d.mp3", BGM_N);
			mciSendString(BGM_Name, 0, 0, 0);
			if (BGM_N < 6) BGM_N++; else BGM_N = 1;
			sprintf_s(BGM_Name, "open ./music/BGM%d.mp3", BGM_N);
			mciSendString(BGM_Name, 0, 0, 0);
			setVolume();
			sprintf_s(BGM_Name, "play ./music/BGM%d.mp3", BGM_N);
			mciSendString(BGM_Name, 0, 0, 0);
		}
		if (clickButton(reduceVolume, m))
		{
			if (nVolume > 0) nVolume -= 50;//减少音量
			setVolume();
		}
		if (clickButton(increaseVolume, m))
		{
			if (nVolume < 1000)nVolume += 50;//增加音量
			setVolume();
		}
		if (clickButton(backButton, m))
		{
			window();
			break;
		}
		EndBatchDraw();
	}
	closegraph();
}

int main()
{
	initgraph(800, 600);
	mciSendString("open ./music/BGM1.mp3", 0, 0, 0);
	setVolume();
	mciSendString("play ./music/BGM1.mp3", 0, 0, 0);
	window();
	return 0;
}



//---------------------------------BFS---------------------------------------

int personX, personY, destinationX, destinationY, boxX, boxY;//人的初始位置，箱子的初始位置，目的地的初始位置
int boxMap[20][20][20][20];//记录每次推箱子，每个箱子的位置

void setBoxMap(int px, int py, int prex, int prey)
{
	for (int i = 0; i <= point[level].x; i++)
	{
		for (int j = 0; j <= point[level].y; j++)
		{
			boxMap[px][py][i][j] = boxMap[prex][prey][i][j];
		}
	}
}

void location()//定位人物、箱子、目的地
{
	for (int i = 0; i <= point[level].x; i++)//寻找人物位置
	{
		for (int j = 0; j <= point[level].y; j++)
		{
			if (map[i][j] == 5 || map[i][j] == 8)
			{
				personX = i;
				personY = j;
			}
		}
	}
	for (int i = 0; i <= point[level].x; i++)//定位各个箱子的位置
	{
		for (int j = 0; j <= point[level].y; j++)
		{

			if (map[i][j] == 4 || map[i][j]== 7)
			{
				boxMap[personX][personY][i][j] = 1;
			}
		}
	}

	initQueue(bb);
	QelemType temp;
	temp.child = { personX, personY };//设置初始人物的位置
	enQueue(bb, temp);//入队
	QelemType down;
	while (!queueEmpty(bb))//寻找离人物最近的箱子
	{
		temp = getHead(bb);//取队头元素
		deQueue(bb, temp);//出队
		if (map[temp.child.px][temp.child.py] == 4)//寻找到箱子
		{
			boxX = temp.child.px;
			boxY = temp.child.py;
			break;
		}
		for (int j = 0; j < 4; j++)
		{
			int nextX = temp.child.px + moves[j][0];
			int nextY = temp.child.py + moves[j][1];
			if (Bound(nextX, nextY))  continue;
			down.child = { nextX ,nextY };
			enQueue(bb, down);//入队
		}
	}
	clearQueue(bb);//清空队列
	temp.child = { boxX, boxY };
	enQueue(bb, temp);
	while (!queueEmpty(bb))//寻找离箱子最近的目的地
	{
		temp = getHead(bb);//取队头元素
		deQueue(bb, temp);//出队
		if (map[temp.child.px][temp.child.py] == 3 || map[temp.child.px][temp.child.py] == 8)//寻找到目的地
		{
			destinationX = temp.child.px;
			destinationY = temp.child.py;
			break;
		}
		for (int j = 0; j < 4; j++)
		{
			int nextX = temp.child.px + moves[j][0];
			int nextY = temp.child.py + moves[j][1];
			if (Bound(nextX, nextY))  continue;
			down.child = { nextX ,nextY };
			enQueue(bb, down);//入队
		}
	}
}

void BFSdir()//提取移动的方向
{
	memset(&boxMap, 0, sizeof(boxMap));//清零
	memset(&state, 0, sizeof(state));//清空
	destroyQueue(qq);//销毁队列
	destroyQueue(bb);
	initQueue(qq);//初始化队列
	location();//定位各个位置
	stepN = 0;
	int bfsStep = BFS() - 1;//运行BFS，并记录步数
	if (bfsStep != -2)
	{

		dir[bfsStep + 1] = '\0';
		QelemType pre = qq.base[stepN - 1];//记录最后一次移动的位置节点和父节点
		for (int i = stepN - 1; i >= 0; --i)
		{
			if (pre.parent.px == qq.base[i].child.px && pre.parent.py == qq.base[i].child.py && pre.parent.bx == qq.base[i].child.bx && pre.parent.by == qq.base[i].child.by)
			{//找到父节点
				int a[2] = { pre.child.px - pre.parent.px, pre.child.py - pre.parent.py };//
				if (pre.child.px - pre.parent.px == 0)
				{
					if (pre.child.py - pre.parent.py == 1)
					{
						dir[bfsStep] = 'd';
					}
					else
					{
						dir[bfsStep] = 'a';
					}
				}
				else
				{
					if (pre.child.px - pre.parent.px == 1)
					{
						dir[bfsStep] = 's';
					}
					else
					{
						dir[bfsStep] = 'w';
					}
				}
				bfsStep--;
				pre = qq.base[i];

			}
		}
	}
	else
	{
		memset(dir, 0, sizeof dir);
		MessageBox(GetHWnd(), "无法到达？", "提示", MB_OKCANCEL);
	}

}


//移动条件
bool Bound(int x, int y)//人物的移动条件，遇到不合理的位置返回真  
{
	if (x < 0 || y < 0 || x >= point[level].x || y >= point[level].y || map[x][y] == 1)
		return true;
	else
		return false;
}
bool boxBound(int x, int y)//箱子的移动条件，遇到不合理的位置返回真  
{
	if (x < 0 || y < 0 || x >= point[level].x || y >= point[level].y || map[x][y] == 1 || map[x][y] == 4 || map[x][y] == 7)
		return true;
	else
		return false;
}
bool pbBound(int px, int py, int x, int y, int m, int n, int bx, int by)//判断箱子可否被人物推动
{
	if ((boxMap[px][py][x][y] == 1 ) && (map[m][n] == 1 || map[m][n] == 4 || map[m][n] == 7 || (bx == m && by == n)))
		return true;
	else
		return false;
}


int BFS()//利用广度优先算法寻找最短的线路
{
	state[personX][personY][boxX][boxY] = 1; //初始位置设为1
	boxMap[personX][personY][boxX][boxX] = 0;//将目标箱子位置设为0
	QelemType temp;//临时存储当前位置信息
	memset(&temp, 0, sizeof(QelemType));//清空
	temp.child = { personX, personY ,boxX ,boxY };
	enQueue(qq, temp);//入队
	QelemType down;
	memset(&down, 0, sizeof(QelemType));//清空

	int nextX, nextY;//存储下一个点
	while (!queueEmpty(qq))
	{
		temp = getHead(qq);//取队头元素
		deQueue(qq, temp);//出队
		stepN++;
		if (temp.child.bx == destinationX && temp.child.by == destinationY)  //箱子到达目的地 
		{
			return state[temp.child.px][temp.child.py][temp.child.bx][temp.child.by] - 1;//如果箱子在终点，结束，返回步数
			break;
		}
		for (int i = 0; i < 4; i++)//四个方向遍历
		{
			nextX = temp.child.px + moves[i][0];//人物移动
			nextY = temp.child.py + moves[i][1];

			setBoxMap(nextX, nextY, temp.child.px, temp.child.py);

			if (Bound(nextX, nextY))  continue; //如果移动的位置不合法 ，探寻其它方向 
			if (pbBound(temp.child.px, temp.child.py,nextX, nextY, nextX + moves[i][0], nextY + moves[i][1], temp.child.bx, temp.child.by))
				continue;//如果前方的其他箱子不可推动，则探寻其他方向

			if (nextX == temp.child.bx && nextY == temp.child.by)//如果人移动和箱子重合 ，说明人推动了目标箱子
			{
				if (boxBound(temp.child.bx + moves[i][0], temp.child.by + moves[i][1]))
					continue;
				if (state[nextX][nextY][temp.child.bx + moves[i][0]][temp.child.by + moves[i][1]]) continue;
				state[nextX][nextY][temp.child.bx + moves[i][0]][temp.child.by + moves[i][1]] = state[temp.child.px][temp.child.py][temp.child.bx][temp.child.by] + 1;
				//箱子推动，则人和目标箱子位置改变，记录新状态 
				down.child = { nextX, nextY ,temp.child.bx + moves[i][0] ,temp.child.by + moves[i][1] };
				down.parent = { temp.child.px, temp.child.py ,temp.child.bx ,temp.child.by };
				enQueue(qq, down);   //新状态入队  
			}
			else   //人没有推箱子则走空地 
			{
				//if (state[nextX][nextY][temp.child.bx][temp.child.by]) continue; //如果移动后的状态出现过，则重新搜索新方向

				if (boxMap[temp.child.px][temp.child.py][nextX][nextY] == 1)
				{
					boxMap[nextX][nextY][nextX + moves[i][0]][nextY + moves[i][1]] = 1;//记录了其他箱子位置
					boxMap[nextX][nextY][nextX][nextY] = 0;//原来位置置0
				}

				state[nextX][nextY][temp.child.bx][temp.child.by] = state[temp.child.px][temp.child.py][temp.child.bx][temp.child.by] + 1;
				down.child = { nextX, nextY ,temp.child.bx ,temp.child.by };
				down.parent = { temp.child.px, temp.child.py ,temp.child.bx ,temp.child.by };
				enQueue(qq, down);
			}
		}
	}
	return -1;//箱子无法到达目的地返回-1
}



