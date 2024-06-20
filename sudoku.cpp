// sudoku.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "sudoku.h"




int update(single_struct* c_single, int c_value)
{
    int i = 0;
    int status = 0;


    for (i = 0; i < c_single->possible_count; i++)
    {
        if (status == 1)
        {
            c_single->possible_values[i - 1] = c_single->possible_values[i];
            if (i == (c_single->possible_count - 1))
            {
                c_single->possible_values[i] = 0;
            }
        }
        else
        {
            if (c_single->possible_values[i] == c_value)
            {
               
                c_single->possible_values[i] = 0;
                status = 1;
            }
        }

    }
    if (status == 1)
    {
        c_single->possible_count = c_single->possible_count - 1;
    }
    /*
    if (c_single->possible_count == 1)
    {
        c_single->value = c_single->possible_values[0];
        cout << "make sure: " << c_single->value << endl;
    }
    */
    return status;
}

int check_single(single_struct** c_sd_matrix, int x, int y)
{
    int i, j;
    int is, ie;
    int js, je;
    int change_times = 0;
    //vertical
    is = x;
    ie = x + 1;
    js = 0;
    je = 9;
    for (i = is; i < ie; i++)
    {
        for (j = js; j < je; j++)
        {
            if (c_sd_matrix[i][j].value != 0)
            {
                //cout << "update point: " << x << " " << y << " "<< c_sd_matrix[i][j].value<<endl;
                change_times= change_times+update(&c_sd_matrix[x][y], c_sd_matrix[i][j].value);
            }
        }
    }
    //horizonal
    is = 0;
    ie = 9;
    js = y;
    je = y+1;
    for (i = is; i < ie; i++)
    {
        for (j = js; j < je; j++)
        {
            if (c_sd_matrix[i][j].value != 0)
            {
                change_times = change_times + update(&c_sd_matrix[x][y], c_sd_matrix[i][j].value);
            }
        }
    }

    //unit
    is = (x/3)*3;
    ie = is+3;
    js = (y / 3) * 3;
    je = js+3;
    for (i = is; i < ie; i++)
    {
        for (j = js; j < je; j++)
        {
            if (c_sd_matrix[i][j].value != 0)
            {
                change_times = change_times + update(&c_sd_matrix[x][y], c_sd_matrix[i][j].value);
            }
        }
    }

    if (c_sd_matrix[x][y].possible_count <= 0)
        change_times = -1;
    if (c_sd_matrix[x][y].possible_count ==1)
        c_sd_matrix[x][y].value = c_sd_matrix[x][y].possible_values[0];
    return change_times;
}

