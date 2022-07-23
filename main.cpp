#include <iostream>
#include <ncurses.h>
#include <vector>
#include "logic.h"
#include "frontend.h"
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


///DEBUG
void keyPressed(char foo){
    printf("received %c (%d)\n\r", foo, (char)foo);
}

void showSnake(Snake& snake){
    for(auto& i:snake.body){
        std::string s;
        if(i.dir==1)s="UP";
        else if(i.dir==2)s="DOWN";
        else if(i.dir==3)s="RIGHT";
        else if(i.dir==4)s="LEFT";

        std::cout<<"["<<i.pos.first<<" "<<i.pos.second<<" "<<s<<"]\r"<<std::endl;
    }
}

void debugHead(SnakeBody& head){
    std::cout<<"Head position: ["<<head.pos.first<<" "<<head.pos.second<<"]\r"<<std::endl;
    std::cout<<"Head direction: [";
    if(head.dir==UP)std::cout<<"UP]\r"<<std::endl;
    if(head.dir==DOWN)std::cout<<"DOWN]\r"<<std::endl;
    if(head.dir==RIGHT)std::cout<<"RIGHT]\r"<<std::endl;
    if(head.dir==LEFT)std::cout<<"LEFT]\r"<<std::endl;
    std::cout<<"Is on edge? [";
    if( head.pos.first==EDGEDOWN ||
        head.pos.second==EDGEDOWN ||
        head.pos.first==EDGEUP ||
        head.pos.second==EDGEUP)std::cout<<"true]\r"<<std::endl;
    else std::cout<<"false]\r"<<std::endl;
}

bool isAtEdge(Snake& snake){
    if( snake.body[0].pos.first==EDGEDOWN ||
        snake.body[0].pos.second==EDGEDOWN ||
        snake.body[0].pos.first==EDGEUP ||
        snake.body[0].pos.second==EDGEUP)return true;
    return false;
}

void debugIsAtEdge(Snake& snake){
    std::cout<<"[DEBUG] snake got to the edge of the map:\r"<<std::endl;
    std::cout<<"heads position: "<<snake.body[0].pos.first<<" "<<snake.body[0].pos.second<<"\r"<<std::endl;
}
///DEBUG

int getHighScore(std::string filename){
    std::fstream file;
    file.open(filename);
    std::string foo;
    file>>foo;
    file.close();
    return stoi(foo);
    
}

int gameLoop(){
    WINDOW *w;
    char c;
    
    w = initscr();
    noecho();
    nodelay(w, true);

    Snake snake(std::vector<SnakeBody>({
        SnakeBody(std::pair<int,int>(10,10)),
        SnakeBody(std::pair<int,int>(11,10)),
        SnakeBody(std::pair<int,int>(12,10)),
        SnakeBody(std::pair<int,int>(13,10)),
        SnakeBody(std::pair<int,int>(14,10))
    }));
    c = ' ';
    char foo = ' ';
    char temp;
    do{
        c = getch();
    }while(c!='a' && c!='d');
    while(true){
        c = getch();
        if(c!=ERR)foo=c;
        
        //clear();
        if(foo=='w' && temp!='s'){
            snake.move(UP);
            temp='w';
        }
        else if(foo=='s' && temp!='w'){
            snake.move(DOWN);
            temp='s';
        }
        else if(foo=='d' && temp!='a'){
            snake.move(RIGHT);
            temp='d';
        }
        else if(foo=='a' && temp!='d'){
            snake.move(LEFT);
            temp='a';
        }
        else if(foo=='g'){
            snake.grow();
            foo=temp;
        }
        else if(foo!=' '){
            foo=temp;
            keyPressed(foo);
            showSnake(snake);
            debugHead(snake.body[0]);
            //usleep(GAMETICK);
            system("clear");
            continue;
        }
        keyPressed(foo);
        showSnake(snake);
        debugHead(snake.body[0]);
        

        if(isAtEdge(snake)){
            debugIsAtEdge(snake);
            std::cout<<"GAME OVER\r"<<std::endl;
            usleep(3000000);
            break;
        }
        
        
        
        usleep(GAMETICK);
        system("clear");
        
    }
    endwin();
}


int main(){
    gameLoop();
    return 0;
}