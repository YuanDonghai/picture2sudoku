#include "compute.h"
void showresult(int block_size)
{
	//init_matrix();

	show_back();
	show_with_matrix();

	cv::namedWindow("MakeSure", cv::WINDOW_NORMAL);    
    cv::setMouseCallback("MakeSure", MakeSureNumbers, &image);
	int key = 0;
	
    cv::imshow("MakeSure", image);
    key = cv::waitKey(0);
	CheckSD();
	show_with_matrix();
	cv::imshow("MakeSure", image);
	key = cv::waitKey(0);
}
void show_back()
{
	int i, j;
	int x, y;
	image= cv::Mat::zeros(block_size * 9 + 10*block_inter, 2*(block_size * 9 + 10*block_inter), CV_8UC3);
	Mat src = imread("bigrect.jpg");
	resize(src, bigrect, Size(block_size * 9 + 10 * block_inter, block_size * 9 + 10 * block_inter));
	return ;
	image.setTo(cv::Scalar(255, 255, 255));

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			x = block_inter + i * (block_size + block_inter);
			y = block_inter + j * (block_size + block_inter);
			rectangle(image, cv::Rect(x,y, block_size, block_size), cv::Scalar(100, 100, 100), -1);		
		}
	}	

	
}
void show_with_matrix()
{
	//matrix
	int i, j;
	int x, y;
	//image= cv::Mat::zeros(block_size * 9 + 10*block_inter, block_size * 9 + 10*block_inter, CV_8UC3);
	//image.setTo(cv::Scalar(255, 255, 255));
	Mat temp= cv::Mat::zeros(block_size * 9 + 10*block_inter, block_size * 9 + 10*block_inter, CV_8UC3);
	temp.setTo(cv::Scalar(255, 255, 255));
	hconcat(temp, bigrect, image);
	//cv::imshow("MakeSure", image);
	//int key = cv::waitKey(0);
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			x = block_inter + i * (block_size + block_inter);
			y = block_inter + j * (block_size + block_inter);
			rectangle(image, cv::Rect(x,y, block_size, block_size), cv::Scalar(100, 100, 100), -1);		
		}
	}
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if(matrix[i][j]!=0)
			{
				cv:Point textPoint(block_inter + (i) * (block_size + block_inter)+3, block_inter + (j+1) * (block_size + block_inter)-7);
				if (matrix_status[i][j] == 1)
				{
					cv::putText(image, to_string(matrix[i][j]), textPoint, cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
				}
				else
				{
					cv::putText(image, to_string(matrix[i][j]), textPoint, cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
				}

			}
		}
	}
	//

}
void init_matrix(int ** in_matrix)
{	
    int i = 0, j = 0;
    for (i = 0; i < 9; i++)
    {
		for(j=0;j<9;j++)
		{
			matrix[j][i]=in_matrix[i][j];
			if(in_matrix[i][j]!=0)
				matrix_status[j][i] = 1;
			else
				matrix_status[j][i] = 0;
		}
    }
}

void show_select(Rect select,int value)
{
	int i=0;
	int index=0;
	cv::Point cpoint;
	cv::rectangle(image, select, cv::Scalar(255, 255, 255), -1);
	for(i=0;i<9;i++)
	{
		if(index==value)
		{
			index++;
		}
		//cv:Point textPoint(block_inter + (i) * (block_size + block_inter)+3, block_inter + (j+1) * (block_size + block_inter)-7);
		cpoint.x=select.x+(i%3)*(block_size + block_inter)+3;
		cpoint.y=select.y+(i/3+1)*(block_size + block_inter)-7;
		cv::putText(image, to_string(index), cpoint, cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255, 0), 2);
		index++;
	}
}
void MakeSureNumbers(int event, int x, int y, int flags, void* param)
{
	Mat* image = (Mat*)param;
	int key;
	Rect sub;
	int click_value=0;
	int select_value=0;
	int x_matrix=0,y_matrix=0;

    switch (event) {
    case cv::EVENT_LBUTTONDOWN:
		
		if(gclick_status==0)// go to select
		{
			show_with_matrix();
			gclick_pos.x = x;
			gclick_pos.y = y;
			//show_with_matrix();
			click_value=GetClickNum(x,y);
			sub = GetSelectRect(x, y);
			
			show_select(sub,click_value);
			gclick_status=1;
		}
		else
		{
			x_matrix=(gclick_pos.x-block_inter)/(block_size + block_inter);
			y_matrix=(gclick_pos.y-block_inter)/(block_size + block_inter);
			sub = GetSelectRect(gclick_pos.x, gclick_pos.y);
			
			select_value=GetSelectNum(x,y,sub,matrix[x_matrix][y_matrix]);
			if(-1 != select_value)
			{
				matrix[x_matrix][y_matrix]=select_value;
				matrix_status[x_matrix][y_matrix] = 1;
			}
			CheckSD();
			show_with_matrix();
			gclick_status=0;
		}
		
		//gclick_status = (gclick_status++) % 2;
		imshow("MakeSure", *image);
		key= cv::waitKey(0);
        break;
	case 0:
		break;
    default:

        break;
    }

    
}
Rect GetSelectRect(int x, int y)
{
	Rect res;	
	int xindex = (x - 3) / (3*(block_size + block_inter));
	int yindex=(y-3) / (3 * (block_size + block_inter));
	
	if (xindex > 6)
		xindex = 6;
	if (yindex > 6)
		yindex = 6;

	res.x= xindex* 3*(block_size + block_inter);
	res.y = yindex *3* (block_size + block_inter);
	res.width=3* (block_size + block_inter);
	res.height= 3 * (block_size + block_inter);
	return res;
}
int GetClickNum(int x,int y)
{
	int clicknum=0;
	int i=(x-block_inter)/(block_size + block_inter);
	int j=(y-block_inter)/(block_size + block_inter);
	return matrix[i][j];
}
int GetSelectNum(int x,int y,Rect select,int oldvalue)
{
	if(x<select.x || x>(select.x+select.width)||y<select.y ||y>(select.y+select.height))
		return -1;
	int index=((x-select.x)/(block_size + block_inter))+((y-select.y)/(block_size + block_inter))*3;
	if(index>=oldvalue)
		index++;
	
	return index;
}

int CheckSD()
{
	int i=0;
	int j=0;
	int res=0;
	single_struct** sd_matrix = malloc_matrix();
    single_struct** gsd_matrix = malloc_matrix();

 
    for (i = 0; i < 9; i++)
    {
		for(j=0;j<9;j++)
		{
			//sd_matrix[i][j]=matrix[i][j];
			if (matrix_status[i][j] == 1)
			{
				if (matrix[i][j] != 0)
				{
					value_single(sd_matrix, i, j, matrix[i][j]);
					cout << "set " << i << " " << j << " " << matrix[i][j] << endl;
				}
				else
				{
					matrix_status[i][j] = 0;
				}
			}
			else
			{
				matrix[i][j] = 0;
			}
			
			
		}
    }
	show_matrix(sd_matrix);
	res=recursion_function(gsd_matrix,sd_matrix);
	if (res == 0)
	{
		for (i = 0; i < 9; i++)
		{
			for (j = 0; j < 9; j++)
			{
				matrix[i][j] = gsd_matrix[i][j].value;
			}
		}
	}
	return res;
}