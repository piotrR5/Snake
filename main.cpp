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
                else if(plane.plane[i][j].type==WALL)std::cout<<"00";
            }
            std::cout<<"\r"<<std::endl;
        }
    }
};
///DEBUG

void printScreen(WINDOW *w, Plane& plane, int keyPressed){
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
            else if(plane.plane[i][j].type==WALL)std::cout<<"00";
        }
        std::cout<<" |\n\r";
    }
    std::cout<<std::string(12,' ')<<std::string(54,'-')<<"\n\r";
    if(plane.snake.size()>plane.highScore)plane.highScore=plane.snake.size();
    std::cout<<"\n\r";
    std::cout<<std::string(15, ' ')<<"[ current score: "<<plane.snake.size()<<" ]\n\r";
    std::cout<<std::string(15, ' ')<<"[ highscore: "<<plane.highScore<<" ]\n\r";
    std::cout<<std::string(15, ' ')<<"[ input: ";
    if(keyPressed==KEY_UP)std::cout<<"ARR_UP";
    else if(keyPressed==KEY_DOWN)std::cout<<"ARR_DOWN";
    else if(keyPressed==KEY_RIGHT)std::cout<<"ARR_RIGHT";
    else if(keyPressed==KEY_LEFT)std::cout<<"ARR_LEFT";
    std::cout<<" ]\n\r";

}


int getHighScore(std::string filename){
    std::fstream file;
    file.open(filename);
    std::string foo;
    file>>foo;
    file.close();
    return stoi(foo);
}

void saveScore(std::string filename, int currentHighScore){
    std::ofstream file;
    file.open(filename, std::ios::in);
    file<<std::to_string(currentHighScore);
    file.close();
}

void levelBuilder(WINDOW *w, Plane& plane){
    plane.GAMETICK=GAMETICK_EASY;
    int input=10;
    int x=10,y=10;
    keypad(w,true);
    nodelay(w, false);
    //intrflush(stdscr, FALSE);
    system("clear");
    while(true){
        std::cout<<std::string(10,' ')<<"Use arrow keys to move the cursor,\n\r";
        std::cout<<std::string(10,' ')<<"press f to place or remove fruit,\n\r";
        std::cout<<std::string(10,' ')<<"press w to place or remove wall.\n\r";
        std::cout<<std::string(10,' ')<<"change velocity of snake using 1,2,3\n\r";
        std::cout<<"\t\t\tx: "<<x<<" y: "<<y<<"\n\r";
        /*for(auto& i:plane.plane){
            for(auto& j:i){
                std::cout<<"[";
                if(j.pos.first==x && j.pos.second==y)std::cout<<"|";
                else if(j.type==EMPTY)std::cout<<" ";
                else if(j.type==FRUIT)std::cout<<"o";
                else if(j.type==BODY)std::cout<<"#";
                else if(j.type==HEAD)std::cout<<"0";
                else if(j.type==WALL)std::cout<<"w";
                std::cout<<"]";
            }
            std::cout<<"\n\r";
        }*/

        for(int i=0;i<plane.sizeX;i++){
            for(int j=0;j<plane.sizeX;j++){
                std::cout<<"[";
                if(i==x && j==y)std::cout<<"|";
                else if(plane.plane[24-i][j].type==EMPTY)std::cout<<" ";
                else if(plane.plane[24-i][j].type==FRUIT)std::cout<<"o";
                else if(plane.plane[24-i][j].type==BODY)std::cout<<"#";
                else if(plane.plane[24-i][j].type==HEAD)std::cout<<"0";
                else if(plane.plane[24-i][j].type==WALL)std::cout<<"w";
                std::cout<<"]";
            }
            std::cout<<"\n\r";
        }
        std::cout<<"\t\t\tSnake speed: "<<plane.GAMETICK<<"\n\r";
        input=getch();
        if(input==KEY_UP){
            if(x>0)x--;
        }
        else if(input==KEY_DOWN){
            if(x<24)x++;
        }
        else if(input==KEY_RIGHT){
            if(y<24)y++;
        }
        else if(input==KEY_LEFT){
            if(y>0)y--;
        }
        else if(input=='f'){
            if(plane.plane[24-x][y].type==EMPTY)plane.plane[24-x][y].type=FRUIT;
            else if(plane.plane[24-x][y].type==FRUIT)plane.plane[24-x][y].type=EMPTY;
        }
        else if(input=='w'){
            if(plane.plane[24-x][y].type==EMPTY)plane.plane[24-x][y].type=WALL;
            else if(plane.plane[24-x][y].type==WALL)plane.plane[24-x][y].type=EMPTY;
        }
        else if(input=='1'){
            plane.GAMETICK=GAMETICK_EASY;
        }
        else if(input=='2'){
            plane.GAMETICK=GAMETICK_NORMAL;
        }
        else if(input=='3'){
            plane.GAMETICK=GAMETICK_HARD;
        }
        else if(input==' ' || input==KEY_ENTER){
            return;
        }
        system("clear");
    }
}

