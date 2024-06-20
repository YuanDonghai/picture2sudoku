
#include <opencv2/opencv.hpp>  
#include <iostream>  
#include <vector>  


using namespace cv;
using namespace std;

struct call_back_data {
    Mat* image;
    Mat* number9;
    int** sub_rect_matrix;
};
struct call_back_data_1 {
    Mat* image;
    int* res;
};
struct call_back_click_pos {
    int w;
    int h;
    int* numbers;
    int * pos;
    Mat* image;
};
class image2text {
public:
    Mat src;//原始图像，color
    Mat gray;//原始-灰度图像 gray
    Mat edges;//edges 用于gray之后做边界处理
    Mat big_rect;//从src中选择最大的矩形区域，相当于9*9的方格
    Mat big_rect_gray;//从gray中选择最大的矩形区域
    Mat thresholded;//二值图像 ，从big_rect_gray转化
    Rect sub_rect[81];//从big_rect区域中等分9*9的rect
    Mat basepic[9];//基准比对照片
    Mat number9;//数字图片，用于选择
    //functions
    int LoadImageSrc(String fname);

    void LoadBaseImages();
    // find the bigest area edges
    void FindAllRect();

    // erase the grid of bigest rect
    void EraseRectGrid(int gray_thresh);

    // generate sub rect
    void GenerateSubRect();

    //void InitMatrix();
    // check 
    void CheckAllNum();


    // generate values
    void GenerateFromImage();

    void SwapValues();
    // values into matrix
    void Suduko();

    // show
    void OutPut();

    void ShowImage(int image_type, String window_title);

    int ChangeValues();
    int ChangeValues_method1();

    void SaveBigRect();

    void CopyToOut(int ** outmatrix);
private:
    int find_area_max_from_contours(vector<vector<Point>> contours);
    int gray_values_distribution(int black_rate = 20);
    void generate_sub_rect();
    void bin_sub_rect_edge_distribution(int area_rate_min=5,int area_rate_max=80);
    int ger_sub_num_black_counts(Rect rect);
    //t save_sub_num
    // sub edges are �ɼ�sub rect����� �ٷֱ�
    int sub_rect_rate_min = 0;
    int sub_rect_rate_max = 0;
    int sub_rect_w_max = 0;
    int sub_rect_h_max = 0;
    
    Rect sub_rect_num[81];//81��rect
    Mat sub_rect_num_mat[81];
    int sub_rect_num_status[81];
    int sub_rect_matrix[9][9];

    int sd_sub_rect_num_status[81];
    int sd_sub_rect_matrix[9][9];

    double sub_rect_matrix_sim[9][9];
    int sub_sudoku_matrix[9][9];
    int sub_rect_num_max_index=0;

    int compare_bin_image(Mat src, Mat dest);

    // ���ڱȶ����ֵ�����
    int resize_w = 100;
    int resize_h = 150;

    void my_check_num(Mat num);


    //matrix
    

    void show_sub_rect_matrix();
    void sd_show_sub_rect_matrix();

    void check_results_1();// init get values
    void check_results_2();// check standard_deviation out of 4
    void check_results_3(int index[81],int len);// group check
    void check_results_4();// check single sim
    void check_results_5();// check status=1 and change to 3
    int check_results_6();// check if all can be sure
    double compare_images(Mat image1,Mat image2);
    int numbers_global=0;
    // Standard deviation
    double standard_deviation(double data[9],int len);
    double average(double data[9], int len);
};
void mouseHandler1(int event, int x, int y, int flags, void* param);
void mouseHandler1sub(int event, int x, int y, int flags, void* param);
