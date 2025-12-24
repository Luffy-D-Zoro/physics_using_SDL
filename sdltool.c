#include<stdio.h>
#include<SDL.h>
#include <SDL_ttf.h>
#include<math.h>
#define height 600
#define width 960
#define color1 0xffffffff
#define color2 0x00000000
typedef struct 
{
    double x,y;
    double r;
}Circle;
typedef struct {
    double x_velocity;
    double y_velocity;
    double damping;
    double gravity;
    double energy_loss;
    double firction_coeff;
}data_motion;
void friction(Circle* circle,data_motion * motion){
    //ground
    int sign;
    if(motion->x_velocity==0){
        sign=1;
    }
    else{
        sign = fabs(motion->x_velocity)/motion->x_velocity;
    }
        if(circle->y+circle->r>=height){
        motion->x_velocity -= sign*motion->firction_coeff*motion->gravity;
        // motion->x_velocity=fabs(motion->x_velocity);
    }
}
void boundary(Circle* circle,data_motion* motion){
    if(circle->y+circle->r >height){
        circle->y = height - circle->r;   
        motion->y_velocity *= motion->energy_loss;      

        if (fabs(motion->y_velocity) < 0.5)
            motion->y_velocity = 0;
    }
    //top
    if (circle->y - circle->r < 0) {
        circle->y = circle->r;
        motion->y_velocity *= motion->energy_loss;
    }
    //right
    if (circle->x + circle->r > width) {
        circle->x = width - circle->r;
        motion->x_velocity *=motion->energy_loss;
    }
    //left
     if (circle->x - circle->r < 0) {
        circle->x = circle->r;
        motion->x_velocity *= motion->energy_loss;
    }


}
// void motion_handler(Circle* circle,data_motion* motion){
//     boundary(circle,motion);
//     circle->x+= motion->x_velocity*motion->damping*16;
//     circle->y+= motion->y_velocity*motion->damping*16;
//     motion->y_velocity+=motion->gravity;
// }
void motion_handler(Circle* circle, data_motion* motion)
{
     motion->y_velocity += motion->gravity;
    
    circle->x += motion->x_velocity*8 ;
    circle->y += motion->y_velocity*8 ;
    
    boundary(circle, motion);
    friction(circle,motion);
    
    
    motion->x_velocity *= motion->damping;
    motion->y_velocity *= motion->damping;
   
}

void fill_Circle(SDL_Surface* surface,Circle circle , uint32_t color){
    double radius_sq = pow(circle.r,2);
    for(int i=circle.x-circle.r;i<=circle.x+circle.r;i++){
        for(int j=circle.y-circle.r;j<=circle.y+circle.r;j++){
            double distance = pow(circle.x-i,2)+pow(circle.y-j,2);
            if(distance<=radius_sq){
                SDL_Rect rect = {i,j,1,1};
                SDL_FillRect(surface,&rect,color);
            }
        }
    }
}
int main(){
    printf("Helllo world");
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window  =  SDL_CreateWindow("Sample Gui",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    SDL_FillRect(surface,NULL,color1);
    
    SDL_UpdateWindowSurface(window);
    //objects
//   typedef struct {
//     double x_velocity;
//     double y_velocity;
//     double damping;
//     double gravity;
//     double energy_loss;
//     double firction_coeff;
// }data_motion;
    Circle circle ={200,400,50};
    data_motion motion ={0,0,0.9999,0.2,-0.8,0.1};
    
    //main 
    SDL_Event event;
    int is_Running=1;
    int is_dragging=0;
    uint32_t cur_time;
    uint32_t prev_time;
    double curr_x;
    double curr_y;
    double prev_x;
    double prev_y;
    while(is_Running){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                is_Running=0;
            }
            // if(event.type==SDL_MOUSEMOTION && event.motion.state==SDL_BUTTON_LEFT){
                // circle.x=event.motion.x;
                // circle.y=event.motion.y;
            // }
            if(event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT){
                is_dragging=1;
                prev_time=SDL_GetTicks();
                prev_x=event.button.x;
                prev_y=event.button.y;
            }
            if(is_dragging && event.type==SDL_MOUSEMOTION ){
                //retrieving data
                cur_time=SDL_GetTicks();
                curr_x=event.motion.x;
                curr_y=event.motion.y;
                //storing current phase
                uint32_t delta_time = cur_time-prev_time;
                if(delta_time==0){
                    delta_time=1;
                }
                double delta_x = curr_x-prev_x;
                double delta_y = curr_y-prev_y;
                motion.x_velocity=delta_x/delta_time;
                motion.y_velocity=delta_y/delta_time;
                //next loop setters
                circle.x=event.motion.x;
                circle.y=event.motion.y;
                prev_x=curr_x;
                prev_y=curr_y;
                prev_time=cur_time;
            }
            if (event.type == SDL_MOUSEBUTTONUP &&
                event.button.button == SDL_BUTTON_LEFT) {
                is_dragging = 0;
            }
            
        }
        if(is_dragging==0){
            // circle.x +=(motion.x_velocity)*(motion.damping)*16;
            // circle.y +=(motion.y_velocity)*(motion.damping)*16;
            // motion.y_velocity+=motion.gravity;
            motion_handler(&circle,&motion);
        }
        SDL_FillRect(surface,NULL,color1);
        //SDL_UpdateWindowSurface(window);
        fill_Circle(surface,circle,color2);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(16);

    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}