/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * Using the C-API of this library.
 *
 */


#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "led-matrix-c.h"
#include "set.h"
#include "set.c"

#define PLAYER_PIXELMAP {{ {3,0,255,255,255},{4,0,255,255,255},{3,1,255,255,255},{4,1,255,255,255},{0,2,255,255,255},{1,2,255,255,255},{2,2,255,255,255},{3,2,255,255,255},{4,2,255,255,255},{5,2,255,255,255},{6,2,255,255,255},{7,2,255,255,255},{3,3,255,255,255},{4,3,255,255,255},{2,4,255,255,255},{3,4,255,255,255},{4,4,255,255,255},{5,4,255,255,255} },18}
#define SMALL_STAR_OBJ {{{1,0,255,238,0},{0,1,255,238,0},{1,1,255,238,0},{1,2,255,238,0},{2,1,255,238,0}},5}
#define MID_STAR_OBJ {{{1,0,171,107,68},{2,0,171,107,68},{3,0,171,107,68},{0,1,171,107,68},{1,1,171,107,68},{2,1,171,107,68},{3,1,171,107,68},{0,2,171,107,68},{1,2,171,107,68},{2,2,171,107,68}},10}
#define BIG_STAR_OBJ {{ {4,0,242,148,53},{5,0,242,148,53},{3,1,242,148,53},{6,1,242,148,53},{1,2,242,148,53},{2,2,242,148,53},{3,2,242,148,53},{6,2,242,148,53},{7,2,242,148,53},{8,2,242,148,53},{0,3,242,148,53},{9,3,242,148,53},{1,4,242,148,53},{8,4,242,148,53},{2,5,242,148,53},{7,5,242,148,53},{1,6,242,148,53},{1,7,242,148,53},{8,6,242,148,53},{8,7,242,148,53},{4,7,242,148,53},{5,7,242,148,53},{2,8,242,148,53},{3,8,242,148,53},{6,8,242,148,53},{7,8,242,148,53},{3,3,255,255,255},{1,3,255,238,0},{2,3,255,238,0},{2,4,255,238,0},{2,7,255,238,0},{3,4,255,238,0},{3,5,255,238,0},{3,6,255,238,0},{3,7,255,238,0},{2,6,255,238,0},{4,1,255,238,0},{4,2,255,238,0},{4,3,255,238,0},{4,4,255,238,0},{4,5,255,238,0},{4,6,255,238,0},{5,1,255,238,0},{5,2,255,238,0},{5,3,255,238,0},{5,4,255,238,0},{5,5,255,238,0},{5,6,255,238,0},{6,3,255,238,0},{6,4,255,238,0},{6,5,255,238,0},{6,6,255,238,0},{6,7,255,238,0},{7,3,255,238,0},{7,4,255,238,0},{7,6,255,238,0},{7,7,255,238,0},{8,3,255,238,0}},58}

#define STAR_TYPE_BIG_STAR 0
#define STAR_TYPE_MID_STAR 1
#define STAR_TYPE_SMALL_STAR 2

#define SCORE_SMALL_STAR 1
#define SCORE_MID_STAR 2
#define SCORE_BIG_STAR 5

#define HP_SMALL_STAR 1
#define HP_MID_STAR 2
#define HP_BIG_STAR 3

#define PLAYER_STARTING_X 12
#define PLAYER_STARTING_Y 59
#define RAND_0_4 rand()%5
#define RAND_0_1 -1*(rand()%3-1)
#define FRAME_TIME 300000

static int score = 0;
static int on_screen_stars_id[50];
static int on_screen_stars_num = 0;
volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
    interrupt_received = true;
}

// static long exist_stars_id [] = {}
// typedef struct {
//     int x,y,r,g,b;
// } point;
// typedef struct {
//     point pixel_map[60];
//     int pixel_number;
// }object;
// typedef struct {
//     object obj;
//     int hp;
//     long id;
//     int score;
// }stars;
// typedef struct {
//     object obj;
//     long id;
// }bullet;

