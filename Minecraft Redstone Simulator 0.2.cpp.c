#include <stdlib.h>
#include <graphics.h>//链接参数-mwindows

#define RedstoneTick 100

enum BlockID
{
	Air = 0,//空气
	Redstone_Wire = 1,//红石线
	Redstone_Torch = 2,//红石火把
	Redstone_Block = 3,//红石块
	Redstone_Lamp = 4,//红石灯
	Redstone_Repeater = 5,//红石中继器
	Redstone_Comparator = 6,//红石比较器
	Lever = 7,//拉杆
	Button = 8,//按钮
};
int numberOfBlockType = 9;

typedef struct _BlockState
{
	int state;//状态
	int direction;//方向
	int delay;//红石中继器的延时红石刻(1-4)
	int redstoneSignal;//红石信号强度(0-15)
	int redstoneTick;//元件所处红石刻(0.1s)
}BlockState;

enum ColorElem
{
	Color_BackGround = 0,//背景
	Color_Border = 1,//边界线
	Color_Inventory_BK = 2,//物品栏背景
	Color_Inventory_MainHandBorder = 3,//物品栏主手边框
	Color_InventoryCell_BK = 4,//物品栏单元格背景
	Color_InventoryNumber = 5,//物品栏单元格序号
	Color_InventoryNumber_Current = 6,//主手序号
	Color_MouseHighlight = 7,//鼠标悬浮高亮
	Color_RedstoneSignal_0 = 8,//红石信号强度0
	//Color_RedstoneSignal_1 = 9,//红石信号强度1
	//Color_RedstoneSignal_2 = 10,//红石信号强度2
	//Color_RedstoneSignal_3 = 11,//红石信号强度3
	//Color_RedstoneSignal_4 = 12,//红石信号强度4
	//Color_RedstoneSignal_5 = 13,//红石信号强度5
	//Color_RedstoneSignal_6 = 14,//红石信号强度6
	Color_RedstoneSignal_15 = 23,//红石信号强度15
	Color_Wood = 24,//木质材料
};

color_t Color[] = {
	EGERGB(15, 15, 15),//背景
	EGERGB(80, 80, 80),//边界线
	EGERGB(24, 24, 24),//物品栏背景
	RED,//物品栏主手边框
	EGERGB(15, 15, 15),//物品栏单元格背景
	WHITE,//物品栏单元格序号
	RED,//主手序号
	EGEARGB(30, 255, 0, 0),//鼠标悬浮高亮
	EGERGB(0x87, 0, 0),//红石信号强度0
	EGERGB(0x8f, 0, 0),//红石信号强度1
	EGERGB(0x97, 0, 0),//红石信号强度2
	EGERGB(0x9f, 0, 0),//红石信号强度3
	EGERGB(0xa7, 0, 0),//红石信号强度4
	EGERGB(0xaf, 0, 0),//红石信号强度5
	EGERGB(0xb7, 0, 0),//红石信号强度6
	EGERGB(0xbf, 0, 0),//红石信号强度7
	EGERGB(0xc7, 0, 0),//红石信号强度8
	EGERGB(0xcf, 0, 0),//红石信号强度9
	EGERGB(0xd7, 0, 0),//红石信号强度10
	EGERGB(0xdf, 0, 0),//红石信号强度11
	EGERGB(0xe7, 0, 0),//红石信号强度12
	EGERGB(0xef, 0, 0),//红石信号强度13
	EGERGB(0xf7, 0, 0),//红石信号强度14
	EGERGB(0xff, 0, 0),//红石信号强度15
	PERU
};

BlockID** world;
BlockState** state;
BlockState* inventoryState;
int height = 0;
int width = 0;
int blockSideLength = 0;
int heightOfInventory = 0;
int currentTick = 0;

void InitInventory()
{
	int i;
	inventoryState =(BlockState*) calloc(numberOfBlockType, sizeof(BlockState));
	for(i=0; i<numberOfBlockType; i++)
	{
		inventoryState[i].state = 0;
		inventoryState[i].direction = 0;
		inventoryState[i].delay = 0;
		inventoryState[i].redstoneSignal = 0;
		inventoryState[i].redstoneTick = 0;
		if(i == Air);
		else if(i == Redstone_Wire);
		else if(i == Redstone_Torch)
		{
			inventoryState[i].redstoneSignal = 15;
		}
		else if(i == Redstone_Block)
		{
			inventoryState[i].redstoneSignal = 15;
		}
		else if(i == Redstone_Lamp);
		else if(i == Redstone_Repeater)
		{
			inventoryState[i].delay = 1;
		}
		else if(i == Redstone_Comparator);
	}
}

