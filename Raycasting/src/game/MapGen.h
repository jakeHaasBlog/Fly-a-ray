#pragma once
#include <iostream>
#include "string"
#include <vector>
#include "engine/SeeableEntity.h"
#include "engine/SeeableLine.h"



namespace maping {

    extern float blockSize;
    extern int rowNum;
    extern int colNum;

    extern struct vector2D;
    extern struct block;

    extern std::vector<block> blocks;
    extern std::vector<SeeableEntity*>* wallsPtr;

    extern int map[];


    void init(std::vector<SeeableEntity*>* in);
    void setWalls();

}