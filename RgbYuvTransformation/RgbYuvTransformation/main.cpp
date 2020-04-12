#include <iostream>
#include "declarations.h"
using namespace std;

int main(int argc, char* argv[])
{
	FILE* rgbOriFilePtr = NULL;	// 原RGB图像的文件指针
	FILE* yuvFilePtr = NULL;	// YUV图像的文件指针
	FILE* rgbRecFilePtr = NULL;	// 复原的RGB文件的文件指针
	const char* rgbOriFileName = argv[1];	// 原RGB图像文件名
	const char* yuvFileName = argv[2];	// YUV图像文件名
	const char* rgbRecFileName = argv[3];	// 复原RGB图像文件名
	int width = 256;	// 图像宽
	int height = 256;	// 图像高
	int rgbFileSize;	// RGB图像总字节数
	int yuvFileSize;	// YUV图像总字节数
	unsigned char* rgbOriBuffer = NULL;	// 原RGB图像缓冲区
	unsigned char* yBuffer = NULL;		// Y分量缓冲区
	unsigned char* uBuffer = NULL;		// U分量缓冲区
	unsigned char* vBuffer = NULL;		// V分量缓冲区
	unsigned char* rgbRecBuffer = NULL;	// 复原RGB图像缓冲区

	// 打开文件
	if (fopen_s(&rgbOriFilePtr, rgbOriFileName, "rb") == 0)
	{
		cout << "Successfully opened " << rgbOriFileName << "." << endl;
	}
	else
	{
		cout << "Failed to open " << rgbOriFileName << "." << endl;
		exit(0);
	}
	if (fopen_s(&yuvFilePtr, yuvFileName, "wb+") == 0)
	{
		cout << "Successfully opened " << yuvFileName << "." << endl;
	}
	else
	{
		cout << "Failed to open " << yuvFileName << "." << endl;
		exit(0);
	}
	if (fopen_s(&rgbRecFilePtr, rgbRecFileName, "wb") == 0)
	{
		cout << "Successfully opened " << rgbRecFileName << "." << endl;
	}
	else
	{
		cout << "Failed to open " << rgbRecFileName << "." << endl;
		exit(0);
	}

	// 计算原RGB图像总字节数
	fseek(rgbOriFilePtr, 0L, SEEK_END);
	rgbFileSize = ftell(rgbOriFilePtr);
	rewind(rgbOriFilePtr);
	cout << "The space that " << rgbOriFileName << " accounts for is " << rgbFileSize << " Bytes = " << rgbFileSize / 1024 << " kB." << endl;
	yuvFileSize = rgbFileSize / 2;

	// 建立缓冲区
	rgbOriBuffer = new unsigned char[rgbFileSize];
	yBuffer = new unsigned char[rgbFileSize / 3];
	uBuffer = new unsigned char[rgbFileSize / 12];		// 4:2:0格式
	vBuffer = new unsigned char[rgbFileSize / 12];
	rgbRecBuffer = new unsigned char[rgbFileSize];

	fread(rgbOriBuffer, sizeof(unsigned char), rgbFileSize, rgbOriFilePtr);	// RGB图像读入缓冲区
	rgb2yuv(yuvFilePtr, rgbFileSize, width, height, rgbOriBuffer, yBuffer, uBuffer, vBuffer);
	yuv2rgb(rgbRecFilePtr, yuvFileSize, width, height, yBuffer, uBuffer, vBuffer, rgbRecBuffer);
	errorData(yuvFileSize, rgbRecBuffer, argv);

	delete[]rgbOriBuffer;
	delete[]yBuffer;
	delete[]uBuffer;
	delete[]vBuffer;
	delete[]rgbRecBuffer;
	fclose(rgbOriFilePtr);
	fclose(yuvFilePtr);
	fclose(rgbRecFilePtr);
}