void RotateInventory(int id)
{
	if(id == Air);
	else if(id == Redstone_Wire);
	else if(id == Redstone_Torch)
	{
		inventoryState[id].direction = (inventoryState[id].direction+1)%5;
	}
	else if(id == Redstone_Block);
	else if(id == Redstone_Lamp)
	{
		//inventoryState[id].state = !(inventoryState[id].state);//debug
	}
	else if(id == Redstone_Repeater)
	{
		//inventoryState[id].state = !(inventoryState[id].state);//debug
		inventoryState[id].direction = (inventoryState[id].direction+1)%4;
	}
	else if(id == Redstone_Comparator)
	{
		//inventoryState[id].state = !(inventoryState[id].state);//debug
		inventoryState[id].direction = (inventoryState[id].direction+1)%4;
	}
	else if(id == Lever)
	{
		//inventoryState[id].state = !(inventoryState[id].state);//debug
	}
}

void ResizeWorld(int h, int w)
{
	int r, c;
	if(height != 0 && width != 0)
	{
		for(r=0; r<height; r++)
		{
			free(world[r]);
			free(state[r]);
		}
		free(world);
		free(state);
	}
	if(h != 0 && w != 0)
	{
		world =(BlockID**) calloc(h, sizeof(BlockID*));
		state =(BlockState**) calloc(h, sizeof(BlockState*));
		for(r=0; r<h; r++)
		{
			world[r] =(BlockID*) calloc(w, sizeof(BlockID));
			state[r] =(BlockState*) calloc(w, sizeof(BlockState));
		}
	}
	height = h;
	width = w;
}

void ResizeWindow(int length)
{
	blockSideLength = length;
	heightOfInventory = 2*length;
	setcaption("Minecraft Redstone Simulator");
	SetProcessDPIAware();//避免Windows缩放造成模糊
	initgraph(width*blockSideLength, height*blockSideLength+heightOfInventory, INIT_RENDERMANUAL);
	setfont(blockSideLength/2, 0, "Consolas");
	setbkmode(TRANSPARENT);//默认设置为无背景字体
	ege_enable_aa(true);
}

void DrawTorch(int x0, int y0, int state)
{
	int x = x0-blockSideLength/2;
	int y = y0-blockSideLength/2;
	if(state == 0)//亮起
	{
		setfillcolor(Color[Color_RedstoneSignal_15]);
		ege_fillellipse(x+blockSideLength*3/8, y+blockSideLength*3/8, blockSideLength/4, blockSideLength/4);
		setfillcolor(YELLOW);
		ege_fillellipse(x+blockSideLength*7/16, y+blockSideLength*7/16, blockSideLength/8, blockSideLength/8);
	}
	else//熄灭
	{
		setfillcolor(Color[Color_RedstoneSignal_0]);
		ege_fillellipse(x+blockSideLength*3/8, y+blockSideLength*3/8, blockSideLength/4, blockSideLength/4);
		setfillcolor(EGERGB(49, 26, 17));
		ege_fillellipse(x+blockSideLength*7/16, y+blockSideLength*7/16, blockSideLength/8, blockSideLength/8);
	}
}

