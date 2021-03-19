#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "LifeState.h"
#include "ADTMap.h"

#include "bmp.h"
#include "gif.h"


//With Create Fuction, I reserve space

//create int
int *create_int(int value)
{
    int *p = malloc(sizeof(int));
    *p = value;
    return p;
}

//create char
char *create_char(char value)
{
    char *p = malloc(sizeof(char));
    *p = value;
    return p;
}

//create lifecell
LifeCell create_lifecell(LifeCell cell)
{
    LifeCell cell_1 = malloc(sizeof(struct lifecell));
    cell_1->x = cell->x;
    cell_1->y = cell->y;
    return cell_1;
}

//create state
LifeState create_state(LifeState state)
{
    LifeState state_new = life_create();
    char life = 'o';
    LifeCell cell = malloc(sizeof(struct lifecell));
    for (MapNode node = map_first(state->map);
         node != MAP_EOF;
         node = map_next(state->map, node))
    {
        cell = map_node_key(state->map, node);
        map_insert(state_new->map, create_lifecell(cell), create_char(life));
    }
    state_new->mincell->x = state->mincell->x;
    state_new->mincell->y = state->mincell->y;
    state_new->maxcell->x = state->maxcell->x;
    state_new->maxcell->y = state->maxcell->y;
    return state_new;
}

//compare fuction
int compare_state(Pointer a, Pointer b)
{
    LifeState state_1 = a;
    LifeState state_2 = b;
    int size_1 = map_size(state_1->map);
    int size_2 = map_size(state_2->map);
    
    if (size_1 != size_2)
    {
        return size_1 - size_2;
    }

    int size = size_1;
    
    MapNode node_1 = map_first(state_1->map);
    MapNode node_2 = map_first(state_2->map);
    
    LifeCell cell_1 = malloc(sizeof(struct lifecell));
    LifeCell cell_2 = malloc(sizeof(struct lifecell));
    
    for (int i = 0; i < size; ++i)
    {
        cell_1 = map_node_key(state_1->map, node_1);
        cell_2 = map_node_key(state_2->map, node_2);
        if (cell_1->x != cell_2->x)
        {
            return cell_1->x - cell_2->x;
        }
        if (cell_1->y != cell_2->y)
        {
            return cell_1->y - cell_2->y;
        }
        node_1 = map_next(state_1->map, node_1);
        node_2 = map_next(state_2->map, node_2);
    }
    
    return 0;
}

//compare int
int compare_int(Pointer a, Pointer b)
{
    return *(int *)a - *(int *)b;
}

//compare lifecell
int compare_lifecell(Pointer a, Pointer b)
{
    LifeCell c1 = a;
    LifeCell c2 = b;
    if (c1->x != c2->x)
    {
        return c1->x - c2->x;
    }
    if ((c1->y) != (c2->y))
    {
        return c1->y - c2->y;
    }
    return 0;
}

//life create
LifeState life_create()
{
    LifeState state;
    state = malloc(sizeof(struct lifestate));
    state->map = map_create(compare_lifecell, free, free);
    state->mincell = malloc(sizeof(struct lifecell));
    state->maxcell = malloc(sizeof(struct lifecell));
    return state;
}

//life create from file, where give for input
//save the cell in life, in a map
LifeState life_create_from_rle(char *file)
{
    LifeState state = life_create();
    if (strcmp(file, "-") == 0)
    {
        exit(0);
    }

    FILE *filename = fopen(file, "r");
    
    char character;
    character = fgetc(filename);
    
    int m = 0;
    int i = 0;
    int j = 0;

    LifeCell cell = malloc(sizeof(struct lifecell));
    
    cell->x = 0;
    cell->y = 0;
    state->mincell = create_lifecell(cell);
    state->maxcell = cell;

    //reade the file
    //i save the file in a map, where the map have the cell where is 'o'
    char life = 'o';
    while (character != EOF)
    {
        if (character == 'b')
        {
            if (m)
            {
                for (int k = 0; k < m; ++k)
                {
                    j++;
                }
            }
            else
            {
                j++;
            }
            m = 0;
            character = fgetc(filename);
            continue;
        }
        if (character == 'o')
        {
            if (m)
            {
                cell->x = i;
                for (int f = 0; f < m; ++f)
                {
                    cell->y = j;
                    map_insert(state->map, create_lifecell(cell), create_char(life));
                    j++;
                }
            }
            else
            {
                cell->x = i;
                cell->y = j;
                map_insert(state->map, create_lifecell(cell), create_char(life));
                j++;
            }
            m = 0;
        }
        else if (character == '$')
        {

            i++;
            j = 0;
        }
        else if (character == '!')
        {
            cell->x = i;
            cell->y = j - 1;
            state->maxcell = create_lifecell(cell);
            break;
        }
        else
        {
            char *f = malloc(sizeof(char));
            sprintf(f, "%c", character);
            int k = atoi(f);
            if (m)
            {
                m = m * 10;
                m = m + k;
            }
            else
            {
                m = k;
            }
        }
        character = fgetc(filename);
    }

    fclose(filename);
    return state;
}

