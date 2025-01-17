//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 450

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
            {
                // seed pseudorandom number generator
                srand48(time(NULL));

                // instantiate window
                GWindow window = newGWindow(WIDTH, HEIGHT);

                // instantiate bricks
                initBricks(window);

                // instantiate ball, centered in middle of window
                GOval ball = initBall(window);

                // instantiate paddle, centered at bottom of window
                GRect paddle = initPaddle(window);

                // instantiate scoreboard, centered in middle of window, just above ball
                GLabel label = initScoreboard(window);

                // number of bricks initially
                int bricks = COLS * ROWS;

                // number of lives initially
                int lives = LIVES;

                // number of points initially
                int points = 0;

                // keep playing until game over
                
               double velocity = drand48() + 2.0;
               double vel=2.0;                 
               while (true)
                {
                      GEvent event1 = getNextEvent(MOUSE_EVENT);
                      if (event1 != NULL)
                      {
                            // if the event was movement
                            if (getEventType(event1) == MOUSE_CLICKED)
                            {
                            setLocation(ball,215, 333);
                            
                             while (lives > 0 && bricks > 0)
                            {            
                                  GEvent event = getNextEvent(MOUSE_EVENT);
                                  if(event!=NULL)
                                  {
                                         if (getEventType(event) == MOUSE_MOVED)
                                            {
                                                double x = getX(event)-35 ;
                                                setLocation(paddle, x, 517);
                                            }
                        
                                    }                 
                                    move(ball, velocity, vel);  
                                                                       
                                    if (getX(ball) + getWidth(ball) >= getWidth(window))
                                    velocity = -velocity;
                                    
                                    else if (getX(ball) <= 0)
                                    velocity = -velocity;
                                    
                                    else if(getY(ball)<=0)
                                    vel= -vel;
                                    
                                    else if (getY(ball) + getHeight(ball) >= getHeight(window))
                                    {            
                                         lives--;
                                         break;
                                    } 
                                    
                                    GObject object=detectCollision(window,ball); 
                                                                                
                                    if(object == paddle)
                                    vel= -vel;
                                    
                                    else if((strcmp(getType(object),"GRect")==0)&&(object != paddle))
                                    {
                                        vel=-vel;
                                        removeGWindow(window,object);
                                        bricks--;
                                        points++;
                                        updateScoreboard(window,label,points);
                                    }
                                    pause(10);                                    
                                }                                   
                            }             
                       } 
                       
                       if(lives==0)
                       break;
                       
                       }               
                       // wait for click before exiting
                       waitForClick();

                       // game over
                       closeGWindow(window);
                
                       return 0;

                       }

 /**
 * Initializes window with a grid of bricks.
 */
 
void initBricks(GWindow window)
            {
                int i=0,j=0,c=0;
                
                for(i=27;i<102;i+=15)
                {
                    c++;
                    for(j=2;j<=450;j+=45)
                    {
                        GRect rect=newGRect(0,0,40,10);
                        setFilled(rect,true);
                        
                        if(c==1)
                        {
                            setColor(rect,"RED");
                            setFilled(rect,true);                        
                        }
                        
                        else if(c==2)
                        {
                            setColor(rect,"ORANGE");
                            setFilled(rect,true);
                        }
                        
                        else if(c==3)
                        {
                            setColor(rect,"YELLOW");
                            setFilled(rect,true);                        
                        }
                        
                        else if(c==4)
                        {
                            setColor(rect,"GREEN");
                            setFilled(rect,true);
                        }
                        
                        else if(c==5)
                        {
                            setColor(rect,"BLUE");
                            setFilled(rect,true);
                        }
                        
                        setLocation(rect,j, i);
                        add(window,rect);                        
                        }
                    }
                }

/**
 * Instantiates ball in center of window.  Returns ball.
*/

GOval initBall(GWindow window)
            {
                GOval circle = newGOval(0, 0, 25, 25);                
                setColor(circle,"BLACK");
                setFilled(circle,true);
                add(window,circle);
                setLocation(circle,215, 333);
                return circle;
            }

/**
 * Instantiates paddle in bottom-middle of window.
*/

GRect initPaddle(GWindow window)
            {
                GRect rect=newGRect(70,10,70,10);
                setColor(rect,"BLACK");
                setFilled(rect,true);
                add(window,rect);
                setLocation(rect,190, 517);
                return rect;
            }

/**
 * Instantiates, configures, and returns label for scoreboard.
*/
GLabel initScoreboard(GWindow window)
            {
                double x,y;
                GLabel label=newGLabel("0");
                setFont(label,"SansSerif-72");
                x=(getWidth(window)-getWidth(label))/2;
                y=(getHeight(window)-getHeight(label))/2;
                setLocation(label,x,y);
                add(window,label);
                return label;
            }

/**
 * Updates scoreboard's label, keeping it centered in window.
*/

void updateScoreboard(GWindow window, GLabel label, int points)
            {
                // update label
                char s[12];
                sprintf(s, "%i", points);
                setLabel(label, s);

                // center label in window
                double x = (getWidth(window) - getWidth(label)) / 2;
                double y = (getHeight(window) - getHeight(label)) / 2;
                setLocation(label, x, y);
            }

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
*/
GObject detectCollision(GWindow window, GOval ball)
            {
                // ball's location
                double x = getX(ball);
                double y = getY(ball);

                // for checking for collisions
                GObject object;

                // check for collision at ball's top-left corner
                object = getGObjectAt(window, x, y);
                
                if (object != NULL)
                {
                    return object;
                }

                // check for collision at ball's top-right corner
                object = getGObjectAt(window, x + 2 * RADIUS, y);
                
                if (object != NULL)
                {
                    return object;
                }

                // check for collision at ball's bottom-left corner
                object = getGObjectAt(window, x, y + 2 * RADIUS);
                
                if (object != NULL)
                {
                    return object;
                }

                // check for collision at ball's bottom-right corner
                object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
                
                if (object != NULL)
                {
                    return object;
                }

                // no collision
                return NULL;
            }
