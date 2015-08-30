#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include "ships.h"

#define CONST (7)

struct elt {
    struct elt *next;
    struct position pos;
    struct ship *vessel;
};

struct field {
    size_t size;
    size_t n; 
    struct elt **head;
};

/* make a new ocean */
static struct field *fieldCreateInternal(size_t initsize){
    struct field *ocean;
    ocean = (struct field *) malloc(sizeof(struct field));
    assert(ocean);
    ocean->size = initsize;
    ocean->n = 0;
    ocean->head = (struct elt **) malloc(sizeof(struct elt *) * ocean->size);
    for (size_t i = 0; i<ocean->size; i++){
        ocean->head[i] = 0;
    }
    //printf("created ocean of size %zu\n", initsize);
    return ocean;
}

/*
 * Create a playing field for holding ships.
 */
struct field *fieldCreate(void){
    return fieldCreateInternal(CONST);
}

/*
 * Free all space associated with a field.
 */
void fieldDestroy(struct field *ocean){
    struct elt *freeMe;
    for(size_t h = 0; h < ocean->size; h++) {
        while(ocean->head[h]) {
            //printf("freeing %u, %u, %c\n", ocean->head[h]->pos.x, ocean->head[h]->pos.y, ocean->head[h]->vessel->name);
            freeMe = ocean->head[h];
            ocean->head[h] = freeMe->next;
            free(freeMe->vessel);
            free(freeMe);
        }
    }
    free(ocean->head);
    free(ocean);
}

/* compute random-looking hash of position */
static size_t hash(size_t range, struct position pos)
{
    return (97 * pos.x * pos.y) % range;
}

/* internal expansion of ocean */
static void expand(struct field *ocean) {
    struct field *o2;
    struct field swap;  /* holds contents of ocean temporarily for swap */
    struct elt *e;
    o2 = fieldCreateInternal(ocean->size * 2);
    /* move everything from ocean to o2 */
    for(size_t h = 0; h < ocean->size; h++) {
        for(e = ocean->head[h]; e != 0; e = e->next) {
            fieldPlaceShip(o2, *e->vessel);
        }
    }
    swap = *ocean;
    *ocean = *o2;   /* put new hash table in old struct */
    *o2 = swap;
    fieldDestroy(o2);
    //printf("expanded from %zu to %zu", ocean->size/2, ocean->size);
}

/* test membership, return the ship if a ship exists at pos */
struct ship *fieldMember(const struct field *f, struct position p)
{
    struct elt *e;
    struct ship *vessel = 0;
    for(e = f->head[hash(f->size, p)]; e != 0; e = e->next) {
        //printf("searching for %u %u, looking at %u %u %c\n", p.x, p.y, e->pos.x, e->pos.y, e->vessel->name);
        if(e->pos.x == p.x && e->pos.y == p.y) {
            vessel = e->vessel;
            //printf("found! %u %u %c\n", p.x, p.y, vessel->name);
            break;
        }
    }
    if(vessel == 0){
        //printf("no ship at %u %u\n", p.x, p.y);
    }
    return vessel;
}

/* removes entire ship given coordinates, which may not be the topLeft of the vessel */
void fieldRemove (struct field *f, struct position pos) {
    struct elt **e;
    struct elt *freeMe;
    struct position p;
    int w = 1;
    int dir;
    int len;
    struct ship *vessel = fieldMember(f, pos);
    if (vessel == 0){
        return;
    }
    /*
     * Sneaky trick: instead of keeping a pointer to an elt
     * as we walk through the list, we keep a pointer to the
     * location in the list that points to it.  That way, when
     * we decide to patch out *e, we have the location (e) that
     * we need to change to point to (*e)->next.
     */
    dir = vessel->direction;
    len = vessel->length;
    p.x = vessel->topLeft.x;
    p.y = vessel->topLeft.y;
    while (w <= len){
        for(e = &(f->head[hash(f->size, p)]); *e != 0; e = &((*e)->next)) {
            if((*e)->pos.x == p.x && (*e)->pos.y == p.y) {
                /* got it */
                //printf("then: %u %u %c\n", (*e)->pos.x, (*e)->pos.y, (*e)->vessel->name);
                freeMe = *e;
                *e = freeMe->next;
                free(freeMe->vessel);
                free(freeMe);
                if (*e != 0) {
                    //printf("now: %u %u %c\n", (*e)->pos.x, (*e)->pos.y, (*e)->vessel->name);
                }
                else{
                    //printf("now: null\n");
                }
                break;
            }
        }
        if(dir == HORIZONTAL){
            p.x++;
        }
        else{
            p.y++;
        }
        w = 1 + w;
        //printf("%u %u\n", w, len);
    }
    f->n--;
}

/* places an element in field given ship and pos*/
void fieldPlaceShipInternal(struct field *f, struct ship s, struct position temppos){
    struct elt *e;
    size_t h;
    e = malloc(sizeof(struct elt));
    assert(e);
    h = hash(f->size, temppos);
    e->pos = temppos;
    e->vessel = malloc(sizeof(struct ship));
    assert(e->vessel);
    memcpy(e->vessel, &s, sizeof(struct ship));
    e->next = f->head[h];
    f->head[h] = e;
    //printf("fieldPlaceShipInternal: %u %u %c\n", e->pos.x, e->pos.y, e->vessel->name);
}

/*
 * Place a ship in a field with given placement and name.
 *
 * If placement.length is less than one or greater than MAX_SHIP_LENGTH, 
 * or if some part of the ship would have a coordinate greater than COORD_MAX, 
 * or if the ship's name is NO_SHIP_NAME,
 * the function returns without placing a ship.
 *
 * Placing a new ship that intersects any previously-placed ships
 * sinks the previous ships, removing them from the field.
 */
void fieldPlaceShip(struct field *f, struct ship s){
    unsigned int w=1;
    struct position temppos;
    temppos.x = s.topLeft.x;
    temppos.y = s.topLeft.y;
    if (s.length<1 || s.length>MAX_SHIP_LENGTH || s.name == NO_SHIP_NAME){
        return;
    }
    else if ((s.direction == HORIZONTAL && ((s.topLeft.x - 1 + s.length)>COORD_MAX || (s.topLeft.x - 1 + s.length)<s.topLeft.x)) || (s.direction == VERTICAL && ((s.topLeft.y - 1 + s.length)>COORD_MAX || (s.topLeft.y - 1 + s.length)<s.topLeft.y))) {
        return;
    }
    else {
        //printf("can you not?");
        if(f->n >= f->size) {
            /* make the hash table bigger */
            expand(f);
        }
        while (w <= s.length) {
            //printf("removal error?");
            fieldRemove(f, temppos);
            fieldPlaceShipInternal(f, s, temppos);
            if(s.direction == HORIZONTAL){
                temppos.x++;
            }
            else{
                temppos.y++;
            }
            w++;
        }
        f->n++;
        return;
    }
}

/*
 * Attack!
 *
 * Drop a shell at given position.
 *
 * Returns NO_SHIP_NAME if attack misses (does not intersect any ship).
 *
 * Otherwise returns name of ship hit. 
 *
 * Hitting a ship sinks it, removing it from the field.
 */
char fieldAttack(struct field *f, struct position p){
    char shipName = NO_SHIP_NAME;
    struct ship *vessel = fieldMember(f, p);
    //printf("field attack! %u %u %c\n", p.x, p.y, vessel->name);
    if (vessel != 0) {
        shipName = vessel->name;
        fieldRemove(f, p);
    }
    return shipName;
}

/*
 * Return number of ships in the field.
 */
size_t fieldCountShips(const struct field *f){
    return f->n;
}