void life_save_to_rle(LifeState state, char *file)
{
    FILE *filename = fopen(file, "w");
    MapNode node;
    LifeCell cell;


    //i have save from input-file from the "table" max and min cell
    int min_j = state->mincell->y;
    int min_i = state->mincell->x;
    int max_j = state->maxcell->y;
    
    //dead and life cell
    char ch_dead = 'b';
    char ch_life = 'o';
    
    //previous i and j
    int previous_j = min_j;
    int previous_i = min_i;
    
    //numbers, i ,j
    int numbers = 0;
    int i;
    int j;
    int life = 0;

    int add = 0;
    int first_previous_j = min_j;
    
    //one for-loop with the life cell
    for (node = map_first(state->map); node != MAP_EOF; node = map_next(state->map, node))
    {
        cell = map_node_key(state->map, node);
        
        i = cell->x;
        j = cell->y;
        if (i > previous_i)
        {
            //i print in file the cell
            //i print not only cell in life, but i print if must, dead cell
            if (life == 1)
            {
                fprintf(filename, "%c", ch_life);
            }
            else if (life > 1)
            {   
                //if the cell are many, i print with number
                fprintf(filename, "%d%c", life, ch_life);
            }

            life = 0;
            if (max_j > previous_j)
            {
                numbers = max_j - previous_j;
            }
            else
            {
                numbers = previous_j - max_j;
            }

            if (numbers > 1)
            {   
                //i print dead cell from previous line
                fprintf(filename, "%d%c", numbers, ch_dead); 
            }                                           
            else if (numbers == 1)
            {
                fprintf(filename, "%c", ch_dead);
            }
            //the "$" is the change line in rle
            fprintf(filename, "$\n");

            //save the previous i and j
            previous_j = min_j;
            previous_i = i;
            first_previous_j = min_j;
        }

        //i print the life cell
        if (j > previous_j)
        {
            add = j - previous_j;
        }
        else
        {
            add = previous_j - j;
        }
        if (add == 0)
        {
            life++;
        }
        else if (add == 1)
        {
            life++;
        }
        else
        {
            if (life == 1)
            {
                fprintf(filename, "%c", ch_life);
            }
            else if (life > 1)
            {   
                //if the cell are many, i print with number
                fprintf(filename, "%d%c", life, ch_life);
            }
            life = 0;
            if (add > 1)
            {
                life++;
            }
        }

        //i print dead cell.
        if (j > previous_j)
        {
            numbers = j - previous_j;
        }
        else
        {
            numbers = previous_j - j;
        }
        if (first_previous_j == min_j && j != previous_j)
        {
            if (numbers > 1)
            {   
                //i print dead cell from previous line
                fprintf(filename, "%d%c", numbers, ch_dead);
            }                                              
            else if (numbers == 1)
            {
                fprintf(filename, "%c", ch_dead);
            }
        }
        else if (numbers > 0)
        {
            numbers--;
            if (numbers > 1)
            {   
                //i print dead cell from previous line
                fprintf(filename, "%d%c", numbers, ch_dead);
            }                    
            else if (numbers == 1)
            {
                fprintf(filename, "%c", ch_dead);
            }
        }

        //previous
        first_previous_j = j + 1;
        previous_j = j;
    }

    if (life == 1)
    {
        fprintf(filename, "%c", ch_life);
    }
    else if (life > 1)
    {
        fprintf(filename, "%d%c", life, ch_life);
    }

    //i print the remaining dead cells if any.
    if (max_j > previous_j)
    {
        numbers = max_j - previous_j;
    }
    else
    {
        numbers = previous_j - max_j;
    }
    if (numbers > 1)
    {
        fprintf(filename, "%d%c", numbers, ch_dead);
    }
    else if (numbers == 1)
    {
        fprintf(filename, "%c", ch_dead);
    }
    //i print "!", where for rle-file is the end file.
    fprintf(filename, "!");
    fclose(filename);
}

