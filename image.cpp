
#include "image.h"

// private
int image2text::find_area_max_from_contours(vector<vector<Point>> contours)
{
    int index = 0;
    int i = 0;
    double area = contourArea(contours[0]);
    double swap = 0;
    for (i = 1; i < contours.size(); i++)
    {
        swap = contourArea(contours[i]);
        if (swap > area)
        {
            area = swap;
            index = i;
        }
    }
    return index;
}

void image2text::ShowImage(int image_type, String window_title)
{
    switch (image_type)
    {
    case 0:
        imshow(window_title, src);
        break;
    case 1:
        imshow(window_title, gray);
        break;
    case 2:
        imshow(window_title, thresholded);
        break;
    case 3:
        imshow(window_title, big_rect);
        break;
    default:
        break;
    }
    waitKey(0);
}
int image2text::gray_values_distribution(int black_rate)
{
    int ret_value = 0;
    int gray_values[256];

    int i = 0;
    for (i = 0; i < 256; i++)
    {
        gray_values[i] = 0;
    }
    // big_rect_gray_gray
    int l_rows = big_rect_gray.rows;
    int l_cols = big_rect_gray.cols;
    int x, y;
    for (x = 0; x < l_rows; x++)
    {
        for (y = 0; y < l_cols; y++)
        {
            gray_values[(int)big_rect_gray.at<uchar>(x, y)]++;
        }
    }

    long all_sum = l_rows * l_cols;
    long sum = 0;
    int rate = 0;
    for (i = 0; i < 256; i++)
    {
        sum = sum + gray_values[i];
        rate = sum * 100 / all_sum;
        if (rate >= black_rate)
        {
            ret_value = i;
            break;
        }
    }
    return ret_value;
}
void image2text::generate_sub_rect()
{
    int x = 0, y = 0, w = thresholded.cols, h = thresholded.rows;
    int index = 0;

    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            sub_rect[index].x = x + i * w / 9;
            sub_rect[index].y = y + j * h / 9;
            sub_rect[index].width = w / 9;
            sub_rect[index++].height = h / 9;
        }
    }
    for (i = 0; i < 81; i++)
    {
        rectangle(thresholded, sub_rect[i], Scalar(0, 0, 0), 1);
        //   rectangle(big_rect, sub_rect[i], Scalar(0, 0, 255), 1);
    }

}
void image2text::bin_sub_rect_edge_distribution(int area_rate_min, int area_rate_max)
{
    Mat l_temp;
    Mat l_save;
    string s_save, stemp;
    Rect r_temp, r_wbig;

    int i = 0;
    int j = 0;
    vector<vector<Point>> contours;

    //
    int dis_test[101];
    for (i = 0; i < 100; i++)
    {
        dis_test[i] = 0;
    }
    int dd = 0;
    //
    long unit_area = sub_rect[0].width * sub_rect[0].height;
    long sub_num_area_max = 0, sub_num_area_max_swap = 0;

    numbers_global=0;
    for (i = 0; i < 81; i++)
    {
        l_temp = thresholded(sub_rect[i]);
        findContours(l_temp, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        sub_rect_num_status[i] = 0;
        for (j = 0; j < contours.size(); j++)
        {
            sub_rect_num[i] = boundingRect(contours[j]);
            dd = sub_rect_num[i].width * sub_rect_num[i].height * 100 / unit_area;

            if (dd > area_rate_min && dd < area_rate_max)
            {
                sub_rect_num_mat[i] = l_temp(sub_rect_num[i]);
                sub_rect_num_status[i] = 1;
                dis_test[dd]++;
                sub_num_area_max_swap = sub_rect_num[i].width * sub_rect_num[i].height;
                if (sub_num_area_max_swap > sub_num_area_max)
                {
                    sub_num_area_max = sub_num_area_max_swap;
                    sub_rect_num_max_index = i;
                }

                r_wbig = sub_rect_num[i];
                r_wbig.x = sub_rect[i].x + sub_rect_num[i].x;
                r_wbig.y = sub_rect[i].y + sub_rect_num[i].y;

                numbers_global++;
               // rectangle(thresholded, r_wbig, Scalar(0, 0, 0), 1);
              //  ShowImage(2, "number");
                
                l_save = big_rect_gray(r_wbig);
                s_save = to_string(i);
                stemp = "D://1//" + s_save + ".jpg";
                imwrite(stemp, l_save);
                cout << " edge_distribution : " << i << " = " << " area: " << contourArea(contours[j]) <<" / " << sub_num_area_max_swap << " balck: "<< ger_sub_num_black_counts(r_wbig)<<endl;
                
            }
            //cout << " edge_distribution : " << i << " = " << j << " area: " << contourArea(contours[j]) << endl;
        }

         //rectangle(l_temp, r_temp, Scalar(0, 0, 0), 1);
         //imshow("window_title", l_temp);
         //waitKey(0);
    }
    for (i = 0; i < 100; i++)
    {
        if (dis_test[i] != 0)
        {
            if (sub_rect_rate_min == 0)
            {
                sub_rect_rate_min = i;
            }
            if (sub_rect_rate_max != 0)
            {
                sub_rect_rate_max = i;
            }
        }
        else
        {
            if (sub_rect_rate_min != 0 && sub_rect_rate_max == 0)
            {
                sub_rect_rate_max = i;
            }
        }
    }

}

int image2text::ger_sub_num_black_counts(Rect rect)
{
    int black_point_counts = 0;
    Mat l_mat = big_rect_gray(rect);
    for (int i_rows = 0; i_rows < l_mat.rows; i_rows++)
    {
        for (int i_cols = 0; i_cols < l_mat.cols; i_cols++)
        {
            if (thresholded.at<uchar>(i_rows, i_cols) == 0)
            {  // ��������Ǻ�ɫ��ֵΪ0��  
                black_point_counts++;
            }
        }
    }
    return black_point_counts;
}

void image2text::my_check_num(Mat num)
{
    // int resize_w = 100;
    int ws[100];
    int hs[150];

    int ws10[10] = { 0,0,0,0,0,0,0,0,0,0 };
    int hs10[15] = { 0,0,0,0,0,0,0,0,0,0 };

    int wi, hi;
    for (wi = 0; wi < resize_w; wi++)
    {
        ws[wi] = 0;
    }
    for (hi = 0; hi < resize_h; hi++)
    {
        hs[hi] = 0;
    }
    for (wi = 0; wi < resize_w; wi++)
    {
        for (hi = 0; hi < resize_h; hi++)
        {
            if (num.at<uchar>(hi, wi) == 0)
            {
                ws[wi]++;
                hs[hi]++;

                ws10[wi / 10]++;
                hs10[hi / 10]++;
            }
        }
    }

    //imshow("window_title", num);
    //waitKey(0);

    // �� 20%Ϊ��λ �����ֵ

    int x20[5] = { 0,0,0,0,0 };

    for (wi = 0; wi < resize_w; wi++)
    {
        x20[ws[wi] * 5 / resize_h]++;
    }

    int y20[5] = { 0,0,0,0,0 };
    for (hi = 0; hi < resize_h; hi++)
    {
        y20[hs[hi] * 5 / resize_w]++;
    }

    cout << "x20: [" << x20[0] << " " << x20[1] << " " << x20[2] << " " << x20[3] << " " << x20[4] << " ]" << endl;
    cout << "y20: [" << y20[0] << " " << y20[1] << " " << y20[2] << " " << y20[3] << " " << y20[4] << " ]" << endl;

    cout << "ws10: ";
    for (wi = 1; wi < 9; wi++)
    {
        //cout << " " << ws10[wi]*100/1500;
        cout << " " << ws10[wi];
    }
    cout << endl << "hs10: ";
    for (hi = 1; hi < 14; hi++)
    {
        // cout << " " << hs10[hi]*100/1000;
        cout << " " << hs10[hi];
    }
    cout << endl;

    int wwave = 0, hwave = 0;
    int wwave_water = 400, hwave_water = 300;
    for (wi = 1; wi < 9; wi++)
    {
        if ((ws10[wi] < wwave_water) && (ws10[wi + 1] >= wwave_water))
        {
            wwave++;
        }
    }
    for (hi = 1; hi < 14; hi++)
    {
        if ((hs10[hi] < hwave_water) && (hs10[hi + 1] >= hwave_water))
        {
            hwave++;
        }
    }
    cout << "wave: x=" << wwave << " y= " << hwave << endl;

    if (x20[4] > (resize_w / 10))
    {
        // cout << "--------------1 or 4" << endl;
        if (x20[4] > (resize_w / 5))
        {
            cout << "--------------1 " << endl;
        }
        else
        {
            cout << "--------------4" << endl;
        }
    }

    int temp1 = 0, temp2 = 0;
    if (y20[0] > (resize_h / 2))
    {
        //cout << "--------------2 7 " << x20[0]<<endl;
        // �鿴�ײ� 
        for (hi = resize_h * 4 / 5; hi < resize_h; hi++)
        {
            temp1 = temp1 + hs[hi];
        }
        for (hi = 0; hi < resize_h / 5; hi++)
        {
            temp2 = temp2 + hs[hi];
        }

        if (temp1 < temp2)
        {
            cout << "--------------2 " << temp1 << " " << temp2 << endl;
        }
        else
        {
            cout << "--------------7 " << temp1 << " " << temp2 << endl;
        }
    }

}

void image2text::show_sub_rect_matrix()
{
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (sub_rect_matrix[i][j] == 0 || sub_rect_num_status[j*9+i]==2)
            {
                cout << "   ";
            }
            else
            {
                cout << " " << sub_rect_matrix[i][j] << " ";
            }
        }
        cout << endl;
    }
    return ;
    cout<<"-----------"<<endl;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (sub_rect_num_status[j*9+i]!= 2)
            {
                cout << "   ";
            }
            else
            {
                cout << " " << sub_rect_matrix[i][j] << " ";
            }
        }
        cout << endl;
    }
}
void image2text::sd_show_sub_rect_matrix()
{
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (sd_sub_rect_num_status[j*9+i]==0)
            {
                cout << "   ";
            }
            else
            {
                cout << " " << sd_sub_rect_matrix[i][j] << " ";
            }
        }
        cout << endl;
    }
}