void DrawBlockID(int x, int y, int id, BlockState s)
{
	//setcolor(CYAN);
	//xyprintf(x, y, "%d", id);
	//标准方块边长16
	if(id == Air);
	else if(id == Redstone_Wire)
	{
		//中点画圆
		setfillcolor(Color[Color_RedstoneSignal_0+s.redstoneSignal]);
		//ege_fillcircle(x+blockSideLength/2, y+blockSideLength/2, blockSideLength/4);
		ege_fillellipse(x+blockSideLength*3/8, y+blockSideLength*3/8, blockSideLength/4, blockSideLength/4);
		//方向，4二进制位从高到低表示上，右，下，左是否连接
		if(s.direction & 8)
		{
			ege_fillrect(x+blockSideLength*7/16, y, blockSideLength*2/16, blockSideLength/2);
		}
		if(s.direction & 4)
		{
			ege_fillrect(x+blockSideLength/2, y+blockSideLength*7/16, blockSideLength/2, blockSideLength*2/16);
		}
		if(s.direction & 2)
		{
			ege_fillrect(x+blockSideLength*7/16, y+blockSideLength/2, blockSideLength*2/16, blockSideLength/2);
		}
		if(s.direction & 1)
		{
			ege_fillrect(x, y+blockSideLength*7/16, blockSideLength/2, blockSideLength*2/16);
		}
		//能量数字
		//setcolor(Color[Color_RedstoneSignal_0+s.redstoneSignal]);
		//xyprintf(x+blockSideLength*8/16, y, "%2d", s.redstoneSignal);
	}
	else if(id == Redstone_Torch)
	{
		//方向，0底，1234上右下左是否连接方块侧壁
		setfillcolor(Color[Color_Wood]);
		if(s.direction == 1)
		{
			ege_fillrect(x+blockSideLength*7/16, y, blockSideLength*2/16, blockSideLength/2);
		}
		else if(s.direction == 2)
		{
			ege_fillrect(x+blockSideLength/2, y+blockSideLength*7/16, blockSideLength/2, blockSideLength*2/16);
		}
		else if(s.direction == 3)
		{
			ege_fillrect(x+blockSideLength*7/16, y+blockSideLength/2, blockSideLength*2/16, blockSideLength/2);
		}
		else if(s.direction == 4)
		{
			ege_fillrect(x, y+blockSideLength*7/16, blockSideLength/2, blockSideLength*2/16);
		}
		DrawTorch(x+blockSideLength/2, y+blockSideLength/2, s.state);
	}
	else if(id == Redstone_Block)
	{
		setfillcolor(EGERGB(230, 32, 8));
		ege_fillrect(x, y, blockSideLength, blockSideLength);
		setfillcolor(EGERGB(115, 12, 0));
		ege_fillrect(x+blockSideLength/16, y+blockSideLength/16, blockSideLength*14/16, blockSideLength*14/16);
	}
	else if(id == Redstone_Lamp)
	{
		setfillcolor(EGERGB(49, 26, 17));
		ege_fillrect(x, y, blockSideLength, blockSideLength);
		if(s.state == 0) setfillcolor(EGERGB(134, 78, 41));//off
		else setfillcolor(EGERGB(243, 192, 121));//on
		ege_fillrect(x+blockSideLength/16, y+blockSideLength/16, blockSideLength*14/16, blockSideLength*14/16);
	}
	else if(id == Redstone_Repeater)
	{
		setfillcolor(EGERGB(140, 140, 140));
		ege_fillrect(x, y, blockSideLength, blockSideLength);
		setfillcolor(EGERGB(200, 200, 200));
		ege_fillrect(x+blockSideLength/16, y+blockSideLength/16, blockSideLength*14/16, blockSideLength*14/16);
		//方向，0123朝向上右下左
		if(s.state == 0) setfillcolor(Color[Color_RedstoneSignal_0]);
		else setfillcolor(Color[Color_RedstoneSignal_15]);
		if(s.direction == 0)
		{
			//上2*2，间隔2，下8*2
			ege_fillrect(x+blockSideLength*7/16, y+blockSideLength*2/16, blockSideLength*2/16, blockSideLength*2/16);
			ege_fillrect(x+blockSideLength*7/16, y+blockSideLength*6/16, blockSideLength*2/16, blockSideLength*8/16);
			DrawTorch(x+blockSideLength*8/16, y+blockSideLength*3/16, !(s.state));
			DrawTorch(x+blockSideLength*8/16, y+blockSideLength*(5+s.delay*2)/16, !(s.state));
		}
		else if(s.direction == 1)
		{
			ege_fillrect(x+blockSideLength*12/16, y+blockSideLength*7/16, blockSideLength*2/16, blockSideLength*2/16);
			ege_fillrect(x+blockSideLength*2/16, y+blockSideLength*7/16, blockSideLength*8/16, blockSideLength*2/16);
			DrawTorch(x+blockSideLength*13/16, y+blockSideLength*8/16, !(s.state));
			DrawTorch(x+blockSideLength*(11-s.delay*2)/16, y+blockSideLength*8/16, !(s.state));
		}
		else if(s.direction == 2)
		{
			ege_fillrect(x+blockSideLength*7/16, y+blockSideLength*12/16, blockSideLength*2/16, blockSideLength*2/16);
			ege_fillrect(x+blockSideLength*7/16, y+blockSideLength*2/16, blockSideLength*2/16, blockSideLength*8/16);
			DrawTorch(x+blockSideLength*8/16, y+blockSideLength*13/16, !(s.state));
			DrawTorch(x+blockSideLength*8/16, y+blockSideLength*(11-s.delay*2)/16, !(s.state));
		}
		else if(s.direction == 3)
		{
			ege_fillrect(x+blockSideLength*2/16, y+blockSideLength*7/16, blockSideLength*2/16, blockSideLength*2/16);
			ege_fillrect(x+blockSideLength*6/16, y+blockSideLength*7/16, blockSideLength*8/16, blockSideLength*2/16);
			DrawTorch(x+blockSideLength*3/16, y+blockSideLength*8/16, !(s.state));
			DrawTorch(x+blockSideLength*(5+s.delay*2)/16, y+blockSideLength*8/16, !(s.state));
		}
		//延时数字
		/*if(s.state == 0) setcolor(Color[Color_RedstoneSignal_0]);
		else setcolor(Color[Color_RedstoneSignal_15]);
		xyprintf(x+blockSideLength*11/16, y, "%d", s.delay);*/
	}
	else if(id == Redstone_Comparator)
	{
		setfillcolor(EGERGB(140, 140, 140));
		ege_fillrect(x, y, blockSideLength, blockSideLength);
		setfillcolor(EGERGB(200, 200, 200));
		ege_fillrect(x+blockSideLength/16, y+blockSideLength/16, blockSideLength*14/16, blockSideLength*14/16);
		//方向，0123朝向上右下左
		if(s.state == 0) setfillcolor(Color[Color_RedstoneSignal_0]);
		else setfillcolor(Color[Color_RedstoneSignal_15]);
		if(s.state == 0) setcolor(Color[Color_RedstoneSignal_0]);//debug
		else setcolor(Color[Color_RedstoneSignal_15]);
		if(s.direction == 0)
		{
			//上2*2，间隔7，下2*8
			ege_fillrect(x+blockSideLength*7/16, y+blockSideLength*2/16, blockSideLength*2/16, blockSideLength*2/16);
			ege_fillrect(x+blockSideLength*4/16, y+blockSideLength*11/16, blockSideLength*8/16, blockSideLength*2/16);
			DrawTorch(x+blockSideLength*8/16, y+blockSideLength*3/16, !(s.state));
			DrawTorch(x+blockSideLength*5/16, y+blockSideLength*12/16, !(s.state));
			DrawTorch(x+blockSideLength*11/16, y+blockSideLength*12/16, !(s.state));
		}
		else if(s.direction == 1)
		{
			ege_fillrect(x+blockSideLength*12/16, y+blockSideLength*7/16, blockSideLength*2/16, blockSideLength*2/16);
			ege_fillrect(x+blockSideLength*3/16, y+blockSideLength*4/16, blockSideLength*2/16, blockSideLength*8/16);
			DrawTorch(x+blockSideLength*13/16, y+blockSideLength*8/16, !(s.state));
			DrawTorch(x+blockSideLength*4/16, y+blockSideLength*5/16, !(s.state));
			DrawTorch(x+blockSideLength*4/16, y+blockSideLength*11/16, !(s.state));
		}
		else if(s.direction == 2)
		{
			ege_fillrect(x+blockSideLength*7/16, y+blockSideLength*12/16, blockSideLength*2/16, blockSideLength*2/16);
			ege_fillrect(x+blockSideLength*4/16, y+blockSideLength*3/16, blockSideLength*8/16, blockSideLength*2/16);
			DrawTorch(x+blockSideLength*8/16, y+blockSideLength*13/16, !(s.state));
			DrawTorch(x+blockSideLength*5/16, y+blockSideLength*4/16, !(s.state));
			DrawTorch(x+blockSideLength*11/16, y+blockSideLength*4/16, !(s.state));
		}
		else if(s.direction == 3)
		{
			ege_fillrect(x+blockSideLength*2/16, y+blockSideLength*7/16, blockSideLength*2/16, blockSideLength*2/16);
			ege_fillrect(x+blockSideLength*11/16, y+blockSideLength*4/16, blockSideLength*2/16, blockSideLength*8/16);
			DrawTorch(x+blockSideLength*3/16, y+blockSideLength*8/16, !(s.state));
			DrawTorch(x+blockSideLength*12/16, y+blockSideLength*5/16, !(s.state));
			DrawTorch(x+blockSideLength*12/16, y+blockSideLength*11/16, !(s.state));
		}
		//模式字符
		/*if(s.state == 0) setcolor(Color[Color_RedstoneSignal_0]);
		else setcolor(Color[Color_RedstoneSignal_15]);
		if(s.delay == 0) xyprintf(x+blockSideLength*6/16, y+blockSideLength*3/16, "c");
		else xyprintf(x+blockSideLength*6/16, y+blockSideLength*3/16, "m");*/
	}
	else if(id == Lever)
	{
		//8*6
		setfillcolor(GRAY);
		ege_fillrect(x+blockSideLength*5/16, y+blockSideLength*4/16, blockSideLength*6/16, blockSideLength*8/16);
		setfillcolor(Color[Color_Wood]);
		if(s.state == 0)
		{
			ege_fillrect(x+blockSideLength*7/16, y+blockSideLength*2/16, blockSideLength*2/16, blockSideLength*6/16);
		}
		else
		{
			ege_fillrect(x+blockSideLength*7/16, y+blockSideLength*8/16, blockSideLength*2/16, blockSideLength*6/16);
		}
	}
	else if(id == Button)
	{
		//4*6
		setfillcolor(Color[Color_Wood]);
		ege_fillrect(x+blockSideLength*5/16, y+blockSideLength*6/16, blockSideLength*6/16, blockSideLength*4/16);
	}
}

