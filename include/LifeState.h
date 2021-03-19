#ifndef LIFESTATE_H
#define LIFESTATE_H

#include "ADTList.h"
#include "ADTMap.h"

#define ERROR -1

struct lifecell{
	int x, y;
};

typedef struct lifecell* LifeCell;

typedef struct lifestate* LifeState;

struct lifestate
{
    Map map;
    LifeCell mincell;
    LifeCell maxcell;
};

LifeState create_state( LifeState);
void life_destroy(LifeState);

LifeState life_create();

LifeState life_create_from_rle(char* file);

void pri(LifeState state);

void life_save_to_rle(LifeState state, char* file);

bool life_get_cell(LifeState state, LifeCell cell);

void life_set_cell(LifeState state, LifeCell cell, bool value);

LifeState life_evolve(LifeState state);

List life_evolve_many(LifeState state, int steps, ListNode* loop, char* file);

#endif