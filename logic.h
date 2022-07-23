#include <vector>
#include <stack>
#include <map>
#include <string>
#include <thread>
#include <memory>
#include <mutex>
#include <future>
#include <condition_variable>
#include <ncurses.h>
#include <fstream>
#include <iostream>
//#include <deque>
//TODO
//snake handling:
//  movement
//  loss
//  gaining length
//
//game handling:
//  game loop
//  dialog with frontend.h
//  degub features
//async input handling

#define EMPTY 0;
#define FRUIT 1;
#define BODY 2;
#define HEAD 3;


#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4

#define GAMETICK 500000

class GameObject{
    int type;
    std::pair<int,int>pos;
public:
    int getType();
    void setType(int type);

    GameObject(int type);
    GameObject();
};



class SnakeBody : public GameObject{
public:
    std::pair<int,int>pos;
    int dir;
    std::pair<int,int>prevPos;
    SnakeBody(std::pair<int,int>pos);

};


class Snake{
public:

    std::vector<SnakeBody>body;
    int size();
    void grow();
    void move(int dir);
    

    Snake(std::vector<SnakeBody> snake);
    Snake(SnakeBody head);
};

class Plane{
public:

    const int sizeX;
    const int sizeY;
    std::vector<std::vector<GameObject>>plane;
    

    
    Plane(int sizeX, int sizeY);
};