// public
int image2text::LoadImageSrc(String fname)
{
    src = imread(fname);
    if (src.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }
    return 0;
}
void image2text::FindAllRect()
{
    // ת��Ϊ�Ҷ�ͼ��  
    cvtColor(src, gray, COLOR_BGR2GRAY);

    // ���б�Ե���  
    Canny(gray, edges, 50, 150);

    // �ҵ�����  
    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int bigest_area_index = find_area_max_from_contours(contours);
    Rect bigest_rect = boundingRect(contours[bigest_area_index]);
    big_rect = src(bigest_rect);
    big_rect_gray = gray(bigest_rect);

    // big_rect_gray_gray=
}
void image2text::EraseRectGrid(int gray_thresh)
{
    if(gray_thresh==0)
    {
        gray_thresh = gray_values_distribution(15);
    }
   // int gray_thresh = gray_values_distribution(15);
    threshold(big_rect_gray, thresholded, gray_thresh, 255, cv::THRESH_BINARY);
   //  threshold(big_rect_gray, thresholded, 200, 255, cv::THRESH_BINARY);
     //ShowImage(1, "gray");
     
    int black_point_rate = 50;

    int l_rows = thresholded.rows;
    int l_cols = thresholded.cols;

    int i_counts = 0;

    Rect erase_line;


    // rows
    int i_rows = 0, i_cols = 0;
    for (i_rows = 0; i_rows < l_rows; i_rows++)
    {
        i_counts = 0;
        for (i_cols = 0; i_cols < l_cols; i_cols++)
        {
            if (thresholded.at<uchar>(i_rows, i_cols) == 0)
            {  // ��������Ǻ�ɫ��ֵΪ0��  
                i_counts++;
            }
        }

        if (i_counts * 100 / l_cols >= black_point_rate)
        {
            //cout << "rows : " << i_rows << " counts: " << i_counts << "rate:" << i_counts * 100 / l_cols << endl;
            erase_line.x = 0;
            erase_line.y = i_rows;
            erase_line.width = l_cols;
            erase_line.height = 1;
            rectangle(thresholded, erase_line, Scalar(255, 0, 0), 1);
            rectangle(big_rect_gray, erase_line, Scalar(255, 255, 255), 1);
        }
    }

    // cols
    i_rows = 0, i_cols = 0;
    for (i_cols = 0; i_cols < l_cols; i_cols++)
    {
        i_counts = 0;
        for (i_rows = 0; i_rows < l_rows; i_rows++)
        {
            if (thresholded.at<uchar>(i_rows, i_cols) == 0)
            {  // ��������Ǻ�ɫ��ֵΪ0��  
                i_counts++;
            }
        }
        if (i_counts * 100 / l_rows >= black_point_rate)
        {
            // cout << "cols : " << i_cols << " counts: " << i_counts << "rate:" << i_counts * 100 / l_rows << endl;
            erase_line.x = i_cols;
            erase_line.y = 0;
            erase_line.width = 1;
            erase_line.height = l_rows;
            rectangle(thresholded, erase_line, Scalar(255, 0, 0), 1);
        }

    }


    // ShowImage(2, "bin");
     /*
     int totalBlackPixels = 0;
     for (int y = 0; y < thresholded.rows; ++y) {
         for (int x = 0; x < thresholded.cols; ++x) {
             if (thresholded.at<uchar>(y, x) == 0) {  // ��������Ǻ�ɫ��ֵΪ0��
                 totalBlackPixels++;
             }
         }
     }
     */
}
void image2text::GenerateSubRect()
{
    generate_sub_rect();
    bin_sub_rect_edge_distribution();
}

