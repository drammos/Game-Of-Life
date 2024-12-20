#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#include "LifeState.h"
#include "ADTMap.h"
#include <stdint.h>
#include "bmp.h"
#include "gif.h"

//With Create Fuction, I reserve space

// Create int
int *create_int(int value)
{
    int *p = malloc(sizeof(int));
    *p = value;
    return p;
}

// Create char
char *create_char(char value)
{
    char *p = malloc(sizeof(char));
    *p = value;
    return p;
}

// Create lifecell
LifeCell create_lifecell(LifeCell cell)
{
    LifeCell cell_1 = malloc(sizeof(struct lifecell));
    cell_1->x = cell->x;
    cell_1->y = cell->y;
    return cell_1;
}

// Create state
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
    return state_new;
}

// Hashing for lifecell
uint32_t hash_lifecell(Pointer cell) {
    LifeCell c = (LifeCell)cell;

    uint32_t hash = 0;
    hash = c->x * 73856093u ^ c->y * 19349663u;

    return hash;
}

// Compare fuction
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

// Compare int
int compare_int(Pointer a, Pointer b)
{
    return *(int *)a - *(int *)b;
}

// Compare lifecell
int compare_lifecell(Pointer a, Pointer b)
{
    LifeCell c1 = (LifeCell)a;
    LifeCell c2 = (LifeCell)b;
    if (c1->x==c2->x) {
        return c1->y - c2->y;
    }

    return c1->x - c2->x;
}

// Life create
LifeState life_create()
{
    LifeState state;
    state = malloc(sizeof(struct lifestate));
    state->map = map_create(compare_lifecell, free, free);
    map_set_hash_function(state->map, hash_lifecell);
    
    return state;
}

