#ifndef __ANSWER03_H__
#define __ANSWER03_H__

#define VALID_SOLUTION 0
#define INVALID_FILENAME 1
#define INVALID_FLOOR_LENGTH 2
#define INVALID_COORDINATES 3
#define INVALID_FILE_SIZE 4
#define INSUFFICIENT_MEMORY 5
#define INVALID_FLOOR_CONTENT 6

typedef struct _tiling_solution {
   int floor_length;     // length of square floor, power of two >= 1
   int row, column;      // valid row and column coordinates
   char **floor_content; // address of 2-dimensional array for floor content
} tiling_solution;

char **allocate_2d_array(int n);

// first parameter should be non-NULL
//
void free_2d_array(char **array, int n)
                __attribute__((nonnull(1)));

// all parameters should be non-NULL
//
int determine_tiling_solution_category(char *filename, tiling_solution *ts)
                __attribute__((nonnull));

// all parameters should be non-NULL
//
bool check_solution(tiling_solution ts)
                __attribute__((nonnull));
//
//
bool check_add_x(tiling_solution *ts, int i, int j, int tile_type)
                __attribute__((nonnull));
//
//
int count_digits(int n)
                __attribute__((nonnull));
//
//
bool is_valid_filesize(tiling_solution *ts, FILE *fptr)
                __attribute__((nonnull));
//
//
int save_tiling_solution(char *filename, tiling_solution *ts)
                __attribute__((nonnull));
// all parameters non-NULL
//
void generate_tiling_solution(tiling_solution *ts)
                __attribute__((nonnull));
//
//
void recursive_tiling_generation(tiling_solution *ts, int row, int col, int length, int originx, int originy)
                __attribute__((nonnull));
//
//
void place_tile_type(tiling_solution *ts, int x, int y, char n, int *rows, int *cols)
                __attribute__((nonnull));
#endif