void image2text::LoadBaseImages()
{
    int i = 0;
    Mat temp;
    string str;
    for (i = 0; i < 9; i++)
    {
        str = "pic\\a" + to_string(i + 1) + ".jpg";
        temp = imread(str, IMREAD_GRAYSCALE);
        threshold(temp, basepic[i], 180, 255, cv::THRESH_BINARY);
    }
    number9 = imread("pic\\nums.jpeg", IMREAD_COLOR);
}
void image2text::CheckAllNum()
{
    check_results_1();
    check_results_2();
    
    check_results_5();
   // show_sub_rect_matrix();
    cout<<endl;
  //  sd_show_sub_rect_matrix();
    return;
    Mat diff;
    int i = 0;
    int j = 0;
    int k=0;
    double similarity = 0, swap;
    int big_int = 0;
    Mat num, basenum;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            sub_rect_matrix[i][j] = 0;
        }
    }
    for (i = 0; i < 81; i++)
    {

        if (sub_rect_num_status[i] == 1)
        {
            similarity = 0;
            big_int = 1;
            for (j = 0; j < 9; j++)
            {
                resize(sub_rect_num_mat[i], num, Size(100, 150));
                resize(basepic[j], basenum, Size(100, 150));
                //absdiff(sub_rect_num_mat[i],basepic[j], diff); // �������ֵ��
                absdiff(num, basenum, diff);

                swap = 1.0 - (countNonZero(diff) / (double)(num.total())); // �������ƶ� 
                //   cout << "simi with " << i << " j= " << j+1 << " sim: " << swap << " man: "<< compare_bin_image(num, basenum)<<endl;
                //cout << "simi with " << i << " j= " << j + 1 << " sim: " << swap << " man: " << endl;
                if (swap > similarity)
                {
                    similarity = swap;
                    big_int = j + 1;
                }

            }
            //cout << "big simi: i="<<i<<" match num= " << big_int << " per= "<< similarity << endl;
            sub_rect_matrix[i % 9][i / 9] = big_int;
            sub_rect_matrix_sim[i % 9][i / 9]=similarity;
            //value_single(sd_matrix, i % 9, i / 9, big_int);
            //value_single(sd_matrix, i / 9, i % 9, big_int);
            //my_check_num(num);
        }

    }
    double temp_sim_sum=0;
    double temp_second_sum=0;
    double temp_standard_deviation=0;
    double temp_sd_data[9]={0,0,0,0,0,0,0,0,0};
    int temp_sim_counts=0,second_com_index=0;
    int special_num_pos[9] = { 0,0,0,0,0,0,0,0,0 };
    int l=0;
    for(i=1;i<=9;i++)
    {
        cout<<"first makesure value="<<i<<endl;
        temp_sim_counts=0;

  
        for(j=0;j<81;j++)
        {
            if(sub_rect_matrix[j % 9][j / 9] == i)
            {
                cout<<"["<<j<<"] sim="<<sub_rect_matrix_sim[j % 9][j / 9]<<endl;
                temp_sim_sum=temp_sim_sum+sub_rect_matrix_sim[j % 9][j / 9];
                special_num_pos[temp_sim_counts]=j;
                temp_sd_data[temp_sim_counts] = sub_rect_matrix_sim[j % 9][j / 9] * 100;
                temp_sim_counts++; 
            }
        }



        cout<<"second compare "<<i<<" counts="<<temp_sim_counts<<endl;
        for(k=0;k<temp_sim_counts;k++)
        {
            cout<<"second "<<special_num_pos[k]<<endl;
            l=0;
            for(j=0;j<temp_sim_counts;j++)
            {
                if(j!=k)
                {
                    resize(sub_rect_num_mat[special_num_pos[k]], num, Size(100, 150));
                    resize(sub_rect_num_mat[special_num_pos[j]], basenum, Size(100, 150));
                    //absdiff(sub_rect_num_mat[i],basepic[j], diff); // �������ֵ��
                    absdiff(num, basenum, diff);
                    swap = 1.0 - (countNonZero(diff) / (double)(num.total())); 
                    //temp_sd_data[l++]=swap*100;
                    cout<<"second " <<special_num_pos[k]<<" with "<<special_num_pos[j]<<" sim= "<<swap<<endl;
                }
            }
           
           // resize(sub_rect_num_mat[i], num, Size(100, 150));
            //resize(basepic[j], basenum, Size(100, 150));
        }
        
        temp_standard_deviation = standard_deviation(temp_sd_data, temp_sim_counts );
        //if (temp_standard_deviation < 1)
            cout << "second standard_deviation " << temp_standard_deviation << endl;

        cout<<"---------------------"<<endl;
    }
    show_sub_rect_matrix();
}

