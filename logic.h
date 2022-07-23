#include <vector>
#include <stack>
#include <map>
#include <string>
#include <thread>
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

class GameObject{
    int type;
    std::pair<int,int>pos;
public:
    int getType();
    void setType(int type);

    std::pair<int,int>getPos();
    void setPos(int x, int y);

    GameObject(int x, int y, int type);
    GameObject(int type);
    GameObject();
};





class Snake : public GameObject{
    std::stack<std::pair<int,int>> body;
    std::vector<std::pair<int,int>>prevPos;

public:
    void grow();

    std::vector<std::pair<int,int>> getBody();
    std::vector<std::pair<int,int>> getPrevPos();
    

    Snake(std::vector<std::pair<int,int>>body);
    Snake(int x, int y);
    Snake();
};

class Fruit : public GameObject{
    std::pair<int,int>pos;

public:
    void getEaten();

    Fruit(int x, int y);
    Fruit();
};

class Plane{
    int sizeX;
    int sizeY;
    std::vector<std::vector<GameObject>>plane;
public:
    int getSizeX();
    int getSizeY();
    void setSizeX();
    void setSIzeY();

    GameObject at(int x, int y);
    void set(int x, int y, GameObject val);
    

    
    Plane(int sizeX, int sizeY);
};

char asyncInput();

int gameLoop();


