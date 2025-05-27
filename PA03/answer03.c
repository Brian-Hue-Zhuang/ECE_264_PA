#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "answer03.h"

bool check_solution(tiling_solution ts);
bool check_add_x(tiling_solution *ts, int i, int j, int tile_type);
int count_digits(int n);
bool is_base_two(int n);
bool is_valid_filesize(tiling_solution *ts, FILE *fptr);
void recursive_tiling_generation(tiling_solution *ts, int row, int col, int length, int originx, int originy);
void place_tile_type(tiling_solution *ts, int x, int y, char n, int *rows, int *cols);

char **allocate_2d_array(int n)
{
    char **arr;
    int i;
    arr = (char **)malloc(sizeof(char*) * n);
    if(!arr)
    {
        return NULL;
    }
    for(i = 0; i < n; i++)
    {
        arr[i] = malloc(sizeof(char) * n);
        if(!arr[i])
        {
            free_2d_array(arr, n);
            return NULL;
        }
    }
    return arr;
}

void free_2d_array(char **array, int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        free(array[i]);
    }
    free(array);
}

int determine_tiling_solution_category(char *filename, tiling_solution *ts)
{
    FILE *fptr = fopen(filename, "r");
    FILE *fptr_2 = fopen(filename, "r");
    int row, col, length, count;
    if(!fptr)
    {
        fprintf(stderr, "FAILURE filename\n");
        return INVALID_FILENAME;
    }
    if(!fptr_2)
    {
        return INVALID_FILENAME;
    }
    if(fscanf(fptr, "%d", &length) != 1 || length != ts->floor_length || length < 0 || ftell(fptr) != (count = count_digits(length)) || !is_base_two(length))
    {
        fclose(fptr);
        fclose(fptr_2);
        fprintf(stderr, "FAILURE length\n");
        return INVALID_FLOOR_LENGTH;
    }

    if(fscanf(fptr, "%d,%d", &row, &col) != 2 || row != ts->row || col != ts->column || ftell(fptr) != (count += count_digits(row) + count_digits(col) + 2))
    {
        fclose(fptr);
        fclose(fptr_2);
        fprintf(stderr, "FAILURE coords\n");
        return INVALID_COORDINATES;
    }
    if(!is_valid_filesize(ts, fptr_2))
    {
        fclose(fptr);
        fclose(fptr_2);
        fprintf(stderr, "FAILURE filesize\n");
        return INVALID_FILE_SIZE;
    }
    fclose(fptr_2);
    ts->floor_content = allocate_2d_array(length);
    if(!ts->floor_content)
    {
        fclose(fptr);
        fclose(fptr_2);
        fprintf(stderr, "FAILURE memory\n");
        return INSUFFICIENT_MEMORY;
    }
    int i, j; 
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < length; j++)
        {
            if(fscanf(fptr, " %c", &ts->floor_content[i][j]) != 1)
            {
                free_2d_array(ts->floor_content, length);
                fclose(fptr);
                fprintf(stderr, "FAILURE content\n");
                return INVALID_FLOOR_CONTENT;
            }
        }
    }
    if(!check_solution(*ts))
    {
        fclose(fptr);
        fprintf(stderr, "FAILURE content\n");
        return INVALID_FLOOR_CONTENT;
    }
    fclose(fptr);
    fprintf(stderr, "SUCCESS\n");
    return VALID_SOLUTION;
}

bool check_solution(tiling_solution ts)
{
    int i, j;
    int counter = 0;
    for(i = 0; i < ts.floor_length; i++)
    {
        for(j = 0; j < ts.floor_length; j++)
        {
            switch(ts.floor_content[i][j])
            {
                case '0':
                if(i != 0 && j !=0)
                {
                    if(ts.floor_content[i][j-1] == '0' && ts.floor_content[i-1][j] == '0')
                    {
                        if(check_add_x(&ts, i, j, 0))
                            counter += 3;
                    }
                }
                break;
                case '1':
                if(i != 0 && j != ts.floor_length - 1)
                {
                    if(ts.floor_content[i-1][j] == '1' && ts.floor_content[i][j+1] == '1')
                    {
                        if(check_add_x(&ts, i, j, 1))
                            counter += 3;
                    }
                }
                break;

                case '2':
                if(i != ts.floor_length - 1 && j != 0)
                {
                    if(ts.floor_content[i][j-1] == '2' && ts.floor_content[i+1][j] == '2')
                    {
                        if(check_add_x(&ts, i, j, 2))
                            counter += 3;
                    }
                }
                break;

                case '3':
                if(i != ts.floor_length - 1 && j != ts.floor_length - 1)
                {
                    if(ts.floor_content[i+1][j] == '3' && ts.floor_content[i][j+1] == '3')
                    {
                        if(check_add_x(&ts, i, j, 3))
                            counter += 3;
                    }
                }
                break;

                case '4':
                if(check_add_x(&ts, i, j, 4))
                    counter++;
                break;

                default:
                break;
            }
        }
    }
    if(counter != (ts.floor_length * ts.floor_length))
    {
        return false;
    }
    return true;
}

