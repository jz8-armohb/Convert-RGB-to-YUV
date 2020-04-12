#include <iostream>
#include "declarations.h"

int yuv298[256], yuv411[256];
int yuv101[256], yuv211[256];
int yuv519[256];

void yuvLookupTable()
{
	for (int i = 0; i < 256; i++)
	{
		yuv298[i] = 298 * i;
		yuv411[i] = 411 * i;
		yuv101[i] = 101 * i;
		yuv211[i] = 211 * i;
		yuv519[i] = 519 * i;
	}
}

void yuv2rgb(FILE* rgbFile, int yuvSize, int w, int h, unsigned char* yBuf, unsigned char* uBuf, unsigned char* vBuf, unsigned char* rgbBuf)
{
	unsigned char* uBuf444 = new unsigned char[yuvSize * 2 / 3];	// ��ԭ��4:4:4��U����������
	unsigned char* vBuf444 = new unsigned char[yuvSize * 2 / 3];	// ��ԭ��4:4:4��V����������
	int pxNum = w * h;	// ͼ���е���������

	// 4:2:0 to 4:4:4
	for (int i = 0; i < h / 2; i++)	// i������
	{
		for (int j = 0; j < w / 2; j++)	// j������
		{
			uBuf444[2 * i * w + 2 * j] = uBuf[i * w / 2 + j];
			uBuf444[2 * i * w + 2 * j + 1] = uBuf[i * w / 2 + j];
			uBuf444[2 * i * w + 2 * j + w] = uBuf[i * w / 2 + j];
			uBuf444[2 * i * w + 2 * j + w + 1] = uBuf[i * w / 2 + j];

			vBuf444[2 * i * w + 2 * j] = vBuf[i * w / 2 + j];
			vBuf444[2 * i * w + 2 * j + 1] = vBuf[i * w / 2 + j];
			vBuf444[2 * i * w + 2 * j + w] = vBuf[i * w / 2 + j];
			vBuf444[2 * i * w + 2 * j + w + 1] = vBuf[i * w / 2 + j];
		}
	}

	// YUV (4:4:4) to RGB
	for (int i = 0; i < pxNum; i++)
	{
		// �м������ʹ��int�ͣ��Ա�֤�㹻�ľ��ȣ���ֹ���
		int y = yBuf[i];		// YUVͼ���i�����ص�Y����
		int u = uBuf444[i];	// YUVͼ���i�����ص�U������4:4:4��
		int v = vBuf444[i];	// YUVͼ���i�����ص�V������4:4:4��
		int r;
		int g;
		int b;

		yuvLookupTable();
		//r = (298 * y + 411 * v - 57344) >> 8;	// ��ԭ��RGBͼ���R����
		r = (yuv298[y] + yuv411[v] - 57344) >> 8;	// ��ԭ��RGBͼ���R����
		if (r < 0)	
			r = 0;	// ����
		if (r > 255)
			r = 255;
		//g = (298 * y - 101 * u - 211 * v + 34739) >> 8;	// ��ԭ��RGBͼ���G����
		g = (yuv298[y] - yuv101[u] - yuv211[v] + 34739) >> 8;	// ��ԭ��RGBͼ���G����
		if (g < 0)
			g = 0;
		if (g > 255)
			g = 255;
		//b = (298 * y + 519 * u - 71117) >> 8;	// ��ԭ��RGBͼ���B����
		b = (yuv298[y] + yuv519[u] - 71117) >> 8;	// ��ԭ��RGBͼ���B����
		if (b < 0)
			b = 0;
		if (b > 255)
			b = 255;

		rgbBuf[3 * i + 2] = (unsigned char)r;	// ��ԭ��RGBͼ���R����
		rgbBuf[3 * i + 1] = (unsigned char)g;	// ��ԭ��RGBͼ���G����
		rgbBuf[3 * i] = (unsigned char)b;	// ��ԭ��RGBͼ���B����

	}
	delete[]uBuf444;
	delete[]vBuf444;

	fwrite(rgbBuf, sizeof(unsigned char), yuvSize * 2, rgbFile);
}