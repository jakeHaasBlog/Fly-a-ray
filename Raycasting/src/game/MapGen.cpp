#include "MapGen.h"


namespace maping {

    float blockSize = .3;
    int rowNum = 8;
    int colNum = 16;

    std::vector<block> blocks;
    std::vector<SeeableEntity*>* wallsPtr;

    int map[] =
    {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,1,0,0,1,1,0,1,1,0,0,0,0,0,
        0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,
        0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
        0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
        0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
        0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    };

    struct vector2D {
        float x = 0;
        float y = 0;
        vector2D() {

        }
        vector2D(float xIn, float yIn) {
            x = xIn;
            y = yIn;
        }
    };

    struct block {
        bool active = false;
        vector2D pos;
        //nsew
        bool edgeActive[4] = { false,false,false,false };
        int edgeIndex[4] = { NULL,NULL,NULL,NULL };

        block(float x, float y) {
            pos.x = x;
            pos.y = y;
        }

    };

    // util
    vector2D getCord(int index) {
        int x = index % colNum;
        int y = (index - x) / colNum;
        return vector2D(x, y);
    }

    int getIndex(int x, int y) {
        return x + (y * 16);
    }

    void init(std::vector<SeeableEntity*>* in) {

        wallsPtr = in;

        blocks.clear();
        blocks.reserve(rowNum*colNum);
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 16; x++) {
                blocks.push_back(block(x * blockSize, y * blockSize));
                if (map[getIndex(x, y)] == 1) {
                    blocks[getIndex(x, y)].active = true;
                }
            }
        }
    }

    void addWall(int blockIndex, int dir) {
        //nsew
        switch (dir) {
        case 0:
            wallsPtr->push_back(new SeeableLine(blocks[blockIndex].pos.x, blocks[blockIndex].pos.y, blocks[blockIndex].pos.x + blockSize, blocks[blockIndex].pos.y));

            break;
        case 1:
            wallsPtr->push_back(new SeeableLine(blocks[blockIndex].pos.x, blocks[blockIndex].pos.y, blocks[blockIndex].pos.x, blocks[blockIndex].pos.y + blockSize));

            break;
        case 2:
            wallsPtr->push_back(new SeeableLine(blocks[blockIndex].pos.x, blocks[blockIndex].pos.y + blockSize, blocks[blockIndex].pos.x + blockSize, blocks[blockIndex].pos.y + blockSize));

            break;
        case 3:
            wallsPtr->push_back(new SeeableLine(blocks[blockIndex].pos.x, blocks[blockIndex].pos.y, blocks[blockIndex].pos.x, blocks[blockIndex].pos.y + blockSize));

            break;
        default:
            std::cout << "invalad direction" << std::endl;
            return;
            break;
        }

        blocks[blockIndex].edgeActive[dir] = true;
        blocks[blockIndex].edgeIndex[dir] = wallsPtr->size() - 1;
    }


    void extendEdge(int blockIndex, int edgeType) {
        switch (edgeType) {
        case 0:
            wallsPtr[blocks[blockIndex - 1].edgeIndex[edgeType]].p[1].position.x += blockSize;
            blocks[blockIndex].edgeActive[edgeType] = true;
            blocks[blockIndex].edgeIndex[edgeType] = blocks[blockIndex - 1].edgeIndex[edgeType];
            break;
        case 1:
            wallsPtr[blocks[blockIndex - colNum].edgeIndex[edgeType]].p[1].position.y += blockSize;
            blocks[blockIndex].edgeActive[edgeType] = true;
            blocks[blockIndex].edgeIndex[edgeType] = blocks[blockIndex - colNum].edgeIndex[edgeType];
            break;
        case 2:
            wallsPtr[blocks[blockIndex - 1].edgeIndex[edgeType]].p[1].position.x += blockSize;
            blocks[blockIndex].edgeActive[edgeType] = true;
            blocks[blockIndex].edgeIndex[edgeType] = blocks[blockIndex - 1].edgeIndex[edgeType];
            break;
        case 3:
            wallsPtr[blocks[blockIndex - colNum].edgeIndex[edgeType]].p[1].position.y += blockSize;
            blocks[blockIndex].edgeActive[edgeType] = true;
            blocks[blockIndex].edgeIndex[edgeType] = blocks[blockIndex - colNum].edgeIndex[edgeType];
            break;
        default:
            std::cout << "invalad direction" << std::endl;
            return;
            break;
        }

    }


    void setWalls() {
        for (int c = 0; c < blocks.size(); c++) {

            //scan for blocks that are active.
            if (blocks[c].active) {

                // is there an active block to the west
                if (!blocks[c - 1].active) {

                    // if there is no active block to the west check the north
                    if (!blocks[c - colNum].active) {
                        //if there is no active block to the north or west creat new edge to the west
                        addWall(c, 3);
                    }
                    else {
                        // if there is a northern block does it have a western edge
                        if (!blocks[c - colNum].edgeActive[3]) {
                            //if there is no western edge on the northern block add a western edge to this block
                            addWall(c, 3);
                        }
                        else {
                            // if the northern block does have a western edge extend its western edge
                            extendEdge(c, 3);
                        }
                    }
                }
                // if there is a block to the west make sure that both blocks are on the same y level
                else if (getCord(c).y != getCord(c - 1).y) {
                    addWall(c, 3);
                }

                // is there an active block to the east
                if (!blocks[c + 1].active) {
                    // if there is no active block to the east check the north
                    if (!blocks[c - colNum].active) {
                        // if block does not have a block to the east or the north create new edge to the east;
                        addWall(c, 1);

                    }
                    else {
                        // if block has a block to it's north does that block have an eastern edge
                        if (!blocks[c - colNum].edgeActive[1]) {
                            // if the block to the north does not have an eastern edge creat an eastern edge on this block
                            addWall(c, 1);
                        }
                        else {
                            // if the northern block does have an eastern edge extend that edge.
                            extendEdge(c, 1);
                        }
                    }
                }
                // if there is a block to the east make sure that both blocks are on the same y level
                else if (getCord(c).y != getCord(c + 1).y) {
                    addWall(c, 1);
                }

                // is there an active block to the north
                if (!blocks[c - colNum].active) {
                    // if there is no active block to the north is there one to the west
                    if (!blocks[c - 1].active || getCord(c).y != getCord(c - 1).y) { // also check that the west block is on the same y level as this block
                        // if there is no active block to the north or west create a new edge to the north
                        addWall(c, 0);
                    }
                    else {
                        // if block has a block to the west does that block have a northern edge
                        if (!blocks[c - 1].edgeActive[0]) {
                            // if the block to the west does not have a northern edge create a northern edge on this block
                            addWall(c, 0);
                        }
                        else {
                            // if the western block does have a northern edge extend that edge
                            extendEdge(c, 0);
                        }
                    }
                }

                // if there is an active block to the south
                if (!blocks[c + colNum].active) {
                    // if there is no active block to the south is there one to the west
                    if (!blocks[c - 1].active || getCord(c).y != getCord(c - 1).y) { // also check that the west block is on the same y level as this block
                        // if there is no active block to the south or west create a southern edge
                        addWall(c, 2);
                    }
                    else {
                        // if block has a block to the west does that block have a southern edge
                        if (!blocks[c - 1].edgeActive[2]) {
                            // if the western block does not have a southern edge create a southern edge on this block
                            addWall(c, 2);
                        }
                        else {
                            // if the western block does have a southern edge extend that edge;
                            extendEdge(c, 2);
                        }
                    }
                }
            }


        }
    }



}

