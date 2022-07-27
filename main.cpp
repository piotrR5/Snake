#include <iostream>
#include <ncurses.h>
#include <vector>
#include "logic.h"
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
struct debug{
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

    void debugSnake(Snake& snake){
        std::cout<<"Hit himself? ["<<(snake.hitSnakeSelf() ? "true" : "false")<<"\r"<<std::endl;
    }

    void debugIsAtEdge(Snake& snake){
        std::cout<<"[DEBUG] snake got to the edge of the map:\r"<<std::endl;
        std::cout<<"heads position: "<<snake.body[0].pos.first<<" "<<snake.body[0].pos.second<<"\r"<<std::endl;
    }

    void debugHitSnakeSelf(Snake& snake){
        std::cout<<"[DEBUG] Snake just hit itself: \r"<<std::endl;
        std::cout<<"snake positions: ";
        for(auto i:snake.body)std::cout<<"["<<i.pos.first<<" "<<i.pos.second<<"]\r"<<std::endl;
        for(int i=0;i<snake.size();i++){
            for(int j=i+1;j<snake.size();j++){
                if(snake.body[i].pos.first==snake.body[j].pos.first && snake.body[i].pos.second==snake.body[j].pos.second)std::cout<<"Snake hit himself at: ["<<snake.body[j].pos.first<<" "<<snake.body[j].pos.second<<"]\n\r";
            }
        }
    }

    void debugShowHighScore(int hs){
        std::cout<<"Current HighScore: ["<<hs<<"]\n\r";
    }

    void printPlane(Plane& plane){
        for(int i=plane.sizeY;i>=0;i--){
            for(int j=plane.sizeX;j>=0;j--){
                if(plane.plane[i][j].type==EMPTY)std::cout<<"  ";
                else if(plane.plane[i][j].type==FRUIT)std::cout<<"o ";
                else if(plane.plane[i][j].type==BODY)std::cout<<"[]";
                else if(plane.plane[i][j].type==HEAD)std::cout<<"##";
            }
            std::cout<<"\r"<<std::endl;
        }
    }
};
///DEBUG

void printScreen(WINDOW *w, Plane& plane, char keyPressed){
    std::cout<<"\n\t\t\t##### ##   #  ###  #   # #####\r"<<std::endl;
    std::cout<<"\t\t\t#     # #  # #   # #  #  #    \r"<<std::endl;
    std::cout<<"\t\t\t##### #  # # ##### # #   #### \r"<<std::endl;
    std::cout<<"\t\t\t    # #   ## #   # #  #  #    \r"<<std::endl;
    std::cout<<"\t\t\t##### #    # #   # #   # #####\r"<<std::endl;
    std::cout<<"\n";
    //showSnake(plane.snake);
    std::cout<<std::string(12,' ')<<std::string(54,'-')<<"\n\r";

    for(int i=EDGEUP-1;i>=0;i--){
        std::cout<<std::string(12,' ')<<"| ";
        for(int j=0;j<EDGEUP;j++){
            if(plane.plane[i][j].type==EMPTY)std::cout<<"  ";
            else if(plane.plane[i][j].type==BODY)std::cout<<"##";
            else if(plane.plane[i][j].type==HEAD)std::cout<<"OO";
            else if(plane.plane[i][j].type==FRUIT)std::cout<<"<>";
        }
        std::cout<<" |\n\r";
    }
    std::cout<<std::string(12,' ')<<std::string(54,'-')<<"\n\r";

    std::cout<<"\n\r";
    std::cout<<std::string(15, ' ')<<"[ current score: "<<plane.snake.size()<<" ]\n\r";
    std::cout<<std::string(15, ' ')<<"[ highscore: "<<plane.highScore<<" ]\n\r";
    std::cout<<std::string(15, ' ')<<"[ input: "<<keyPressed<<" ]\n\r";

}


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
    Snake snake(SnakeBody({10,10}));

    Plane plane(EDGEUP, EDGEUP, snake);
    
    plane.generateFruit(5);
    plane.highScore=getHighScore("highScore.snake");
    plane.GAMETICK=GAMETICK_NORMAL;
    c = ' ';
    char foo = ' ';
    char temp;
    do{c=getchar();}while(c=='s');
    while(true){
         
        c = getch();
        if(c!=ERR)foo=c;
    
        if(foo=='w' && temp!='s'){
            plane.snake.move(UP);
            temp='w';
        }
        else if(foo=='s' && temp!='w'){
            plane.snake.move(DOWN);
            temp='s';
        }
        else if(foo=='d' && temp!='a'){
            plane.snake.move(RIGHT);
            temp='d';
        }
        else if(foo=='a' && temp!='d'){
            plane.snake.move(LEFT);
            temp='a';
        }
        else if(foo=='g'){
            plane.snake.grow();
            foo=temp;
        }
        else if(foo!=' '){
            foo=temp;
            plane.update();
            printScreen(w, plane, foo);
            system("clear");
            continue;
        }
        plane.update();
        printScreen(w, plane, foo);

       
        

        if(plane.snake.isAtEdge()){
            std::cout<<std::string(32, ' ')<<"GAME OVER, you hit the edge\r"<<std::endl;
            usleep(3000000);
            break;
        }

        if(plane.snake.hitSnakeSelf()){
            std::cout<<std::string(32, ' ')<<"GAME OVER, you hit yourself\r"<<std::endl;
            usleep(10000000);
            break;
        }
        
        
        
        usleep(plane.GAMETICK);
        system("clear");
        touchwin(w);
        wrefresh(w);
        
        
    }
    endwin();
    
}


int main(){
    gameLoop();
    return 0;
}