int image2text::compare_bin_image(Mat src, Mat dest)
{
    Mat result(src.size(), CV_8U);
    int i, j;
    for (i = 0; i < src.rows; i++)
    {
        for (j = 0; j < src.cols; j++)
        {
            if (src.at<uchar>(i, j) == 0 || dest.at<uchar>(i, j) == 0)
            {
                result.at<uchar>(i, j) = 0;
            }
            //  result.at<uchar>(i, j) = src.at<uchar>(i, j) ^ dest.at<uchar>(i, j);
        }
    }
    long black_counts = 0;
    for (i = 0; i < src.rows; i++)
    {
        for (j = 0; j < src.cols; j++)
        {
            if (result.at<uchar>(i, j) == 0)
            {
                black_counts++;
            }
        }
    }
    int rate = black_counts * 100 / src.rows / src.cols;
    return rate;

}


int image2text::ChangeValues_method1()
{
    // set back call
    int i;
    call_back_click_pos trans;
    Mat temp_show;

    int click_pos;
    int input_num=0;
    trans.w=big_rect.cols;
    trans.h=big_rect.rows;
    trans.pos=&click_pos;
    trans.numbers=&input_num;
    trans.image = &temp_show;
    // call back
   // cv::namedWindow("MakeSure1", cv::WINDOW_NORMAL);
   // cv::setMouseCallback("MakeSure1", MakeSureNumbers, &trans);




    string temp;
    int key = 0;
    // show images
    while (1)
    {
        cout<<"pos="<<click_pos<<"input:"<<input_num<<endl;
        temp_show = big_rect;
        for (i = 0; i < 81; i++)
        {
            if (sub_rect_num_status[i] == 1)
            {
                temp = to_string(sub_rect_matrix[i % 9][i / 9]);
                //rectangle(big_rect, sub_rect[i], Scalar(0, 0, 255), 1);
                cv::putText(temp_show, temp, cv::Point(sub_rect[i].x + sub_rect[i].width / 2, sub_rect[i].y + sub_rect[i].height / 2), cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
            }
        }

        cv::imshow("MakeSure1", temp_show);
        key = cv::waitKey(0);
        //cout << "key: " << key << endl;
        if (key == 27 ||key==-1 ) { // 27��ESC����ASCII��  
            break;
        }
    }

    return 0;
}
int image2text::ChangeValues()
{
    // set back call

    int** rect_call_back = new int* [9];
    int i = 0, j = 0;
    for (i = 0; i < 9; i++)
    {
        rect_call_back[i] = new int[9];
    }

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            rect_call_back[i][j] = sub_rect_matrix[i][j];
        }
    }
    call_back_data trans;
    trans.image = &big_rect;
    trans.sub_rect_matrix = rect_call_back;
    trans.number9 = &number9;

    Mat temp_show = big_rect;

    cv::namedWindow("MakeSure", cv::WINDOW_NORMAL);
    //cv::setMouseCallback("Image Window", mouseHandler, &big_rect);
    cv::setMouseCallback("MakeSure", mouseHandler1, &trans);

    string temp;
    int key = 0;
    // show images
    while (1)
    {
        for (i = 0; i < 81; i++)
        {
            if (sub_rect_num_status[i] == 1)
            {
                temp = to_string(rect_call_back[i % 9][i / 9]);
                //rectangle(big_rect, sub_rect[i], Scalar(0, 0, 255), 1);
                cv::putText(temp_show, temp, cv::Point(sub_rect[i].x + sub_rect[i].width / 2, sub_rect[i].y + sub_rect[i].height / 2), cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
            }
        }

        cv::imshow("MakeSure", temp_show);
        key = cv::waitKey(0);
        cout << "key: " << key << endl;
        if (key == 27 || key == -1) { // 27��ESC����ASCII��  
            break;
        }
    }

    return 0;
}