void DrawBlock(int r, int c)
{
	setfillcolor(Color[Color_Border]);
	ege_fillrect(c*blockSideLength, r*blockSideLength, blockSideLength, blockSideLength);
	setfillcolor(Color[Color_BackGround]);
	ege_fillrect(c*blockSideLength+blockSideLength*1/32, r*blockSideLength+blockSideLength*1/32, blockSideLength*31/32, blockSideLength*31/32);
	DrawBlockID(c*blockSideLength, r*blockSideLength, world[r][c], state[r][c]);
}

void DrawWorld(int mainhand)
{
	int r, c, i;
	//清除旧世界，减少锯齿感
	cleardevice();
	//显示所有方块
	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			DrawBlock(r, c);
		}
	}
	//显示物品栏
	setfillcolor(Color[Color_Inventory_BK]);
	ege_fillrect(0, height*blockSideLength, width*blockSideLength, heightOfInventory);
	int x = (width*blockSideLength-(numberOfBlockType*blockSideLength*3/2+(numberOfBlockType-1)*blockSideLength/8))/2;
	int y = height*blockSideLength+(heightOfInventory-blockSideLength*3/2)/2;
	for(i=0; i<numberOfBlockType; i++)
	{
		//主手边框
		if(i == mainhand)
		{
			setfillcolor(Color[Color_Inventory_MainHandBorder]);
			ege_fillrect(x-blockSideLength/16+i*(blockSideLength*3/2+blockSideLength/8), y-blockSideLength/16, blockSideLength*13/8, blockSideLength*13/8);
		}
		//单元格内容
		//边长sl*3/2
		//a = n*sl*3/2+(n-1)*sl/8
		//b = (w*sl-a)/2
		//c = b+i*sl/8
		setfillcolor(Color[Color_InventoryCell_BK]);
		ege_fillrect(x+i*(blockSideLength*3/2+blockSideLength/8), y, blockSideLength*3/2, blockSideLength*3/2);
		DrawBlockID(x+blockSideLength/4+i*(blockSideLength*3/2+blockSideLength/8), y+blockSideLength/4, i, inventoryState[i]);
		//单元格序号
		if(i == mainhand)
		{
			setcolor(Color[Color_InventoryNumber_Current]);
			xyprintf(x+i*(blockSideLength*3/2+blockSideLength/8), y, "%d", i);
		}
		else
		{
			setcolor(Color[Color_InventoryNumber]);
			xyprintf(x+i*(blockSideLength*3/2+blockSideLength/8), y, "%d", i);
		}
	}
	//鼠标悬浮高亮
	mousepos(&c, &r);
	r /= blockSideLength;
	c /= blockSideLength;
	if(r >= 0 && r < height && c >= 0 && c < width)//不在物品栏
	{
		setfillcolor(Color[Color_MouseHighlight]);
		ege_fillrect(c*blockSideLength, r*blockSideLength, blockSideLength, blockSideLength);
		DrawBlockID(c*blockSideLength, r*blockSideLength, world[r][c], state[r][c]);//重绘该方块
	}
}