bool check_add_x(tiling_solution *ts, int i, int j, int tile_type)
{
    bool correct = false;
    switch(tile_type)
    {
        case 0:
        if((ts->floor_content[i - 1][j] & ts->floor_content[i][j - 1]) == '0')
        {
            correct = true;
        }
        ts->floor_content[i][j] = 'x';
        ts->floor_content[i - 1][j] = 'x';
        ts->floor_content[i][j - 1] = 'x';
        break;

        case 1:
        if((ts->floor_content[i - 1][j] & ts->floor_content[i][j + 1]) == '1')
        {
            correct = true;
        }
        ts->floor_content[i][j] = 'x';
        ts->floor_content[i - 1][j] = 'x';
        ts->floor_content[i][j + 1] = 'x';
        break;

        case 2:
        if((ts->floor_content[i + 1][j] & ts->floor_content[i][j - 1]) == '2')
        {
            correct = true;
        }
        ts->floor_content[i][j] = 'x';
        ts->floor_content[i + 1][j] = 'x';
        ts->floor_content[i][j - 1] = 'x';
        break;

        case 3:
        if((ts->floor_content[i + 1][j] & ts->floor_content[i][j + 1]) == '3')
        {
            correct = true;
        }
        ts->floor_content[i][j] = 'x';
        ts->floor_content[i + 1][j] = 'x';
        ts->floor_content[i][j + 1] = 'x';
        break;

        case 4:
        if(ts->floor_content[ts->row][ts->column])
        {
            correct = true;
        }
        ts->floor_content[i][j] = 'x';
        break;

        default:
        return false;
        break;
    }
    return correct;
}

bool is_base_two(int n)
{
    bool is_two = true;
    while(n != 1)
    {
        if(n % 2 != 0)
        {
            is_two = false;
            break;
        }
        n /= 2;
    }
    return is_two;
}

int count_digits(int n)
{
    int digits = 0;
    for(int i = n; i > 0; i /= 10)
    {
        digits++;
    }
    return digits;
}

bool is_valid_filesize(tiling_solution *ts, FILE *fptr)
{
    int file_size, expected_size;
    fseek(fptr, 0L, SEEK_END);
    file_size = ftell(fptr);
    expected_size = ts->floor_length * (ts->floor_length + 1) + count_digits(ts->row) + count_digits(ts->column) + count_digits(ts->floor_length) + 3;
    fprintf(stderr, "File: %d\nExpected: %d\n", file_size, expected_size);
    return expected_size == file_size ? true:false;
}

