#include <iostream>
#include "declarations.h"
using namespace std;

void errorData(int yuvSize, unsigned char* rgbBuf, char* argv[])
{
	FILE* rgbOriFile = NULL;	// ԭʼRGBͼ���ļ�ָ��
	FILE* errorFile = NULL;	// ��������ļ�ָ��
	const char* rgbOriName = argv[1];	// ԭʼRGBͼ���ļ���
	const char* errorName = argv[4];	// ��������ļ���

	// ���ļ�
	if (fopen_s(&rgbOriFile, rgbOriName, "rb") == 0)
	{
		cout << "Successfully opened " << rgbOriName << "." << endl;
	}
	else
	{
		cout << "Failed to open " << rgbOriName << "." << endl;
		exit(0);
	}
	if (fopen_s(&errorFile, errorName, "w") == 0)
	{
		cout << "Successfully opened " << errorName << "." << endl;
	}
	else
	{
		cout << "Failed to open " << errorName << "." << endl;
		exit(0);
	}

	unsigned char* rgbOriBuf = new unsigned char[yuvSize * 2];
	fread(rgbOriBuf, sizeof(unsigned char), yuvSize * 2, rgbOriFile);

	// ��������������csv�ļ�
	fprintf(errorFile, "Pixel,B Error,G Error,R Error\n");
	for (int i = 0; i < yuvSize * 2 / 3; i++)
	{
		fprintf(errorFile, "%d,%d,%d,%d\n", i, (int)abs(rgbBuf[3 * i] - rgbOriBuf[3 * i]), (int)abs(rgbBuf[3 * i + 1] - rgbOriBuf[3 * i + 1]), (int)abs(rgbBuf[3 * i + 2] - rgbOriBuf[3 * i + 2]));
	}

	delete[]rgbOriBuf;
	fclose(rgbOriFile);
	fclose(errorFile);
}