static long pixel_matrix[32][64] = {0};
static void clear_pixel_matrix(){
    memset(pixel_matrix,0,sizeof(long)*32*64);
}
static void set_pixel(void *canvas,point a){
    led_canvas_set_pixel(canvas, a.x, a.y, a.r, a.g, a.b);
}
static void shift_down(object* obj){
    int length = obj->pixel_number;
    for(int i=0;i<length;i++){
        obj->pixel_map[i].y++;
    }
}
static void shift_up(object* obj){
    int length = obj->pixel_number;
    for(int i=0;i<length;i++){
        obj->pixel_map[i].y--;
    }
}
static void shift_right(object* obj){
    int length = obj->pixel_number;
    for(int i=0;i<length;i++){
        obj->pixel_map[i].x++;
    }
}
static void shift_left(object* obj){
    int length = obj->pixel_number;
    for(int i=0;i<length;i++){
        obj->pixel_map[i].x--;
    }
}

stars init_star(int type,long id){
    if(type == STAR_TYPE_SMALL_STAR){
        stars star = {SMALL_STAR_OBJ,HP_SMALL_STAR,id,SCORE_SMALL_STAR};
        printf("Small star init\n");
        return star;
    }
    if(type == STAR_TYPE_MID_STAR){
        stars star = {MID_STAR_OBJ,HP_MID_STAR,id,SCORE_MID_STAR};
        printf("mid star init\n");
        return star;
    }
    if(type == STAR_TYPE_BIG_STAR){
        stars star = {BIG_STAR_OBJ,HP_BIG_STAR,id,SCORE_BIG_STAR};
        printf("big star init\n");
        return star;
    }
    stars star;
    exit(1);
    return star;
}

static point create_laser(object* obj){
    point laser = obj->pixel_map[0];
    return laser;
}    


static void place_object(object* obj,int dx,int dy){
    int length = obj->pixel_number;
    for(int i=0;i<length;i++){
        obj->pixel_map[i].x+=dx;
        obj->pixel_map[i].y+=dy;
    }
}

static void set_pixel_map(void* canvas,const object obj){
        for(int i =0 ; i<obj.pixel_number;i++){
        set_pixel(canvas,obj.pixel_map[i]);
    }
}

static void check_key( char key,object* obj,void* canvas){
    switch(key)
    {
        case 'a': shift_left(obj); break;
        case 's': shift_down(obj); break;
        case 'd': shift_right(obj); break;
        case 'w': shift_up(obj); break;
    
    }
}

static void draw_laser(void* canvas,int start_x,int start_y){
    int r = 20;
    for(int i=start_y;i>=0;i--){
        if(r<=251){
            r+=4;
        }
        led_canvas_set_pixel(canvas,start_x,i,r,30,30);
    }
}
//-----------------------------------------------------------
int main(int argc, char **argv) {
    struct RGBLedMatrixOptions options;
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    int width, height;
    int x, y, i;
    // List *plist;
    // plist = (List *)malloc(sizeof(List));
    // list_init(plist,free);
    // stars star = init_star(STAR_TYPE_MID_STAR,-1);
    // stars star2 = init_star(STAR_TYPE_BIG_STAR,-2);
    // list_ins_next(plist,NULL,&star);
    // list_ins_next(plist,NULL,&star2);
    // print_list_int(plist);
    // stars* p = access_elem_by_id(plist,-1);
    // print_list_int(plist);
    // printf("%d\n",is_elem_in_list(plist,-1));
    // remove_elem_by_id(plist,-1);
    // print_list_int(plist);
    // printf("%d\n",is_elem_in_list(plist,-1));
    // stars temp = init_star(STAR_TYPE_SMALL_STAR,-1);
    memset(&options, 0, sizeof(options));
    options.rows = 32;
    options.cols = 64;
    options.chain_length = 1;
    
//     /* This supports all the led commandline options. Try --led-help */
    matrix = led_matrix_create_from_options(&options, &argc, &argv);
    if (matrix == NULL)
        return 1;

//     /* Let's do an example with double-buffering. We create one extra
//    * buffer onto which we draw, which is then swapped on each refresh.
//    * This is typically a good aproach for animations and such.
//    */
    offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);

    led_canvas_get_size(offscreen_canvas, &width, &height);

