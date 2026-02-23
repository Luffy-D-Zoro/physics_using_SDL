//Include

#include<SDL.h>
#include<SDL_ttf.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

//define 
#define width 400
#define height 500
#define white 0xffffffff
#define black 0x00000000
#define Black (SDL_Color){0,0,0,0}
#define White (SDL_Color){255,255,255,255}
#define green 0xff0000ff
#define h_color 0xffdfdfdf
#define blue 0x0000ffff
#define turn_height 250
#define winner_x 100
#define winner_y 300
#define winner_h 80
#define winner_w 200
#define playagain_x 120
#define playagain_y 400
#define playagain_w 150
#define playagain_h 40
#define gap 5
#define size 50

//struct
typedef struct{
    SDL_Rect cell_rect;
    char val[2];
    int is_filled;
    int is_hover;
    int is_click;
    int is_full;
}cell;
typedef struct{
    SDL_Rect turn_rect;
    SDL_Rect play_again;
    SDL_Rect winner_rect;
}Screen;
typedef struct{
    char turn[2];
    SDL_Rect turn_data_rect;
    int is_gameover;
    int is_playagain;
    SDL_Rect winner_Data_rect;
    char winner[2];
}data;

//func
void logic(cell* board,data* display_data){
    int winninglines[8][3] = {{0,1,2},{3,4,5},{6,7,8}, {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}};
    int full=1;
    for(int i=0;i<8;i++){
        if(board[i].is_filled==0){
            full=0;
        }
        if(board[winninglines[i][0]].is_filled==1  && board[winninglines[i][0]].val[0]==board[winninglines[i][1]].val[0] && board[winninglines[i][0]].val[0] ==board[winninglines[i][2]].val[0]){
            display_data->winner[0]= board[winninglines[i][0]].val[0];
            display_data->is_gameover=1;
            return;
        }
    }
    if(board[8].is_filled==0){
        full=0;
    }
    if(full==1){
        display_data->winner[0]='D';
        display_data->is_gameover=1;
    }
}

void show_input(cell* board,SDL_Surface *surface,TTF_Font* font){
    for(int i=0;i<9;i++){
        if(board[i].is_filled){
            SDL_Surface* board_surf = TTF_RenderText_Blended(font,board[i].val,Black);
            SDL_Rect text_rect = (SDL_Rect){board[i].cell_rect.x+(board[i].cell_rect.w-board_surf->w)/2,board[i].cell_rect.y+(board[i].cell_rect.h-board_surf->h)/2,board_surf->w,board_surf->h};
            SDL_BlitSurface(board_surf,NULL,surface,&text_rect);
            SDL_FreeSurface(board_surf);
        }
    }
}

void input(cell* board,data* dispaly_data,SDL_Surface* surface,TTF_Font* font){
    for(int i=0;i<9;i++){
        if(board[i].is_click==1 && board[i].is_filled==0){
            strcpy(board[i].val,dispaly_data->turn);
            if(dispaly_data->turn[0]=='X'){
                dispaly_data->turn[0]='0';
            }
            else if(dispaly_data->turn[0]=='0'){
                dispaly_data->turn[0]='X';
            }
            board[i].is_filled=1;
            // show_input(&board[i],surface,font);
        }
    }
}

void board_init(cell *board,SDL_Surface* surface){
    for(int i=0;i<9;i++){
        board[i].cell_rect = (SDL_Rect){120+(i%3)*size+(i%3)*gap,50+(i/3)*(size+gap),size,size};
        SDL_FillRect(surface,&board[i].cell_rect,white);
        board[i].val[0] = '\0';
        board[i].val[1] = '\0';
        board[i].is_filled=0;
        board[i].is_hover=0;
        board[i].is_click=0;
        board[i].is_full=0;
        
    }
}
int  isHover(int x,int y,SDL_Rect* rect){
    return (x<=rect->x+rect->w &&
            x>=rect->x && 
            y>=rect->y && 
            y<=rect->y+rect->h
            );
}
void color_hover(SDL_Surface* surface ,cell* board){
    for(int i=0;i<9;i++){
        if(board[i].is_hover==1){
            SDL_FillRect(surface,&board[i].cell_rect,h_color);
        }
        else{
            SDL_FillRect(surface,&board[i].cell_rect,white);
        }
    }
}

void after_gameover(Screen* display,data* display_data,TTF_Font* font,TTF_Font* font_win, SDL_Surface* surface){
    //play again
    char str[32] ;
    snprintf(str,sizeof(str),"Winner :%s",display_data->winner);
    display->play_again = (SDL_Rect){playagain_x,playagain_y,playagain_w,playagain_h};
    SDL_Surface* text_surface = TTF_RenderText_Blended(font,"Play Again",Black);
    SDL_Rect text_play_rect = (SDL_Rect){playagain_x+(playagain_w-text_surface->w)/2,playagain_y+(playagain_h-text_surface->h)/2,text_surface->w,text_surface->h};
    if(display_data->is_playagain==0){
        SDL_FillRect(surface,&display->play_again,blue);
    }
    else{
        SDL_FillRect(surface,&display->play_again,black);
    }
    SDL_BlitSurface(text_surface,NULL,surface,&text_play_rect);

    //winner
    display->winner_rect = (SDL_Rect){winner_x,winner_y,winner_w,winner_h};
    SDL_Surface *win_surf = TTF_RenderText_Blended(font_win,str,Black);
    SDL_Rect win_text_rect = (SDL_Rect){winner_x+(winner_w-win_surf->w)/2,winner_y+(winner_h-win_surf->h)/2,win_surf->w,win_surf->h};
    if(display_data->is_playagain==0){
        SDL_FillRect(surface,&display->winner_rect,green);
    }
    else{
        SDL_FillRect(surface,&display->winner_rect,black);
    }
    SDL_BlitSurface(win_surf,NULL,surface,&win_text_rect);
    SDL_FreeSurface(win_surf);
    SDL_FreeSurface(text_surface);
}

