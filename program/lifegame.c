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

        //οι διαστασεις του gif
        int top = atoi( argv[2]);
        int left = atoi( argv[3]);
        int bottom = atoi( argv[4]);
        int right = atoi( argv[5]);

        //τα πλαισια και αλματα αναμεσα σε εξελιξεις
        unsigned int frames = atoi( argv[6]);
        unsigned int speed = atoi( argv[8]);

        //το ζουμ για τα κελια
        float zoom = atof( argv[7]);
        float cellsize=0;
        if(zoom > 1){
                cellsize = zoom;
        }

        //ο χρόνος που διαρκεί κάθε frame (msecs).
        int delay = atoi( argv[9]);
        
        //οι εξελιξεις που θα γινουν ειναι steps
        unsigned int steps;
        steps = frames*speed; 
        
        ListNode loop = NULL;
        LifeState state_new = create_state( state);

        List list_state = life_evolve_many( state, steps, &loop, argv[1]);
        ListNode node = list_first( list_state);


        //ελενχω ωστε να φτιαξω το size kai size_m για τισ διαστασεις του GIF και Bitmap
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

        // Δημιουργία ενός GIF και ενός bitmap στη μνήμη
	GIF* gif = gif_create( size*zoom, size_m*zoom);
	Bitmap* bitmap = bm_create( size*zoom, size_m*zoom);

	// Default καθυστέρηση μεταξύ των frames, σε εκατοστά του δευτερολέπτου
	gif->default_delay = delay;

        LifeCell cell;
        
        int x = 0;
        int y = 0;

        int developments;
        int counter;
        counter = 1;
        developments = 1;
        int k = 1;

	// Δημιουργούμε ενα animation με ένα "cell" το οποίο μετακινείται από τη δεξιά-πάνω
	// γωνία προς την κάτω-αριστερά. Το cell μετακινείται ένα pixel τη φορά, οπότε το animation
	// θα έχει τόσα frames όσα το μέθεθος της εικόνας.
        while( developments <= steps) {
		if(k == 1){
                        // Σε κάθε frame, πρώτα μαυρίζουμε ολόκληρο το bitmap
		        bm_set_color(bitmap, bm_atoi("black"));
		        bm_clear(bitmap);
                }
        	// Και μετά ζωγραφίζουμε ασπρα τα τετραγωνα οπου ειναι ζωντανα       
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
                        //bm_fillrect(bitmap, x, y,x+1 , y+1);
                        // bm_putpixel( bitmap, x,y);
                }
                
                counter = counter + speed;
                do{             
                        if(loop == NULL){
                                state_new =  life_evolve( state_new);
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
                
                // Τέλος προσθέτουμε το bitmap σαν frame στο GIF (τα περιεχόμενα αντιγράφονται)
	       if(k == 1)gif_add_frame(gif, bitmap);
               else if(developments>steps)gif_add_frame(gif,bitmap);
        }

	// save in gif file
	gif_save(gif, argv[10]);

	// free memory
	bm_free(bitmap);
	gif_free(gif);
        //free(gif_for_save);
}