//     fprintf(stderr, "Size: %dx%d. Hardware gpio mapping: %s\n", width, height, options.hardware_mapping);
//     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//     // int x1=0;
//     // int y1=0;
//     // for(y1=0;y1<64;y1++){
//     //     for(x1=0;x1<32;x1++){
//     //         led_canvas_clear(offscreen_canvas);
//     //         led_canvas_set_pixel(offscreen_canvas, x1, y1, 255, 255, 255);
//     //         offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
//     //     }
//     // }
//     char keypressed;
//     object player= PLAYER_PIXELMAP;
//     point bullet={0,0,0,0,0};
//     //object big_star = big_star_pixel_map;
//     place_object(&player,PLAYER_STARTING_X,PLAYER_STARTING_Y);
//     set_pixel_map(offscreen_canvas,player);
//     offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
//         while(1){
//             // keypressed = getchar();
//             led_canvas_clear(offscreen_canvas);
//             // check_key(keypressed,&player,offscreen_canvas);
//             set_pixel_map(offscreen_canvas,player);
//             draw_laser(offscreen_canvas,0,59);



            



            
//             offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
//         }
    //place_object(&big_star,20,0);
    //set_pixel_map(offscreen_canvas,big_star);
    
    
   
    
    // object little_star = little_star_pixel_map;
    // object little_star1 = little_star_pixel_map;
    // object little_star2 = little_star_pixel_map;
    // object little_star3 = little_star_pixel_map;
    // object little_star4 = little_star_pixel_map;
    // place_object(&little_star,0+rand()%5,0);
    // place_object(&little_star1,6+rand()%5,0);
    // place_object(&little_star2,12+rand()%5,0);
    // place_object(&little_star3,18+rand()%5,0);
    // place_object(&little_star4,24+rand()%5,0);
    // set_pixel_map(offscreen_canvas,little_star1);
    // set_pixel_map(offscreen_canvas,little_star2);
    // set_pixel_map(offscreen_canvas,little_star3);
    // set_pixel_map(offscreen_canvas,little_star4);
    // set_pixel_map(offscreen_canvas,little_star);
    stars ls = init_star(STAR_TYPE_SMALL_STAR,-1);
    while(1){
        led_canvas_clear(offscreen_canvas);
        set_pixel_map(offscreen_canvas,ls.obj);
    //     set_pixel_map(offscreen_canvas,little_star2);
    //     set_pixel_map(offscreen_canvas,little_star3);
    //     set_pixel_map(offscreen_canvas,little_star4);
    //     set_pixel_map(offscreen_canvas,little_star);
    //     led_canvas_clear(offscreen_canvas);
    //     place_object(&little_star,0,rand()%5+1);
    //     place_object(&little_star1,0,rand()%5+1);
    //     place_object(&little_star2,0,rand()%5+1);
    //     place_object(&little_star3,0,rand()%5+1);
    //     place_object(&little_star4,0,rand()%5+1);
    //     set_pixel_map(offscreen_canvas,little_star1);
    //     set_pixel_map(offscreen_canvas,little_star2);
    //     set_pixel_map(offscreen_canvas,little_star3);
    //     set_pixel_map(offscreen_canvas,little_star4);
    //     set_pixel_map(offscreen_canvas,little_star);
        offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
        usleep(FRAME_TIME);
    }









   




   
 
    
    led_canvas_clear(offscreen_canvas);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);
    sleep(30);
    led_matrix_delete(matrix);

    return 0;
}
