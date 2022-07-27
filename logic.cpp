#include "logic.h"

GameObject::GameObject(int type){
    this->type=type;
}

GameObject::GameObject(){
    this->type=0;
}



SnakeBody::SnakeBody(std::pair<int,int>pos){
    this->pos=pos;
    this->dir=UP;
    this->prevPos=std::pair<int,int>(pos.first-1, pos.second-1);
}


int Snake::size(){
    return this->body.size();
}

void Snake::move(int dir){
    lastDir=body[0].dir;
    body[size()-1].dir=dir;
    body[size()-1].prevPos=body[size()-1].pos;
    if(body[size()-1].dir==UP){
        body[size()-1].pos.first++;
    }else if(body[size()-1].dir==DOWN){
        body[size()-1].pos.first--;
    }else if(body[size()-1].dir==RIGHT){
        body[size()-1].pos.second++;
    }else{
        body[size()-1].pos.second--;
    }

    for(int i=0;i<size()-1;i++){
        body[i].prevPos=body[i].pos;
        if(body[i].dir==UP){
            body[i].pos.first++;
        }
        else if(body[i].dir==DOWN){
            body[i].pos.first--;
        }
        else if(body[i].dir==RIGHT){
            body[i].pos.second++;
        }
        else if(body[i].dir==LEFT){
            body[i].pos.second--;
        }
        body[i].dir=body[i+1].dir;
    }
}

void Snake::grow(){
    //strange edgecase when only head is present
    if(size()==1){
        int x=body[0].pos.first,y=body[0].pos.second;
        int dir;
        if(body[0].dir==UP){
            x--;
            dir=UP;
        }
        else if(body[0].dir==DOWN){
            x++;
            dir=DOWN;
        }   
        else if(body[0].dir==RIGHT){
            y--;
            dir=RIGHT;
        }
        else if(body[0].dir==LEFT){
            y++;
            dir=LEFT;
        }
        body.emplace(body.begin(), SnakeBody({x,y}));
        body[0].dir=dir;
        move(body[size()-1].dir);
        return;
    }

    body.emplace(body.begin(), body[0].prevPos);
    body[0].dir=lastDir;
    move(body[size()-1].dir);
}

bool Snake::isAtEdge(){
    if( body[size()-1].pos.first==EDGEDOWN-1 ||
        body[size()-1].pos.second==EDGEDOWN-1 ||
        body[size()-1].pos.first==EDGEUP ||
        body[size()-1].pos.second==EDGEUP)return true;
    return false;
}

bool Snake::hitSnakeSelf(){
    for(int i=0;i<size();i++){
        for(int j=i+1;j<size();j++){
            if(body[i].pos.first==body[j].pos.first && body[i].pos.second==body[j].pos.second)return true;
        }
    }
    return false;
}

bool Snake::isInSnake(std::pair<int,int>pos){
    for(int i=0;i<size();i++){
        if(body[i].pos==pos)return true;
    }
    return false;
}

Snake::Snake(std::vector<SnakeBody> snake){
    body=snake;
}

Snake::Snake(SnakeBody head){
    body=std::vector<SnakeBody>(1, head);
}

Snake::Snake(){
    body=std::vector<SnakeBody>(1, SnakeBody({EDGEUP/2, EDGEUP/2}));
}



void Plane::update(){
    for(int i=0;i<EDGEUP;i++){
        for(int j=0;j<EDGEUP;j++){
            if(plane[i][j].type!=FRUIT){
                plane[i][j].type=EMPTY;
            }   
        }
    }
    
    if(plane[snake.body[snake.size()-1].pos.first][snake.body[snake.size()-1].pos.second].type==FRUIT){
        plane[snake.body[snake.size()-1].pos.first][snake.body[snake.size()-1].pos.first].type=EMPTY;
        snake.grow();
        generateFruit(1);
    }

    for(auto i:snake.body){
        plane[i.pos.first][i.pos.second].type=BODY;
    }
    plane[snake.body[snake.body.size()-1].pos.first][snake.body[snake.body.size()-1].pos.second]=HEAD;
}

void Plane::generateFruit(int n){
    srand(time(NULL));
    int x=0,y=0;
    while(n--){
        do{
            x=rand()%EDGEUP;
            y=rand()%EDGEUP;
        }while(plane[x][y].type!=EMPTY);
        plane[x][y].type=FRUIT;
    }
}

void Plane::isAtFruit(){
    if(plane[snake.body[snake.size()-1].pos.first][snake.body[snake.size()-1].pos.first].type==FRUIT){
        plane[snake.body[snake.size()-1].pos.first][snake.body[snake.size()-1].pos.second].type=EMPTY;
        std::cout<<"OWOC!";
        snake.grow();
        generateFruit(1);
    }
}

Plane::Plane(int sizeX, int sizeY, Snake snake){
    this->sizeX=sizeX;
    this->sizeY=sizeY;
    this->snake=Snake(snake);
    plane=std::vector<std::vector<GameObject>>(EDGEUP, std::vector<GameObject>(EDGEUP, GameObject(0)));
    for(auto i:snake.body){
        plane[i.pos.first][i.pos.second].type=BODY;
    }
    plane[snake.body[snake.body.size()-1].pos.first][snake.body[snake.body.size()-1].pos.second]=HEAD;
}