int save_tiling_solution(char *filename, tiling_solution *ts)
{
    FILE *fptr = fopen(filename, "w");
    int i, j;
    if(!fptr)
    {
        return 0;
    }
    fprintf(fptr, "%d\n%d,%d\n", ts->floor_length, ts->row, ts->column);
    for(i = 0; i < ts->floor_length; i++)
    {
        for(j = 0; j < ts->floor_length; j++)
        {
            fprintf(fptr, "%c", ts->floor_content[i][j]);
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
    return 1;
}

void generate_tiling_solution(tiling_solution *ts) 
{
    //allocate array, insert 4, declare necessary variables
    ts->floor_content = allocate_2d_array(ts->floor_length);
    if(!ts->floor_content)
    {
        return;
    }
    ts->floor_content[ts->row][ts->column] = '4';
    if(ts->floor_length > 1 || is_base_two(ts->floor_length))
    {
        recursive_tiling_generation(ts,ts->row,ts->column,ts->floor_length,0,0);
    }
}

void recursive_tiling_generation(tiling_solution *ts, int row, int col, int length, int originx, int originy)
{
    //Row and column are what parts of the array hold a value (our "4") 
    //length is half of how small said array being viewed is, origin is the top left corner of the quadrant
    length /= 2;
    int rows[4], cols[4];
    
    //a stopper for the recursive loop when examining a 2x2 part of the matrix
    if(length == 1)
    {
        if(row == (originx) && col == (1 + originy))
        {
            place_tile_type(ts, length + originx, length + originy, 1, rows, cols); //quad 1
            // fprintf(stderr, "end 1\n");
        }
        else if(row == (originx) && col == (originy))
        {
            place_tile_type(ts, length + originx, length + originy, 0, rows, cols); //quad 2
            // fprintf(stderr, "end 2\n");
        }
        else if(row == (1 + originx) && col == (originy))
        {
            place_tile_type(ts, length + originx, length + originy, 2, rows, cols); //quad 3
            // fprintf(stderr, "end 3\n");
        }
        else if(row == (1 + originx) && col == (1 + originy))
        {
            place_tile_type(ts, length + originx, length + originy, 3, rows, cols); //quad 4
            // fprintf(stderr, "end 4\n");
        }
        return;
    }

    //find which quadrant has a value, use correct L piece in center of 4 quadrants
    //All the quadrants are located where they would be mathematically on an x-y axis for the center
    //Recursion will shift the origin, row, and column
    if((col >= (length + originy)) && (row < (length + originx))) //quad 1
    {
        place_tile_type(ts, length + originx, length + originy, 1, rows, cols);
        rows[0] = row;
        cols[0] = col;
        // fprintf(stderr, "1\n");
    }
    else if((col < (length + originy)) && (row < (length + originx))) //quad 2
    {
        place_tile_type(ts, length + originx, length + originy, 0, rows, cols);
        rows[1] = row;
        cols[1] = col;
        // fprintf(stderr, "2\n");
    }
    else if((col < (length + originy)) && (row >= (length + originx))) //quad 3
    {
        place_tile_type(ts, length + originx, length + originy, 2, rows, cols);
        rows[2] = row;
        cols[2] = col;
        // fprintf(stderr, "3\n");
    }
    else if((col >= (length + originy)) && (row >= (length + originx))) //quad 4
    {
        place_tile_type(ts, length + originx, length + originy, 3, rows, cols);
        rows[3] = row;
        cols[3] = col;
        // fprintf(stderr, "4\n");
    }

    //4 recursive calls for each quadrant, will keep calling upon itself until it ends on a 2x2
    //updates row & column each time for respective quadrants
    originy += length;
    recursive_tiling_generation(ts,rows[0],cols[0],length,originx,originy); //quad 1 memory leak
    originy -= length;
    recursive_tiling_generation(ts,rows[1],cols[1],length,originx,originy); //quad 2 memory leak
    originx += length;
    recursive_tiling_generation(ts,rows[2],cols[2],length,originx,originy); //quad 3 memory leak
    originy += length;
    recursive_tiling_generation(ts,rows[3],cols[3],length,originx,originy); //quad 4 memory leak
    return;
}

void place_tile_type(tiling_solution *ts, int x, int y, char n, int *rows, int *cols) 
{
    rows[0] = x-1;
    cols[0] = y;
    rows[1] = x-1;
    cols[1] = y-1;
    rows[2] = x;
    cols[2] = y-1;
    rows[3] = x;
    cols[3] = y;
    switch(n)
    {
        case 0: //quad 2
        ts->floor_content[x-1][y] = '0';
        ts->floor_content[x][y] = '0';
        ts->floor_content[x][y-1] = '0';
        break;

        case 1: //quad 1
        ts->floor_content[x-1][y-1] = '1';
        ts->floor_content[x][y-1] = '1';
        ts->floor_content[x][y] = '1';
        break;
        
        case 2: //quad 3
        ts->floor_content[x-1][y-1] = '2';
        ts->floor_content[x-1][y] = '2';
        ts->floor_content[x][y] = '2';
        break;
        
        case 3: //quad 4
        ts->floor_content[x-1][y-1] = '3';
        ts->floor_content[x-1][y] = '3';
        ts->floor_content[x][y-1] = '3';
        break;
    }
}