int check_all_once(single_struct** c_sd_matrix)
{
    int i, j;
    int changed_times = 0;
    int sct = 0;
    for (i = 0; i < 9; i++)
    {       
        for (j = 0; j < 9; j++)
        {
            if (c_sd_matrix[i][j].value == 0)
            {
                sct= check_single(c_sd_matrix, i, j);
                if (sct == -1)
                    return -1;
                changed_times= changed_times+sct;
                
            }           
        }
    }
    //update_all(c_sd_matrix);
    return changed_times;
}
int check_all(single_struct** c_sd_matrix)
{
   
    int res=0;
    int change_times=0;
    while (1)
    {
        change_times= check_all_once(c_sd_matrix);
       // show_matrix(c_sd_matrix);
       // show_matrix_g(c_sd_matrix);
        if (change_times == 0)        
        {
            break;
        }
        if (change_times == -1)
        {
            return -1;
        }
    }

    int i = 0, j = 0, k = 0;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if(c_sd_matrix[j][i].value!=0)
                k++;            
        }
        
    }
   return 81-k;
}
int recursion_function(single_struct** g_sd_matrix,single_struct** c_sd_matrix,int layer)
{
    int ileft=0;
    int i,j,k,l;
    int ss;
    int lres = 0;
    int x = 0;
    int temp = 0;
    single_struct** l_sd_matrix=malloc_matrix();

    swap_value(c_sd_matrix,l_sd_matrix);

    ileft=check_all(l_sd_matrix);
    //show_matrix(l_sd_matrix);
   // show_matrix_g(l_sd_matrix);
    cout << "layer:" <<layer<<" left: " << ileft << endl;
    if (ileft == -1)
    {
        return -1;
    }
    if(ileft==0)
    {
        show_matrix(l_sd_matrix);
        swap_value(l_sd_matrix, g_sd_matrix);
        return 0;
    }
    else
    {
        //for(i=2;i<9;i++)
        {
            for(j=0;j<9;j++)
            {
                for(k=0;k<9;k++)
                {
                   
                    //if(l_sd_matrix[j][k].possible_count==i)
                    if (l_sd_matrix[j][k].possible_count >1)
                    {
                        ss = l_sd_matrix[j][k].possible_count;
                        lres = 0;
                        for (l = 0; l < ss; l++)
                        {
                            for (x = 0; x < layer; x++)
                            {
                                cout << "--";
                            }
                            cout << "  set " << j << " " << k << " " << l_sd_matrix[j][k].possible_values[l] << " {";
                            for (x = 0; x < ss; x++)
                            {
                                cout << " " << l_sd_matrix[j][k].possible_values[x];
                            }
                            cout << " }  ";

                            l_sd_matrix[j][k].value = l_sd_matrix[j][k].possible_values[l];
                            l_sd_matrix[j][k].possible_count = 1;
                            //show_matrix_g(l_sd_matrix);
                            temp = recursion_function(g_sd_matrix, l_sd_matrix, layer + 1);
                            if (0 == temp)
                            {
                                return 0;
                            }
                            if (-1 == temp)
                            {
                                cout << endl;
                                lres++;
                            }
                            if (temp > 0)
                            {
                                lres++;
                            }
                        }
                        if (lres == ss)
                        {   
                            for (x = 0; x < layer; x++)
                            {
                                cout << "--";
                            }
                            cout << "layer over " << layer << endl;
                            return -1;
                        }
                      
                    }
                }
            }
        }
    }

    
    return 0;
}
void swap_value(single_struct** src,single_struct** dest)
{
    int i = 0, j = 0,k=0;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            dest[i][j].value=src[i][j].value;
            dest[i][j].possible_count=src[i][j].possible_count;
            
            for(k=0;k<9;k++)
            {
                dest[i][j].possible_values[k]=src[i][j].possible_values[k];
            }
        }
        
        
    }
}
single_struct** malloc_matrix()
{
    single_struct** sd_matrix = new single_struct* [9];
    int i = 0;
    for (i = 0; i < 9; i++)
    {
        sd_matrix[i] = new single_struct[9];
    }
    return sd_matrix;
}
void show_matrix(single_struct** matrix)
{
    int i = 0, j = 0, k = 0;
    std::cout << "--------------------------------------------"<<std::endl;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if(matrix[j][i].value!=0)
                std::cout << " " << matrix[j][i].value;
            else
                std::cout << " " <<" ";
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << std::endl;
}

void show_matrix_g(single_struct** matrix)
{
    int i = 0, j = 0, k = 0;
    std::cout << "--------------------------------------------"<<std::endl;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (matrix[j][i].value != 0)
                std::cout << " [" << matrix[j][i].value << "]";
            else
            {
                std::cout << " [ " << "]";
                std::cout << "{";
                for (k = 0; k < 9; k++)
                {
                    if (matrix[j][i].possible_values[k] != 0)
                        std::cout << matrix[j][i].possible_values[k] << ",";
                }
                std::cout << "}";
            
            }

            
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << std::endl;
}

void value_single(single_struct** matrix, int x, int y, int value)
{
    matrix[x][y].value = value;
    matrix[x][y].possible_count = 1;
}



void update_all(single_struct** c_sd_matrix)
{
    int i = 0, j = 0, k = 0;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (c_sd_matrix[i][j].possible_count == 1)
            {
                c_sd_matrix[i][j].value = c_sd_matrix[i][j].possible_values[0];
            }
        }
       
    }

}