// I create Life create from rle-file(input)
//save the life-cell, in a map
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

    while (character == '#') {
        char buf[1024];
        fgets(buf, 1024, filename);
        character = fgetc(filename);
    }

    if (character == 'x') {
        char buf[1024];
        fgets(buf, 1024, filename);
        character = fgetc(filename);
    }

    LifeCell cell = malloc(sizeof(struct lifecell));
    
    cell->x = 0;
    cell->y = 0;

    //reade the file
    //i save the file in a map, where the map have the cell where is 'o'
    char life = 'o';
    while (character != EOF)
    {   
        if(character == ' ' || character == '\n' || character == '\t') {
            character = fgetc(filename);
            continue;
        }
        if (character == 'b')
        {
            m = (m == 0) ? 1 : m;
            j += m;
            m = 0;
            
        }
        else if (character == 'o')
        {
            m = (m == 0) ? 1 : m;
            for (int f = 0; f < m; f++) {
                cell->x = i;
                cell->y = j;
                map_insert(state->map, create_lifecell(cell), create_char(life));
                j++;
            }
            m = 0;
        }
        else if (character == '$')
        {
            m = (m == 0) ? 1 : m;
            i += m;
            j = 0;
            m = 0;
        }
        else if (character == '!')
        {
            break;
        }
        else if (isdigit(character)) 
        {    
            m = m * 10 + (character - '0');
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
    int min_i = INT_MIN;
    int min_j = INT_MIN;
    int max_i = INT_MAX;
    int max_j = INT_MAX;

    for (MapNode node = map_first(state->map);
         node != MAP_EOF;
         node = map_next(state->map, node)) {

        LifeCell cell = map_node_key(state, node);
        if (cell->x < min_i) min_i = cell->x;
        if (cell->x > max_i) max_i = cell->x;
        if (cell->y < min_j) min_j = cell->y;
        if (cell->y > max_j) max_j = cell->y;
    }
    
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

// Get cell
bool life_get_cell(LifeState state, LifeCell cell)
{   
    //i check if the cell is in the map( where mains cell in life), otherwise is dead 
    if (map_find_node(state->map, cell) != MAP_EOF)
    { 
        return true;
    }
    return false;
}

// Set cell
// If value = true i change the cell from dead->life 
// Otherwise this cell is dead
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

void calculate_neighbours(LifeState state_neighbours, int x, int y)
{
    LifeCell cell = malloc(sizeof(struct lifecell));
    cell->x = x;
    cell->y = y;

    Pointer* pointer = map_find(state_neighbours->map, cell);
    int neighbours = 0;
    if(pointer!=NULL)
    {
        *pointer = *(int*)pointer + 1;
        // neighbours = neigh + 1;
    }
    // map_insert(state_neighbours->map, create_lifecell(cell), create_int(neighbours));

    free(cell);
}
// Life evolve
LifeState life_evolve(LifeState state)
{
    // I create a new state
    // The new map includes for Key the lifecells 
    // And the neighbours for Value
    LifeState state_neighbours;
    state_neighbours = life_create();
    int min_i = INT_MAX;
    int min_j = INT_MAX;
    int max_i = INT_MIN;
    int max_j = INT_MIN;

    for (MapNode node = map_first(state->map);
         node != MAP_EOF;
         node = map_next(state->map, node)) {

        LifeCell cell = map_node_key(state, node);
        if (cell->x < min_i) min_i = cell->x;
        if (cell->x > max_i) max_i = cell->x;
        if (cell->y < min_j) min_j = cell->y;
        if (cell->y > max_j) max_j = cell->y;
    }

    int neighbours;
    neighbours = 0;
    LifeCell cell_n;
    cell_n = malloc(sizeof(struct lifecell));
    for (int i = min_i - 1; i <= max_i + 1; i++)
    {
        for (int j = min_j - 1; j <= max_j + 1; j++)
        {
            cell_n->x = i;
            cell_n->y = j;
            map_insert(state_neighbours->map, create_lifecell(cell_n), create_int(neighbours));
        }
    }

    for (MapNode node = map_first(state->map);
         node != MAP_EOF;
         node = map_next(state->map, node))
    {
        LifeCell cell = map_node_key(state->map, node);

        //Add new neighbours for left&right cells (new_state) from the state
        calculate_neighbours(state_neighbours, cell->x, cell->y - 1);
        calculate_neighbours(state_neighbours, cell->x, cell->y + 1);

        //Add new neighbours for 3 up-stairs cells (new_state) from the state
        calculate_neighbours(state_neighbours, cell->x - 1, cell->y - 1);
        calculate_neighbours(state_neighbours, cell->x - 1, cell->y + 1);
        calculate_neighbours(state_neighbours, cell->x - 1, cell->y);

        //Add new neighbours for 3 down-stairs cells (new_state) from the state
        calculate_neighbours(state_neighbours, cell->x + 1, cell->y - 1);
        calculate_neighbours(state_neighbours, cell->x + 1, cell->y + 1);
        calculate_neighbours(state_neighbours, cell->x + 1, cell->y);
    }

    //Create a new map
    LifeState state_new;
    state_new = life_create();
    char life = 'o';
    LifeCell cell_new = malloc(sizeof(struct lifecell));
    for (int i = min_i - 1; i <= max_i + 1; i++)
    {
        for (int j = min_j - 1; j <= max_j + 1; j++)
        {
            cell_new->x = i;
            cell_new->y = j;
            Pointer* pointer = map_find(state_neighbours->map, cell_new);
            if(pointer==NULL) continue;
            neighbours = *(int*)pointer;

            if((neighbours<2 || neighbours>3) && (map_find(state->map, cell_new)!=NULL)) continue;
            else if (neighbours == 3 && (map_find(state->map, cell_new) == NULL))
            {
                map_insert(state_new->map, create_lifecell(cell_new), create_char(life));
            }
            else if(map_find(state->map, cell_new) != NULL)
            {
                map_insert(state_new->map, create_lifecell(cell_new), create_char(life));
            }
        }
    }

    life_destroy(state);
    life_destroy(state_neighbours);
    free(cell_n);
    return state_new;
}

// Life Destroy
void life_destroy(LifeState state)
{
    map_destroy(state->map);
    free(state);
}

// Returns a list of at most 'steps' evolutions, starting from the state 'state'.
// If a repetition is found, the *loop stores the node where the evolution continues 
// after the last node in the list, otherwise NULL.
List life_evolve_many(LifeState state, int steps, ListNode *loop, char *file)
{
    if (steps == 0)
    {
        *loop = NULL;
        return NULL;
    }
    Map map = map_create(compare_state, free, free);
    map_set_hash_function(map, hash_lifecell);
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
        state = life_evolve(state);
        if ((node_map = map_find_node(map, state)) != MAP_EOF)
        {
            *loop = list_find_node(list, state, compare_state);
            map_destroy(map);
            return list;
        }
        k++;
        map_insert(map, create_state(state), create_int(k));
        list_insert_next(list, node, create_state(state));
        node = list_next(list, node);
    }
    map_destroy(map);
    return list;
}