//get cell
bool life_get_cell(LifeState state, LifeCell cell)
{   
    //i check if the cell is in the map( where mains cell in life), otherwise is dead 
    if (map_find_node(state->map, cell) != MAP_EOF)
    { 
        return true;
    }
    return false;
}

//set cell
//if value = true i change the cell from dead->life, otherwise this cell is dead
void life_set_cell(LifeState state, LifeCell cell, bool value)
{   
    if (life_get_cell(state, cell) == 1)
    {
        if (value == false)
        {
            map_remove(state->map, cell);
        }
    }
    else
    {
        if (value == true)
        {
            map_insert(state->map, create_lifecell(cell), "life");
        }
    }
}

LifeState life_evolve(LifeState state)
{
    LifeState state_1;
    state_1 = life_create();       //δημιουργω ενα καινουριο state οπου στο map του θα αποθηκευω ποσους ζωντανους γειτονες εχει ο καθε
    int min_i = state->mincell->x; //γειτονας του
    int min_j = state->mincell->y; //το key του θα χει cell και το value ποσους ζωντανους γειτονες εχει
    int max_i = state->maxcell->x;
    int max_j = state->maxcell->y;
    int neighbours;
    neighbours = 0;
    LifeCell cell_n;
    cell_n = malloc(sizeof(struct lifecell));
    for (int i = min_i - 1; i <= max_i + 1; ++i)
    {
        for (int j = min_j - 1; j <= max_j + 1; ++j)
        {
            cell_n->x = i;
            cell_n->y = j;
            map_insert(state_1->map, create_lifecell(cell_n), create_int(neighbours));
        }
    }
    LifeCell cell;
    cell = malloc(sizeof(struct lifecell));
    // printf("\n\n %d \n\n", map_size(state->map));
    for (MapNode node = map_first(state->map);
         node != MAP_EOF;
         node = map_next(state->map, node))
    {
        //αθροιζω το neighbous στο δεξι και αριστερο κελι( του state_1) απο το κελι που βρισκομαι στο state
        cell = map_node_key(state->map, node);

        cell_n->x = cell->x;
        cell_n->y = cell->y - 1;
        neighbours = *(int *)map_find(state_1->map, cell_n);
        neighbours++;
        map_insert(state_1->map, create_lifecell(cell_n), create_int(neighbours));

        cell_n->x = cell->x;
        cell_n->y = cell->y + 1;
        neighbours = *(int *)map_find(state_1->map, cell_n);
        neighbours++;
        map_insert(state_1->map, create_lifecell(cell_n), create_int(neighbours));

        //αθροιζω το neighbours tou state_1 στα 3 πανω κελια( του state_1) απο το κελι που βρισκομαι στο state
        cell_n->x = cell->x - 1;
        cell_n->y = cell->y - 1;
        neighbours = *(int *)map_find(state_1->map, cell_n);
        neighbours++;
        ;
        map_insert(state_1->map, create_lifecell(cell_n), create_int(neighbours));

        cell_n->x = cell->x - 1;
        cell_n->y = cell->y + 1;
        neighbours = *(int *)map_find(state_1->map, cell_n);
        neighbours++;
        map_insert(state_1->map, create_lifecell(cell_n), create_int(neighbours));

        cell_n->x = cell->x - 1;
        cell_n->y = cell->y;
        neighbours = *(int *)map_find(state_1->map, cell_n);
        neighbours++;
        map_insert(state_1->map, create_lifecell(cell_n), create_int(neighbours));

        //αθροιζω το neighbours tou state_1 στα 3 κατω κελια( του state_1) απο το κελι που βρισκομαι στο state
        cell_n->x = cell->x + 1;
        cell_n->y = cell->y - 1;
        neighbours = *(int *)map_find(state_1->map, cell_n);
        neighbours++;
        map_insert(state_1->map, create_lifecell(cell_n), create_int(neighbours));

        cell_n->x = cell->x + 1;
        cell_n->y = cell->y + 1;
        neighbours = *(int *)map_find(state_1->map, cell_n);
        neighbours++;
        map_insert(state_1->map, create_lifecell(cell_n), create_int(neighbours));

        cell_n->x = cell->x + 1;
        cell_n->y = cell->y;
        neighbours = *(int *)map_find(state_1->map, cell_n);
        neighbours++;
        map_insert(state_1->map, create_lifecell(cell_n), create_int(neighbours));
    }

    //θα φτιαξω νεο map και θα διαγραψω το παλιο map
    LifeState state_new;
    state_new = life_create();
    char life = 'o';
    state_new->mincell->x = max_i + 1;
    state_new->mincell->y = max_j + 1;
    state_new->maxcell->x = min_i - 1;
    state_new->maxcell->y = min_j - 1;
    LifeCell cell_new = malloc(sizeof(struct lifecell));
    for (int i = min_i - 1; i <= max_i + 1; ++i)
    {
        for (int j = min_j - 1; j <= max_j + 1; ++j)
        {
            cell_new->x = i;
            cell_new->y = j;
            neighbours = *(int *)map_find(state_1->map, cell_new);
            if ((neighbours == 3 || neighbours == 2) && (map_find(state->map, cell_new) != NULL))
            {
                if (cell_new->x < state_new->mincell->x)
                {
                    state_new->mincell->x = cell_new->x;
                }
                if (cell_new->x > state_new->maxcell->x)
                {
                    state_new->maxcell->x = cell_new->x;
                }
                if (cell_new->y < state_new->mincell->y)
                {
                    state_new->mincell->y = cell_new->y;
                }
                if (cell_new->y > state_new->maxcell->y)
                {
                    state_new->maxcell->y = cell_new->y;
                }
                map_insert(state_new->map, create_lifecell(cell_new), create_char(life));
            }
            else if (neighbours == 3 && (map_find(state->map, cell_new) == NULL))
            {
                if (cell_new->x < state_new->mincell->x)
                {
                    state_new->mincell->x = cell_new->x;
                }
                if (cell_new->x > state_new->maxcell->x)
                {
                    state_new->maxcell->x = cell_new->x;
                }
                if (cell_new->y < state_new->mincell->y)
                {
                    state_new->mincell->y = cell_new->y;
                }
                if (cell_new->y > state_new->maxcell->y)
                {
                    state_new->maxcell->y = cell_new->y;
                }
                //  printf("%d kxcddedeai %d\n",cell->x,cell->y);
                map_insert(state_new->map, create_lifecell(cell_new), create_char(life));
            }
        }
    }

    // printf("i=%d j= %d kaiiii ima=%d jma=%d\n",state_new->mincell->x,state_new->mincell->y,state_new->maxcell->x,state_new->maxcell->y);
    life_destroy(state);
    life_destroy(state_1);
    free(cell_n);
    //free(cell);
    //free(cell_new);
    return state_new;
}

