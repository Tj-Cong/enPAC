//
// Created by lewis on 19-7-19.
//

#ifndef ENPAC_2020_3_0_PETRI_NET_H
#define ENPAC_2020_3_0_PETRI_NET_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "tinyxml.h"
#include <cmath>
#include <google/tcmalloc.h>

using namespace std;

#define MultiFactor 1.3
#define INDEX_ERROR 0xffffffff

typedef unsigned int index_t;
typedef unsigned short weight_t;
typedef unsigned short initM_t;
typedef unsigned int NUM_t;


/****************Global Functions**************/
unsigned int BKDRHash(string str);
unsigned int stringToNum(const string& str);
int my_atoi(string str);
/**********************************************/

struct HashElement{
    string id = "";
    bool place = true;
    index_t position;
};

struct Unit{
    string uid = "";
    NUM_t size;     //该unit中元素的个数;
    NUM_t mark_length;
    index_t startpos;
    index_t mark_sp;
};
typedef struct Small_Arc
{
    weight_t weight;
    index_t idx;
} SArc;

typedef struct Place
{
    string id = "";
    vector<SArc> producer;
    vector<SArc> consumer;
    initM_t initialMarking = 0;
    index_t myunit;
    index_t myoffset;
} *Place_P;

typedef struct Transition
{
    string id = "";
    vector<SArc> producer;
    vector<SArc> consumer;
} *Transition_P;

typedef struct Arc
{
    string id = "";
    string source_id;
    string target_id;
    index_t weight=1;
} *Arc_P;

typedef struct Nodes_Count
{
    NUM_t placesum = 0;       //the count of places
    NUM_t transitionsum = 0;  //the count of transitions
    NUM_t arcsum = 0;         //the count of arcs
}PT_Size;

class Petri
{
public:
    HashElement *Directory;
    Place *place;
    Transition *transition;
    Arc *arc;
    Unit *unittable;
    NUM_t placecount;
    NUM_t transitioncount;
    NUM_t arccount;
    NUM_t unitcount;
    NUM_t Dicsize;
    PT_Size size;
    NUM_t hash_conflict_times;
    bool NUPN;
public:
    Petri();
    void getSize(char *filename);
    void preNUPN(TiXmlElement *structure);
    void readNUPN(char *filename);
    NUM_t getPlaceSize() const;
    void allocHashTable();
    index_t arrange(string id, bool isPlace);
    index_t getPPosition(string str);
    index_t getTPosition(string str);
    index_t getPosition(string str, bool &isplace);
    void readPNML(char *filename);
    void computeUnitMarkLen();
    void printPlace();
    void printTransition();
    void printGraph();
    void printUnit();
    ~Petri();
};
#endif //ENPAC_2020_3_0_PETRI_NET_H
