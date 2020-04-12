#pragma once
void rgbLookupTable();
void yuvLookupTable();
void rgb2yuv(FILE*, int, int, int, unsigned char*, unsigned char*, unsigned char*, unsigned char*);
void yuv2rgb(FILE*, int, int, int, unsigned char*, unsigned char*, unsigned char*, unsigned char*);
void errorData(int, unsigned char*, char* []);