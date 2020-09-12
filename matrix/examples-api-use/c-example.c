/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * Using the C-API of this library.
 *
 */
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "led-matrix-c.h"
volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
    interrupt_received = true;
}

int main(int argc, char **argv) {
    struct RGBLedMatrixOptions options;
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    int width, height;
    //int x, y, i;

    memset(&options, 0, sizeof(options));
    options.rows = 32;
    options.cols = 64;
    options.chain_length = 1;

    /* This supports all the led commandline options. Try --led-help */
    matrix = led_matrix_create_from_options(&options, &argc, &argv);
    if (matrix == NULL)
        return 1;

    /* Let's do an example with double-buffering. We create one extra
   * buffer onto which we draw, which is then swapped on each refresh.
   * This is typically a good aproach for animations and such.
   */
    offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);

    led_canvas_get_size(offscreen_canvas, &width, &height);

    fprintf(stderr, "Size: %dx%d. Hardware gpio mapping: %s\n", width, height, options.hardware_mapping);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    
    // int x1=0;
    // int y1=0;
    // for(y1=0;y1<64;y1++){
    //     for(x1=0;x1<32;x1++){
    //         led_canvas_set_pixel(offscreen_canvas, x1, y1, 255, 255, 255);
    //         offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
    //     }
    // }
    /*led_canvas_set_pixel(offscreen_canvas, 0, 0, 255, 255, 255);
    led_canvas_set_pixel(offscreen_canvas, 31, 63, 255, 0, 0);
    offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
    */

    int sizey = 63;
    int sizex = 32;
    int player[2]={sizey - 1,sizex / 2};
    led_canvas_set_pixel(offscreen_canvas, sizex/2, sizex/1, 255, 255, 255);
    led_canvas_set_pixel(offscreen_canvas, sizex/2+1, sizey/1+1, 255, 0, 0);
    offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);








    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);
    sleep(10);
    /*
   * Make sure to always call led_matrix_delete() in the end to reset the
   * display. Installing signal handlers for defined exit is a good idea.
   */
    led_matrix_delete(matrix);

    return 0;
}
