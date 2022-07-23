#include "logic.h"

int GameObject::getType(){
    return this->type;
}

void GameObject::setType(int type){
    this->type=type;
}

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

    

    

    for(int i=size()-1;i>0;i--){
        body[i].prevPos=body[i].pos;
        if(body[i].dir==UP){
            body[i].pos.first++;
            body[i].dir=body[i-1].dir;
        }else if(body[i].dir==DOWN){
            body[i].pos.first--;
            body[i].dir=body[i-1].dir;
        }else if(body[i].dir==RIGHT){
            body[i].pos.first++;
            body[i].dir=body[i-1].dir;
        }else{
            body[i].pos.first--;
            body[i].dir=body[i-1].dir;
        }
    }

    body[0].dir=dir;
    body[0].prevPos=body[0].pos;
    if(body[0].dir==UP){
        body[0].pos.first++;
    }else if(body[0].dir==DOWN){
        body[0].pos.first--;
    }else if(body[0].dir==RIGHT){
        body[0].pos.second++;
    }else{
        body[0].pos.second--;
    }
}

void Snake::grow(){
    body.push_back(SnakeBody(body[body.size()-1].prevPos));
}


Snake::Snake(std::vector<SnakeBody> snake){
    body=snake;
}

Snake::Snake(SnakeBody head){
    body=std::vector<SnakeBody>(1, head);
}