void PutBlock(BlockID id, int r, int c)
{
	world[r][c] = id;
	state[r][c] = inventoryState[id];
	state[r][c].redstoneTick = currentTick;
}

void SpreadSignalOnWire(int r, int c)
{
	int signal = state[r][c].redstoneSignal-1;
	if(r > 0 && world[r-1][c] == Redstone_Wire && state[r-1][c].redstoneSignal < signal)
	{
		state[r-1][c].redstoneSignal = signal;
		SpreadSignalOnWire(r-1, c);
	}
	if(c+1 < width && world[r][c+1] == Redstone_Wire && state[r][c+1].redstoneSignal < signal)
	{
		state[r][c+1].redstoneSignal = signal;
		SpreadSignalOnWire(r, c+1);
	}
	if(r+1 < height && world[r+1][c] == Redstone_Wire && state[r+1][c].redstoneSignal < signal)
	{
		state[r+1][c].redstoneSignal = signal;
		SpreadSignalOnWire(r+1, c);
	}
	if(c > 0 && world[r][c-1] == Redstone_Wire && state[r][c-1].redstoneSignal < signal)
	{
		state[r][c-1].redstoneSignal = signal;
		SpreadSignalOnWire(r, c-1);
	}
}

void SpreadSignalToComponent(int r, int c, int rs, int cs)
{
	if(world[r][c] != Air && world[r][c] != Redstone_Wire)
	{
		if(world[r][c] == Redstone_Lamp)
		{
			state[r][c].state = 1;
		}
		else if(world[r][c] == Redstone_Repeater)
		{
			if((r == rs-1 && state[r][c].direction == 0)
				|| (c == cs+1 && state[r][c].direction == 1)
				|| (r == rs+1 && state[r][c].direction == 2)
				|| (c == cs-1 && state[r][c].direction == 3))
			{
				state[r][c].state = 1;
			}
		}
		else if(world[r][c] == Redstone_Comparator)
		{
			if((r == rs-1 && state[r][c].direction == 0)
				|| (c == cs+1 && state[r][c].direction == 1)
				|| (r == rs+1 && state[r][c].direction == 2)
				|| (c == cs-1 && state[r][c].direction == 3))
			{
				state[r][c].state = 1;
			}
		}
	}
}

