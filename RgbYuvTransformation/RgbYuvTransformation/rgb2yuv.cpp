#include <iostream>
#include "declarations.h"

int rgb66[256], rgb129[256], rgb25[256];
int rgb38[256], rgb74[256], rgb112[256];
int rgb94[256], rgb18[256];

void rgbLookupTable()
{
	for (int i = 0; i < 256; i++)
	{
		rgb66[i] = 66 * i;
		rgb129[i] = 129 * i;
		rgb25[i] = 25 * i;
		rgb38[i] = 38 * i;
		rgb74[i] = 74 * i;
		rgb112[i] = 112 * i;
		rgb94[i] = 94 * i;
		rgb18[i] = 18 * i;
	}
}

void rgb2yuv(FILE* yuvFile, int rgbSize, int w, int h, unsigned char* rgbBuf, unsigned char* yBuf, unsigned char* uBuf, unsigned char* vBuf)
{
	unsigned char* uBuf444 = NULL;	// 下采样前的U分量缓冲区
	unsigned char* vBuf444 = NULL;	// 下采样前的V分量缓冲区
	uBuf444 = new unsigned char[rgbSize / 3];	// 4:4:4格式
	vBuf444 = new unsigned char[rgbSize / 3];
	int pxNum = w * h;

	// RGB to YUV (4:4:4)
	for (int i = 0; i < pxNum; i++)	// i为图像像素序号
	{
		unsigned char r = rgbBuf[3 * i + 2];	// RGB图像第i个像素的R分量
		unsigned char g = rgbBuf[3 * i + 1];	// RGB图像第i个像素的G分量
		unsigned char b = rgbBuf[3 * i];		// RGB图像第i个像素的B分量
		//yBuf[i] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
		//uBuf444[i] = ((-38 * r - 74 * g + 112 * b) >> 8) + 128;
		//vBuf444[i] = ((112 * r - 94 * g - 18 * b) >> 8) + 128;
		rgbLookupTable();
		yBuf[i] = ((rgb66[r] + rgb129[g] + rgb25[b]) >> 8) + 16;
		uBuf444[i] = ((-rgb38[r] - rgb74[g] + rgb112[b]) >> 8) + 128;
		vBuf444[i] = ((rgb112[r] - rgb94[g] - rgb18[b]) >> 8) + 128;
	}

	// 4:4:4 to 4:2:0
	for (int i = 0; i < h; i += 2)
	{
		for (int j = 0; j < w; j += 2)
		{
			uBuf[i / 2 * w / 2 + j / 2] = uBuf444[i * w + j];
			vBuf[i / 2 * w / 2 + j / 2] = vBuf444[i * w + j];
		}
	}
	delete[]uBuf444;
	delete[]vBuf444;

	fwrite(yBuf, sizeof(unsigned char), rgbSize / 3, yuvFile);
	fwrite(uBuf, sizeof(unsigned char), rgbSize / 12, yuvFile);
	fwrite(vBuf, sizeof(unsigned char), rgbSize / 12, yuvFile);
}