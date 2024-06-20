#include <iostream>
using namespace std;
struct single_struct {
    int value=0;
    int possible_values[9] = { 1,2,3,4,5,6,7,8,9 };
    int possible_count=9;
};
single_struct** malloc_matrix();
void value_single(single_struct** matrix, int x, int y, int value);
void show_matrix(single_struct** matrix);
void show_matrix_g(single_struct** matrix);

int update(single_struct* c_single, int c_value);
void update_all(single_struct** c_sd_matrix);

int check_all_once(single_struct** c_sd_matrix);
int check_all(single_struct** c_sd_matrix);
void swap_value(single_struct** src,single_struct** dest);


int recursion_function(single_struct** g_sd_matrix,single_struct** c_sd_matrix,int layer=0);