/*int IsReceivedSignal(int r, int c)
{
	if(r > 0 && world[r-1][c] == Redstone_Wire && state[r-1][c].redstoneSignal > 0)
	{
		return 1;
	}
	if(c+1 < width && world[r][c+1] == Redstone_Wire && state[r][c+1].redstoneSignal > 0)
	{
		return 1;
	}
	if(r+1 < height && world[r+1][c] == Redstone_Wire && state[r+1][c].redstoneSignal > 0)
	{
		return 1;
	}
	if(c > 0 && world[r][c-1] == Redstone_Wire && state[r][c-1].redstoneSignal > 0)
	{
		return 1;
	}
	return 0;
}*/

int IsSignalSource(int r, int c)
{
	if(world[r][c] == Redstone_Torch && state[r][c].state == 0)
	{
		return 1;
	}
	if(world[r][c] == Redstone_Block)
	{
		return 1;
	}
	if(world[r][c] == Lever && state[r][c].state == 1)
	{
		return 1;
	}
	if(world[r][c] == Button && state[r][c].state == 1)
	{
		return 1;
	}
	return 0;
}

void UpdateWorld()
{
	int r, c;
	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			if(world[r][c] != Air)
			{
				if(world[r][c] == Redstone_Wire)
				{
					state[r][c].direction = 0;//重置连接状态
					if(r > 0 && world[r-1][c] != Air)
					{
						state[r][c].direction |= 8;
						if(world[r-1][c] == Redstone_Wire) state[r-1][c].direction |= 2;
					}
					if(c+1 < width && world[r][c+1] != Air)
					{
						state[r][c].direction |= 4;
						if(world[r][c+1] == Redstone_Wire) state[r][c+1].direction |= 1;
					}
					if(r+1 < height && world[r+1][c] != Air)
					{
						state[r][c].direction |= 2;
						if(world[r+1][c] == Redstone_Wire) state[r+1][c].direction |= 8;
					}
					if(c > 0 && world[r][c-1] != Air)
					{
						state[r][c].direction |= 1;
						if(world[r][c-1] == Redstone_Wire) state[r][c-1].direction |= 4;
					}
					state[r][c].redstoneSignal = 0;//重置信号
				}
				else if(world[r][c] == Redstone_Torch)
				{
					state[r][c].state = 0;
				}
				else if(world[r][c] == Redstone_Lamp)
				{
					state[r][c].state = 0;
				}
				else if(world[r][c] == Redstone_Repeater)
				{
					state[r][c].state = 0;
				}
				else if(world[r][c] == Redstone_Comparator)
				{
					state[r][c].state = 0;
				}
			}
		}
	}
	//红石信号传播
	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			if(state[r][c].redstoneSignal == 15 && world[r][c] != Redstone_Wire && state[r][c].redstoneTick == currentTick)
			{
				if(r > 0 && world[r-1][c] == Redstone_Wire)
				{
					state[r-1][c].redstoneSignal = 15;
					SpreadSignalOnWire(r-1, c);
				}
				if(c+1 < width && world[r][c+1] == Redstone_Wire)
				{
					state[r][c+1].redstoneSignal = 15;
					SpreadSignalOnWire(r, c+1);
				}
				if(r+1 < height && world[r+1][c] == Redstone_Wire)
				{
					state[r+1][c].redstoneSignal = 15;
					SpreadSignalOnWire(r+1, c);
				}
				if(c > 0 && world[r][c-1] == Redstone_Wire)
				{
					state[r][c-1].redstoneSignal = 15;
					SpreadSignalOnWire(r, c-1);
				}
			}
		}
	}
	//更新红石线的红石刻
	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			if(world[r][c] == Redstone_Wire)
			{
				state[r][c].redstoneTick = currentTick+1;
			}
		}
	}
	//点亮元件
	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			if((world[r][c] == Redstone_Wire && state[r][c].redstoneSignal > 0) || IsSignalSource(r, c))
			{
				if(r > 0) SpreadSignalToComponent(r-1, c, r, c);
				if(c+1 < width) SpreadSignalToComponent(r, c+1, r, c);
				if(r+1 < height) SpreadSignalToComponent(r+1, c, r, c);
				if(c > 0) SpreadSignalToComponent(r, c-1, r, c);
			}
		}
	}
	//元件功能
	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			if(world[r][c] == Redstone_Repeater && state[r][c].state == 1)
			{
				if(state[r][c].direction == 0)
				{
					if(r > 0)
					{
						if(world[r-1][c] == Redstone_Wire)
						{
							state[r-1][c].redstoneSignal = 15;
							SpreadSignalOnWire(r-1, c);
						}
						else
						{
							SpreadSignalToComponent(r-1, c, r, c);
						}
					}
				}
				else if(state[r][c].direction == 1)
				{
					if(c+1 < width)
					{
						if(world[r][c+1] == Redstone_Wire)
						{
							state[r][c+1].redstoneSignal = 15;
							SpreadSignalOnWire(r, c+1);
						}
						else
						{
							SpreadSignalToComponent(r, c+1, r, c);
						}
					}
				}
				else if(state[r][c].direction == 2)
				{
					if(r+1 < height)
					{
						if(world[r+1][c] == Redstone_Wire)
						{
							state[r+1][c].redstoneSignal = 15;
							SpreadSignalOnWire(r+1, c);
						}
						else
						{
							SpreadSignalToComponent(r+1, c, r, c);
						}
					}
				}
				else if(state[r][c].direction == 3)
				{
					if(c > 0)
					{
						if(world[r][c-1] == Redstone_Wire)
						{
							state[r][c-1].redstoneSignal = 15;
							SpreadSignalOnWire(r, c-1);
						}
						else
						{
							SpreadSignalToComponent(r, c-1, r, c);
						}
					}
				}
			}
			else if(world[r][c] == Redstone_Comparator && state[r][c].state == 1)
			{
				if(state[r][c].direction == 0)
				{
					if(r > 0 && r+1 < height)
					{
						if(world[r-1][c] == Redstone_Wire)
						{
							state[r-1][c].redstoneSignal = state[r+1][c].redstoneSignal;
							SpreadSignalOnWire(r-1, c);
						}
						else
						{
							SpreadSignalToComponent(r-1, c, r, c);
						}
					}
				}
				else if(state[r][c].direction == 1)
				{
					if(c > 0 && c+1 < width)
					{
						if(world[r][c+1] == Redstone_Wire)
						{
							state[r][c+1].redstoneSignal = state[r][c-1].redstoneSignal;
							SpreadSignalOnWire(r, c+1);
						}
						else
						{
							SpreadSignalToComponent(r, c+1, r, c);
						}
					}
				}
				else if(state[r][c].direction == 2)
				{
					if(r > 0 && r+1 < height)
					{
						if(world[r+1][c] == Redstone_Wire)
						{
							state[r+1][c].redstoneSignal = state[r-1][c].redstoneSignal;
							SpreadSignalOnWire(r+1, c);
						}
						else
						{
							SpreadSignalToComponent(r+1, c, r, c);
						}
					}
				}
				else if(state[r][c].direction == 3)
				{
					if(c > 0 && c+1 < width)
					{
						if(world[r][c-1] == Redstone_Wire)
						{
							state[r][c-1].redstoneSignal = state[r][c+1].redstoneSignal;
							SpreadSignalOnWire(r, c-1);
						}
						else
						{
							SpreadSignalToComponent(r, c-1, r, c);
						}
					}
				}
			}
		}
	}
	//更新全局红石刻
	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			state[r][c].redstoneTick = currentTick+1;
		}
	}
}

