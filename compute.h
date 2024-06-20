#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>  
#include "sudoku.h"
using namespace cv;
using namespace std;

static int gclick_status = 0;
static cv::Point gclick_pos(0, 0);
static int matrix[9][9];
static int matrix_status[9][9];
static int block_size = 30;
static int block_inter = 3;

static Mat image;
static Mat bigrect;

void showresult(int block_size = 30);
void show_back();
void show_with_matrix();
void init_matrix(int ** in_matrix);
void show_select(Rect select,int value);



void MakeSureNumbers(int event, int x, int y, int flags, void* param);
Rect GetSelectRect(int x, int y);
int GetClickNum(int x,int y);
int GetSelectNum(int x,int y,Rect select,int oldvalue);

int CheckSD();