void life_destroy(LifeState state)
{
    map_destroy(state->map);
    free(state->mincell);
    free(state->maxcell);
    free(state);
}

// Επιστρέφει μία λίστα από το πολύ steps εξελίξεις, ξεκινώνας από την κατάσταση
// state. Αν βρεθεί επανάληψη τότε στο *loop αποθηκεύεται ο κόμβος στον οποίο
// συνεχίζει η εξέλιξη μετά τον τελευταίο κόμβο της λίστας, διαφορετικά NULL
List life_evolve_many(LifeState state, int steps, ListNode *loop, char *file)
{
    if (steps == 0)
    {
        *loop = NULL;
        return NULL;
    }
    Map map = map_create(compare_state, free, free);
    int k = 1;
    List list = list_create(free);
    ListNode node;
    node = LIST_BOF;
    list_insert_next(list, node, create_state(state));
    node = list_first(list);
    MapNode node_map;
    map_insert(map, create_state(state), create_int(k));
    for (int i = 0; i < steps; ++i)
    {

        //printf(" στin evolve gia i = %d \n",i);
        state = life_evolve(state);
        if ((node_map = map_find_node(map, state)) != MAP_EOF)
        {
            printf("brethike me i = %d kai map == %d\n", i, map_size(map)); //na diagrafthei
            *loop = list_find_node(list, state, compare_state);
            map_destroy(map);
            return list;
        }
        k++;
        map_insert(map, create_state(state), create_int(k));
        list_insert_next(list, node, create_state(state));
        node = list_next(list, node);
    }
    //printf("aionio\n");                                           //na dagrafthei
    map_destroy(map);
    return list;
}