int main()
{
	int r, c;
	int mainhand = 0;
	mouse_msg mouseMsg;
	key_msg keyMsg;
	InitInventory();
	ResizeWorld(18, 32);
	ResizeWindow(32);
	setbkcolor(Color[Color_BackGround]);
	while(1)
	{
		//更新世界
		UpdateWorld();
		//绘制世界
		DrawWorld(mainhand);
		//检测输入
		while(mousemsg())
		{
			mouseMsg = getmouse();
			if(mouseMsg.is_wheel())
			{
				if(keystate(key_control))
				{
					if(mouseMsg.wheel > 0) ResizeWindow(blockSideLength+4);
					else if(blockSideLength > 4) ResizeWindow(blockSideLength-4);
				}
				else//滚轮切物品栏
				{
					if(mouseMsg.wheel > 0) mainhand = (mainhand+1)%numberOfBlockType;
					else
					{
						mainhand--;
						if(mainhand < 0) mainhand = numberOfBlockType-1;
					}
				}
			}
			if(mouseMsg.is_up())
			{
				r = mouseMsg.y/blockSideLength;
				c = mouseMsg.x/blockSideLength;
				if(r >= 0 && r < height && c >= 0 && c < width)
				{
					if(mouseMsg.is_right())
					{
						if(world[r][c] == Air)
						{
							PutBlock((BlockID)mainhand, r, c);
						}
					}
					else if(mouseMsg.is_left())
					{
						if(world[r][c] != Air)
						{
							PutBlock(Air, r, c);
						}
					}
				}
				else
				{
					if(mouseMsg.is_right())
					{
						RotateInventory(mainhand);
					}
				}
				//点击切物品栏(不做)
			}
		}
		while(kbmsg())
		{
			keyMsg = getkey();
			if(keyMsg.msg == key_msg_char)
			{
				if(keyMsg.key >= '0' && keyMsg.key <= '9')
				{
					mainhand = keyMsg.key-'0'+0;
				}
				else if(numberOfBlockType > 10 && keyMsg.key >= 'A' && keyMsg.key <= 'Z')
				{
					mainhand = keyMsg.key-'A'+10;
				}
				if(mainhand >= numberOfBlockType || mainhand < 0) mainhand = 0;
			}
		}
		delay_ms(RedstoneTick);
		currentTick++;
	}
	closegraph();
	return 0;
}
/*--------------------------------
更新日志：
Minecraft Redstone Simulator 0.2
——新增 红石火把、红石块可以不借助导线点亮周围4个方块
——新增 红石火把、红石块需要1红石刻才能点亮红石线
——新增 激活的红石中继器会使输出方向红石元件激活，输出信号为15
——新增 激活的红石比较器会使输出方向红石元件激活，输出信号为输入信号
——优化 现在只有正确的输入方向能点亮红石中继器和红石比较器
——修复 红石仅会在左半部分点亮
//——优化 现在红石线不会与红石中继器的侧边连接
//——修复 激活的红石中继器、红石比较器的输出红石线不能激活红石元件
--------------------------------*/
