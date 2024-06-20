#include <iostream>
#include "image.h"
#include "compute.h"
using namespace std;



int main()
{
    string fname;
    cout<<"input file path: ";
    cin>>fname;

    int** sd_matrix = new int* [9];
    
    int i = 0;
    int j = 0;
    for (i = 0; i < 9; i++)
    {
        sd_matrix[i] = new int[9];
        
    }
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            sd_matrix[i][j] = 0;
        }
    }

	image2text i2t;
    i2t.LoadImageSrc(fname);
    i2t.LoadBaseImages();   
    i2t.FindAllRect();
    i2t.GenerateFromImage();
    /*
    i2t.SwapValues();
    i2t.Suduko();
    i2t.OutPut();
    */
    i2t.CopyToOut(sd_matrix);
    i2t.SaveBigRect();
    init_matrix(sd_matrix);
   // recursion_function(gsd_matrix, sd_matrix);
    showresult();
   

    return 0;

}