void screen_init(Screen* display,data* display_data,TTF_Font* font,TTF_Font* font_paly,TTF_Font* font_win,SDL_Surface* surface){
    //text rect
    //SDL_Color White =(SDL_Color){255,255,255,255};
    display->turn_rect =(SDL_Rect){130,250,130,30};
    char str[32];
    snprintf(str,sizeof(str),"Current Turn :%s",display_data->turn);
    SDL_Surface* textSurface = TTF_RenderText_Blended(font,str,Black);
    SDL_Rect text_Rect =  (SDL_Rect){display->turn_rect.x+5,turn_height+5,120,textSurface->h};
    if(display_data->is_gameover==0){
        SDL_FillRect(surface,&display->turn_rect,white);
    }
    else{
        SDL_FillRect(surface,&display->turn_rect,black);
    }
    SDL_BlitSurface(textSurface,NULL,surface,&text_Rect);
    //data rect
    //display_data->turn[0]='X';
    // display_data->turn_data_rect = (SDL_Rect){display->turn_rect.x+display->turn_rect.w,250,30,30};
    // SDL_Surface* textureSurface = TTF_RenderText_Blended(font,display_data->turn,Black);
    // SDL_Rect text_Data_rect = (SDL_Rect){display_data->turn_data_rect.x+5,turn_height+5,20,textureSurface->h};
    // SDL_FillRect(surface,&display_data->turn_data_rect,white);
    // SDL_BlitSurface(textureSurface,NULL,surface,&text_Data_rect);
    //display_data->is_gameover=1;
    if(display_data->is_gameover){
        after_gameover(display,display_data,font_paly,font_win,surface);
    }

}
void clear_turn(SDL_Surface* surface,Screen* display){
    SDL_FillRect(surface,&display->turn_rect,black);
}
void fill_rect(SDL_Surface* surface,Screen* display){
    SDL_FillRect(surface,&display->winner_rect,black);
    SDL_FillRect(surface,&display->play_again,black);
}
void gameover(cell* board,data* display_data,SDL_Surface* surface){
    if(display_data->is_playagain==1){
        board_init(board,surface);
        display_data->is_gameover=0;
        display_data->is_playagain=0;
    }
}


int main(){
    printf("Hello world");
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("TIC-TAC-TOE",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface,NULL,black);
    
    TTF_Font* font = TTF_OpenFont("Arial.ttf",18);
    if (!font) {
        printf("Font error: %s\n", TTF_GetError());
        return 1;
    }
    TTF_Font* font_playagain = TTF_OpenFont("Arial.ttf",30);
    if (!font_playagain) {
        printf("Font error: %s\n", TTF_GetError());
        return 1;
    }
    TTF_Font* font_win = TTF_OpenFont("Arial.ttf",35);
    if (!font_win) {
        printf("Font error: %s\n", TTF_GetError());
        return 1;
    }
    cell* board = calloc(9,sizeof(cell));
    Screen *display = calloc(1,sizeof(Screen));
    data *display_data = calloc(1,sizeof(data));
    display_data->turn[0]='X';
    display_data->turn[1]='\0';
    board_init(board,surface);
    
    int is_running =1;
    SDL_Event event;
    
    while(is_running){
        
        if(display_data->is_playagain==1){
            fill_rect(surface,display);
        }
        while(SDL_PollEvent(&event)){
            int mx=0;
            int my=0;
            int mx_click=0;
            int my_click=0;
            if(event.type == SDL_QUIT){
                is_running=0;
            }
            if(event.type == SDL_MOUSEMOTION ){
                mx=event.motion.x;
                my=event.motion.y;
            }
            if(event.type == SDL_MOUSEBUTTONUP ){
                mx = event.button.x;
                my=event.button.y;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                mx_click = event.button.x;
                my_click=event.button.y;
                if(display_data->is_gameover==1 && display_data->is_playagain==0){
                    display_data->is_playagain = isHover(mx_click,my_click,&display->play_again);
                }
            }
            
            for(int i=0;i<9;i++){
                board[i].is_hover= isHover(mx,my,&board[i].cell_rect);
                board[i].is_click=isHover(mx_click,my_click,&board[i].cell_rect);
            }
            if(display_data->is_gameover==0){
                input(board,display_data,surface,font);
            }
            
            screen_init(display,display_data,font,font_playagain,font_win,surface);
        }
        // SDL_FillRect(surface,NULL,black);
        
        color_hover(surface,board);
        logic(board,display_data);
        show_input(board,surface,font);
        if(display_data->is_playagain==1){
            gameover(board,display_data,surface);
        }
        SDL_UpdateWindowSurface(window);
        SDL_Delay(140);
    }

    SDL_DestroyWindow(window);
    return 0;
}
