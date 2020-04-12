#include <iostream>
#include "declarations.h"
using namespace std;

int main(int argc, char* argv[])
{
	FILE* rgbOriFilePtr = NULL;	// ԭRGBͼ����ļ�ָ��
	FILE* yuvFilePtr = NULL;	// YUVͼ����ļ�ָ��
	FILE* rgbRecFilePtr = NULL;	// ��ԭ��RGB�ļ����ļ�ָ��
	const char* rgbOriFileName = argv[1];	// ԭRGBͼ���ļ���
	const char* yuvFileName = argv[2];	// YUVͼ���ļ���
	const char* rgbRecFileName = argv[3];	// ��ԭRGBͼ���ļ���
	int width = 256;	// ͼ���
	int height = 256;	// ͼ���
	int rgbFileSize;	// RGBͼ�����ֽ���
	int yuvFileSize;	// YUVͼ�����ֽ���
	unsigned char* rgbOriBuffer = NULL;	// ԭRGBͼ�񻺳���
	unsigned char* yBuffer = NULL;		// Y����������
	unsigned char* uBuffer = NULL;		// U����������
	unsigned char* vBuffer = NULL;		// V����������
	unsigned char* rgbRecBuffer = NULL;	// ��ԭRGBͼ�񻺳���

	// ���ļ�
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

	// ����ԭRGBͼ�����ֽ���
	fseek(rgbOriFilePtr, 0L, SEEK_END);
	rgbFileSize = ftell(rgbOriFilePtr);
	rewind(rgbOriFilePtr);
	cout << "The space that " << rgbOriFileName << " accounts for is " << rgbFileSize << " Bytes = " << rgbFileSize / 1024 << " kB." << endl;
	yuvFileSize = rgbFileSize / 2;

	// ����������
	rgbOriBuffer = new unsigned char[rgbFileSize];
	yBuffer = new unsigned char[rgbFileSize / 3];
	uBuffer = new unsigned char[rgbFileSize / 12];		// 4:2:0��ʽ
	vBuffer = new unsigned char[rgbFileSize / 12];
	rgbRecBuffer = new unsigned char[rgbFileSize];

	fread(rgbOriBuffer, sizeof(unsigned char), rgbFileSize, rgbOriFilePtr);	// RGBͼ����뻺����
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