void mouseHandler1(int event, int x, int y, int flags, void* param)
{
    //  cv::Mat* image = (cv::Mat*)param;
    if (event == 0)
    {
        return;
    }
    call_back_data* trans = (call_back_data*)param;
    Mat* image = trans->image;
    Mat* numbers = trans->number9;

    int sub_rect_h = image->rows / 9;
    int sub_rect_w = image->cols / 9;
    Rect click_rect, rect_o_img;
    Mat click_mat;
    Mat matToFillleft, lshow;
    Mat mat1;
    Mat mat2(223, 36, CV_8UC3);
    int key = 0;
    int res = 0;
    int lstatus = 0;
    call_back_data_1 sub_data;
    sub_data.image = &lshow;
    sub_data.res = &res;


    string temp;
    switch (event) {
    case cv::EVENT_LBUTTONDOWN: // ���������  
        std::cout << "Mouse left button clicked at position: (" << x << ", " << y << ")" << std::endl;
        click_rect.x = x / sub_rect_w * sub_rect_w;
        click_rect.y = y / sub_rect_h * sub_rect_h;
        click_rect.width = sub_rect_w;
        click_rect.height = sub_rect_h;
        click_mat = (*image)(click_rect);

        resize(click_mat, mat1, Size(36, 52));


        vconcat(mat1, mat2, matToFillleft);
        // cv::imshow("sure", matToFillleft);
      //   cv::waitKey(0);
        cout << matToFillleft.channels() << " " << numbers->channels() << endl;
        hconcat(matToFillleft, *numbers, lshow);


        lstatus = 1;



        // cv::circle(*image, cv::Point(x, y), 10, cv::Scalar(0, 0, 255), -1);
        break;
    default:
        break;
    }

    while (lstatus == 1)
    {
        //click_mat.copyTo(matToFill);
        cv::namedWindow("SubSelect", cv::WINDOW_NORMAL);
        //cv::setMouseCallback("Image Window", mouseHandler, &big_rect);
        cv::setMouseCallback("SubSelect", mouseHandler1sub, &sub_data);
        temp = to_string(res);
        cv::putText(lshow, temp, cv::Point(5, 60), cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
        cv::imshow("SubSelect", lshow);
        key = cv::waitKey(10);
        cout << "key: " << key << " res=" << res << endl;
        if (key == 27 ) { // 27��ESC����ASCII��  
            break;
        }
    }
}
void mouseHandler1sub(int event, int x, int y, int flags, void* param)
{
    call_back_data_1* locals = (call_back_data_1*)param;
    Mat* image = locals->image;
    int* res = locals->res;
    int i = 0, j = 0;
    switch (event) {
    case cv::EVENT_LBUTTONDOWN:
        i = (x - 36) / 92;
        y = y / 92;
        *res = y * 3 + x;
        break;
    default:
        break;
    }
}



double image2text::standard_deviation(double data[9],int len)
{
    double res=0;
    double average=0;
    double sum=0;
    int i;
    for(i=0;i<len;i++ )
    {
        sum=sum+data[i];
    }
    average=sum/len;
    sum=0;
    for(i=0;i<len;i++ )
    {
        sum=sum+pow((data[i]-average),2);
    }
    
    res=sqrt(sum/len);
    return res;
}

void image2text::check_results_1()
{
    Mat diff;
    int i = 0;
    int j = 0;
    int k=0;
    double similarity = 0, swap;
    int big_int = 0;
    Mat num, basenum;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            sub_rect_matrix[i][j] = 0;
        }
    }
    for (i = 0; i < 81; i++)
    {
        if (sub_rect_num_status[i] == 1)
        {
            similarity = 0;
            big_int = 1;
            for (j = 0; j < 9; j++)
            {
                swap=compare_images(sub_rect_num_mat[i],basepic[j]);               
                if (swap > similarity)
                {
                    similarity = swap;
                    big_int = j + 1;
                }
            }           
            sub_rect_matrix[i % 9][i / 9] = big_int;
            sub_rect_matrix_sim[i % 9][i / 9]=similarity;         
        }
    }
}
void image2text::check_results_2()
{

    int i,j;
    int temp_sim_counts;
    double temp_sim_sum=0;
    int special_num_pos[81];
    double temp_sd_data[81];
    double temp_standard_deviation=0;

    for(i=1;i<=9;i++)
    {
        //out<<"check_results_2 value="<<i<<endl;
        temp_sim_counts=0;  
        for(j=0;j<81;j++)
        {
            if(sub_rect_matrix[j % 9][j / 9] == i)
            {
                //cout<<"["<<j<<"] sim="<<sub_rect_matrix_sim[j % 9][j / 9]<<endl;

                temp_sim_sum=temp_sim_sum+sub_rect_matrix_sim[j % 9][j / 9];
                special_num_pos[temp_sim_counts]=j;
                temp_sd_data[temp_sim_counts] = sub_rect_matrix_sim[j % 9][j / 9] * 100;
                temp_sim_counts++; 
            }
        }


        //cout<<"counts= "<<temp_sim_counts;
        temp_standard_deviation = standard_deviation(temp_sd_data, temp_sim_counts );

        //cout << "second standard_deviation " << temp_standard_deviation << endl;
        if (temp_standard_deviation > 4)
        {
            check_results_3(special_num_pos,temp_sim_counts);
        }
        else
        {
            //打上标记，已经证明可以确定了

        }
        

        //cout<<"---------------------"<<endl;
    }
}
void image2text::check_results_3(int index[81],int len)// group check
{
    int i=0,j=0;
    int k = 0;
    double swap;
    double sd_data[81];
    int sd_data_len=0;
    for(i=0;i<len;i++)
    {
        //cout<<" group sd: "<<index[i]<<endl;
        sd_data_len=0;
        for(j=0;j<len;j++)
        {
            if(i!=j)
            {
                sd_data[sd_data_len++]=compare_images(sub_rect_num_mat[index[i]],sub_rect_num_mat[index[j]])*100;
                //cout<<" with "<<index[j]<<" "<<compare_images(sub_rect_num_mat[index[i]],sub_rect_num_mat[index[j]])<<endl;
            }
        }
        swap=standard_deviation(sd_data,sd_data_len);
        //cout<<"group sd= "<<swap<<endl;
        // bigger 10
        if (swap < 10)
        {
            k = index[i];
          //  sub_rect_num_status[(index[i])]=2;
            sub_rect_num_status[k] = 2;
        }
    }
}
void image2text::check_results_4()// check status=2 sim
{
    int i=0;
    int j = 0;
    int k = 0;
    double swap = 0;
    double sd_data[81];
    int sd_data_len = 0;
    int sd_min = 0;
    for (i = 0; i < 81; i++)
    {
        if (sub_rect_num_status[i] == 2)
        {

            for (j = 0; j < 9; j++)
            {
                sd_data_len = 0;
                //swap = compare_images(sub_rect_num_mat[i], basepic[j]);
                sd_data[sd_data_len++]= compare_images(sub_rect_num_mat[i], basepic[j])*100;
                for (k = 0; k < 81; k++)
                {
                    if (sub_rect_num_status[k] == 1 && sub_rect_matrix[k % 9][k / 9] == (j + 1))
                    {
                        //cout << "compare with " << k << " " << sub_rect_matrix[k % 9][k / 9] << endl;
                        sd_data[sd_data_len++] = compare_images(sub_rect_num_mat[k], sub_rect_num_mat[i])*100;
                    }
                }
                swap = standard_deviation(sd_data, sd_data_len);
                cout << "check_results_4  " << i << " with " << j << " sd= " << swap <<" ave: "<< average (sd_data, sd_data_len)<< endl;
            }
        }
    }
}
void image2text::check_results_5()// check status=2 sim
{
    int i=0;
    int j = 0;
    int k = 0;
    double swap = 0;
    double sd_data[81];
    int sd_data_len = 0;
    int sd_min = 0;
    for (i = 0; i < 81; i++)
    {
        if(sub_rect_num_status[i]==1&&sd_sub_rect_num_status[i]!=1)
        {
            sd_sub_rect_num_status[i]=1;            
            sd_sub_rect_matrix[i % 9][i / 9]=sub_rect_matrix[i % 9][i / 9];
        }

    }
}
int image2text::check_results_6()
{
    int i, j;
    int res=0;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (sd_sub_rect_num_status[j*9+i]==1)
            {
                res++;
            }
        }       
    }
    return abs(res- numbers_global);
}

