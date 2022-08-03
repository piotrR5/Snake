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
#include <ctime>
#include <cstdlib>

//game handling:
//  fruits
//  walls

#define EMPTY 0
#define FRUIT 1
#define BODY 2
#define HEAD 3
#define WALL 4


#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4

#define EDGEDOWN 0
#define EDGEUP 25

#define GAMETICK_EASY 350000
#define GAMETICK_NORMAL 300000
#define GAMETICK_HARD 200000

class GameObject{

public:
    int type;
    std::pair<int,int>pos;


    GameObject(int type);
    GameObject();
};



class SnakeBody : public GameObject{
public:
    int dir;
    std::pair<int,int>prevPos;
    SnakeBody(std::pair<int,int>pos);

};


class Snake{
public:
    int lastDir;
    std::vector<SnakeBody>body;
    int size();
    void grow();
    void move(int dir);
    bool isAtEdge();
    bool hitSnakeSelf();
    bool isInSnake(std::pair<int,int>pos);
    bool hitSnakeWall();
    
    //TODO bool hitWall();
    

    Snake();
    Snake(std::vector<SnakeBody> snake);
    Snake(SnakeBody head);
};

class Plane{
public:
    int GAMETICK;
    int sizeX;
    int sizeY;
    int highScore;
    std::vector<std::vector<GameObject>>plane;
    Snake snake;
    
    void generateFruit(int n);
    void generateWalls(std::vector<std::pair<int,int>>pos);
    int update();
    void isAtFruit();
    

    Plane(int sizeX, int sizeY, Snake snake);
};