int gameLoop(){
    WINDOW *w;
    int c;

        

    
    w = initscr();
    noecho();
    
    Snake snake(SnakeBody({10,10}));
    keypad(w,true);
    Plane plane(EDGEUP, EDGEUP, snake);
     
    ////////////////////////////////////////////////////STARTING SCREEN////////////////////
    int diff=1;
    int usrInput;
    
    system("clear");
    while(true){
        std::cout<<"\n\t\t\t##### ##   #  ###  #   # #####\r"<<std::endl;
        std::cout<<"\t\t\t#     # #  # #   # #  #  #    \r"<<std::endl;
        std::cout<<"\t\t\t##### #  # # ##### # #   #### \r"<<std::endl;
        std::cout<<"\t\t\t    # #   ## #   # #  #  #    \r"<<std::endl;
        std::cout<<"\t\t\t##### #    # #   # #   # #####\r"<<std::endl;
        std::cout<<"\n\n\n";
        std::cout<<"\t\t\tChoose difficulty: \n\r";
        if(diff==0)std::cout<<"\t\t\t[ x ] Easy (slow game, a lot of fruit.\n\r";
        else std::cout<<"\t\t\t[   ] Easy (slow game, a lot of fruit\n\r";
        if(diff==1)std::cout<<"\t\t\t[ x ] Normal (faster game, just enough fruit.\n\r";
        else std::cout<<"\t\t\t[   ] Normal (faster game, just enough fruit.\n\r";
        if(diff==2)std::cout<<"\t\t\t[ x ] Hard (hellishly fast game, barely any fruit.\n\r";
        else std::cout<<"\t\t\t[   ] Hard (hellishly fast game, barely any fruit.\n\r";
        std::cout<<"\t\t\tIf you made up your mind, press [SPACE]\n\r";
        std::cout<<"\t\t\tPress e to enter level editor\n\r";
        usrInput=getch();
        if(usrInput==KEY_UP && diff>0)diff--;
        else if(usrInput==KEY_DOWN && diff<2)diff++;
        else if(usrInput==KEY_ENTER || usrInput==' '){
            switch(diff){
                case 0:{
                    plane.GAMETICK=GAMETICK_EASY;
                    plane.generateFruit(15);
                }
                break;
                case 1:{
                    plane.GAMETICK=GAMETICK_NORMAL;
                    plane.generateFruit(10);
                }
                break;
                case 2:{
                    plane.GAMETICK=GAMETICK_HARD;
                    plane.generateFruit(3);
                }
                break;
            }
            break;
        }
        else if(usrInput=='e'){
            levelBuilder(w,plane);
            break;
        }
        system("clear");
    }
    plane.snake.body[0].pos.first=10;
    plane.snake.body[0].pos.second=10;
    plane.update();
    nodelay(w, true); 
    ////////////////////////////////////////////////////STARTING SCREEN////////////////////
    
    
    plane.highScore=getHighScore("highScore.snake");
    
    c = KEY_UP;
    int foo = KEY_UP;
    int temp = KEY_UP;
    printScreen(w, plane, foo);
    //do{c=getchar();}while(c=='s');
    while(true){
         
        c = getch();
        if(c!=ERR)foo=c;
    
        if(foo==KEY_UP && temp!=KEY_DOWN){
            plane.snake.move(UP);
            temp=KEY_UP;
        }
        else if(foo==KEY_DOWN && temp!=KEY_UP){
            plane.snake.move(DOWN);
            temp=KEY_DOWN;
        }
        else if(foo==KEY_RIGHT && temp!=KEY_LEFT){
            plane.snake.move(RIGHT);
            temp=KEY_RIGHT;
        }
        else if(foo==KEY_LEFT && temp!=KEY_RIGHT){
            plane.snake.move(LEFT);
            temp=KEY_LEFT;
        }else{
            foo=temp;
            if(plane.update()==-1)return plane.highScore;
            printScreen(w, plane, foo);
            usleep(plane.GAMETICK);
            system("clear");
            //continue;
        }
        //else if(foo=='g'){
        //    plane.snake.grow();
        //    foo=temp;
        //}
        
        

       
        

        if(plane.snake.isAtEdge()){
            std::cout<<std::string(32, ' ')<<"GAME OVER, you hit the edge\r"<<std::endl;
            //saveScore("highScore.snake", plane.highScore, plane.snake.size());
            usleep(3000000);
            break;

        }

        if(plane.snake.hitSnakeSelf()){
            std::cout<<std::string(32, ' ')<<"GAME OVER, you hit yourself\r"<<std::endl;
            //saveScore("highScore.snake", plane.highScore, plane.snake.size());
            usleep(3000000);
            break;
        }
        if(plane.update()==-1)return plane.highScore;
        printScreen(w, plane, foo);
        
        
        
        usleep(plane.GAMETICK);
        system("clear");
        touchwin(w);
        wrefresh(w);
        
        
    }
    return plane.highScore;
}


int main(){
    int highScore = gameLoop();
    std::cout<<highScore;
    saveScore("highScore.snake",highScore);
    endwin();
    return 0;
}