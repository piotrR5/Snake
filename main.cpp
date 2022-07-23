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

    while(true){
        c = getch();
        int foo = (c!=ERR ? c : foo);
        //clear();
        
        if(foo=='w')snake.move(UP);
        else if(foo=='s')snake.move(DOWN);
        else if(foo='d')snake.move(RIGHT);
        else if(foo=='a')snake.move(LEFT);
        keyPressed(foo);
        showSnake(snake);
        
        //else printf("nothing received", NULL, NULL);
        
        usleep(GAMETICK);
        system("clear");
    }
    endwin();
}


int main(){
    gameLoop();
    return 0;

}