double image2text::compare_images(Mat image1,Mat image2)
{
    double swap;
    Mat c1,c2,diff;
    resize(image1, c1, Size(100, 150));
    resize(image2, c2, Size(100, 150));
    absdiff(c1, c2, diff);
    swap = 1.0 - (countNonZero(diff) / (double)(diff.total()));
    return swap;
}
double image2text::average(double data[9], int len)
{
  
    double sum = 0;
    int i;
    for (i = 0; i < len; i++)
    {
        sum = sum + data[i];
    }
    return  sum / len;
}

void image2text::GenerateFromImage()
{
    int i=0;
    int left=0;
    int gray_th=140;
    EraseRectGrid(0);

        GenerateSubRect();

        CheckAllNum();
        left=check_results_6();
        sd_show_sub_rect_matrix();
       // cout<<"check gray: "<<gray_th<<" left: "<<left<<endl;
        return;
    for(i=0;i<4;i++)
    {
        EraseRectGrid(gray_th);

        GenerateSubRect();

        CheckAllNum();
        left=check_results_6();
      //  cout<<"check gray: "<<gray_th<<" left: "<<left<<endl;
        gray_th=gray_th+20;
    }
    
}
void image2text::CopyToOut(int ** outmatrix)
{
    int i, j;
    
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
           outmatrix[i][j]=sd_sub_rect_matrix[i][j];
        }       
    }

}
void image2text::SaveBigRect()
{    
    cv::imwrite("bigrect.jpg", big_rect);
}