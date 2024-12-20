#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "LifeState.h"
#include "bmp.h"
#include "gif.h"

#include "ADTVector.h"
#include "ADTList.h"
#include "ADTMap.h"

int main(int argc, char *argv[]) {
        
        LifeState state;
        if(argc!=11){
                fprintf(stderr,"Σας παρακαλω, δωστε εγκυρα στοιχεια εισοδου\n");
                return ERROR;
        }        
        state = life_create_from_rle(argv[1]);
        // <state> <top> <left> <bottom> <right> <frames> <zoom> <speed> <delay> <gif>  //pros diagrafi

        //Top, Left, Bottom and Right for gif
        int top = atoi( argv[2]);
        int left = atoi( argv[3]);
        int bottom = atoi( argv[4]);
        int right = atoi( argv[5]);

        //Frames and speed
        unsigned int frames = atoi( argv[6]);
        unsigned int speed = atoi( argv[8]);

        //Zoom for Cells
        float zoom = atof( argv[7]);
        float cellsize=0;
        if(zoom > 1){
                cellsize = zoom;
        }

        //The time for every frame
        int delay = atoi( argv[9]);
        
        //Steps after from initialize .rle
        unsigned int steps;
        steps = frames*speed; 
        
        ListNode loop = NULL;
        LifeState state_new = create_state( state);

        List list_state = life_evolve_many( state, steps, &loop, argv[1]);
        ListNode node = list_first( list_state);


        //Create size and size+m for GIF and Bitmap
        int size;
        int size_m;

        if(bottom > top){
                size = bottom - top;
        }
        else{
                size = top - bottom;
        }
        if(right > left){
                size_m = right - left;
        }
        else{
                size_m = left - right;
        }
        int i_dis = abs(top-bottom)/4;
        int j_dis = abs(right-left)/4;

        // Create Gif and Bitmap
	GIF* gif = gif_create( size*zoom, size_m*zoom);
	Bitmap* bitmap = bm_create( size*zoom, size_m*zoom);

	// Default delay between frames
	gif->default_delay = delay;

        LifeCell cell;
        
        int x = 0;
        int y = 0;

        int developments;
        int counter;
        counter = 1;
        developments = 1;
        int k = 1;

	// We create an animation with a "cell" that moves from the top-right corner
        // to the bottom-left corner. The cell moves one pixel at a time, so the animation
        // will have as many frames as the dimensions of the image.
        while( developments <= steps) {
                // In the start, set black for all cells
		if(k == 1){
		        bm_set_color(bitmap, bm_atoi("black"));
		        bm_clear(bitmap);
                }

        	// Now, set white for living cells
	        bm_set_color(bitmap, bm_atoi("white"));
                k = 0;
                for(MapNode node_map = map_first( state_new->map);
                     node_map != MAP_EOF; 
                     node_map = map_next( state_new->map, node_map))
                {       
                        cell = map_node_key( state_new->map, node_map);
                        
                        if(top<0){
                                y = cell->x*zoom - top; 
                        }
                        else{
                                y=cell->x*zoom;
                        }
                        if(left<0){
                                x = cell->y*zoom - left; 
                        }
                        else {
                                x= cell->y*zoom;
                        }
                        if( left <= x && x <= right && top <= y && y<= bottom ){
                                if(top>0)y = y - top;
                                if(left>0)x = x - left;
                                k = 1;
                                x = x+ i_dis;
                                y = y + j_dis;
                                bm_fillrect(bitmap, x, y, x + cellsize , y + cellsize);
                        }
                }
                
                counter = counter + speed;
                do{             
                        if(loop == NULL){
                                LifeState state_new_1 =  life_evolve( state_new);
                                // life_destroy(state_new);
                                state_new = state_new_1;
                        }
                        else{
                                state_new = list_node_value( list_state, node);
                                node = list_next( list_state, node);
                                if(node == LIST_EOF){
                                        node = loop;
                                }
                        }

                        developments++;
                
                }while( ( developments < counter) && (developments<=steps));
                
	       // In the end, we add the bitmap as frame at GIF
               if(k == 1)gif_add_frame(gif, bitmap);
               else if(developments>steps)gif_add_frame(gif,bitmap);
        }

	// save in gif file
	gif_save(gif, argv[10]);

	// free memory
	bm_free(bitmap);
	gif_free(gif);
}
