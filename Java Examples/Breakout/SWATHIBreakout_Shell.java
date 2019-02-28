import acm.util.*;
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.awt.event.KeyEvent;
import javax.sound.sampled.*;
import java.io.*;
import java.util.Random;

public class SWATHIBreakout_Shell extends GraphicsProgram
{
  	// Display settings   
   private static final int WIDTH = 690;           // width of game display 
   private static final int HEIGHT = 600;          // height of game display
   private static final int HEIGHT_OFFSET = 17;
   
   //Life settings
   private static final int HEART_WIDTH = 25;
   private static final int HEART_HEIGHT = 22;
   private static final int HEART_SEP = 2;

	// Paddle settings
   private static final int PADDLE_WIDTH = 60;     // width of paddle
   private static final int PADDLE_HEIGHT = 10;    // height of paddle
   private static final int PADDLE_X_OFFSET = 292;
   private static final int PADDLE_Y_OFFSET = 60;  // offset of paddle up from the bottom
	
  	// Brick settings
   private static final int NBRICKS_PER_ROW = 34;  // number of bricks per row
   private static final int NBRICK_ROWS = 15;      // number of rows of bricks
   private static final int BRICK_SEP = 4;         // separation between bricks both horizontally and vertically
   private static final int BRICK_WIDTH = 16;   // width of each brick (based on the display dimensions)
   private static final int BRICK_HEIGHT = 20;      // height of brick
   private static final double BRICK_X_OFFSET = 2*BRICK_SEP + PADDLE_X_OFFSET;
   private static final int BRICK_Y_OFFSET = 70;   // offset of the top brick row from top

	// Ball settings
   private static final double BALL_RADIUS = 10;
   private RandomGenerator rgen = new RandomGenerator();       // radius of ball in pixels
   
   // Game settings
   private static final int NTURNS = 3;            // number of turns
   
	// Game Play variables
                              // paddle - based on ACM GRect object
                              // ball - based on ACM GOval object
   private double dx, dy;                          // ball displacement in both directions (x-direction, and y-direction)
   private int lives = NTURNS;                              // number of lives left in game
   private int nBricks = NBRICKS_PER_ROW * NBRICK_ROWS;  // total number of bricks on game board at start of game
   private int points;                             // number of points scored
   private int toggle = 0;                     // used for mouse events (only moves the paddle every 5th mouse move)
   private GImage paddle;
   private GOval ball;
   private GImage life1, life2, life3;
   private AudioClip theme;
   private AudioClip death;
  
   // Label variables
   private GLabel lblPoints = new GLabel("POINTS: " + points, PADDLE_X_OFFSET, 598);  
   
   // Background Variables
   private GImage Bg;
   

   
   
   public static void main(String[] args)       // main method -- called when the program is run
     {
      String[] sizeArgs = { "width=" + 1280, "height=" + 640 };
      new SWATHIBreakout_Shell().start(sizeArgs);
     }
   
   public void Bg()
     {
      Bg = new GImage("DK_bg_resized.jpg");
      add(Bg);
     }
   
   
	
   public void init()                          	// init method -- automatically called on startup
     {
      Bg();
      createBricks();                           // create the bricks
      createPaddle();                           // create the paddle
      createBall();
      addMouseListeners();
      addKeyListeners();    
      lblPoints.setColor(Color.WHITE); 
      add(lblPoints);                                         
     }
   
   public void run()                            // run method -- automatically called after init
     {
      startTheBall();
      playBall();
     }
		
  public void createBricks()                   // createBricks method -- called from the init method
    {
   	//make the bricks
      for(int r = 0; r < 10/* how many rows of bricks */; r++)
       {
         for(int c = 0; c < 34/* how many bricks in each row */; c++)
          {
            Brick brick = new Brick( "DK_Banana_test.png",
                                     BRICK_X_OFFSET+c *(BRICK_WIDTH+BRICK_SEP)/* x arg */, 
               						    HEIGHT_OFFSET+BRICK_SEP+r *(BRICK_HEIGHT+BRICK_SEP)/* y arg */);
            
            add(brick);
          }
       }
    }

   class Brick extends GImage                    // Brick class -- class for all brick objects
      {
      /** Constructor: a banana at (x, y) */
      public Brick(String i, double x, double y)
         {
           super(i,x,y);
         }
      }  

   public void createPaddle()                   // createPaddle method -- called from the init method
   {
      paddle = new GImage("DK_Barrel_resized.png", WIDTH/2 + PADDLE_X_OFFSET, 520);
      add(paddle);
   }
   



   public void createBall()                     // createBall method -- called from the init method
   {
      ball = new GOval(PADDLE_X_OFFSET + WIDTH/2,HEIGHT/2 - BALL_RADIUS, BALL_RADIUS, BALL_RADIUS);
      ball.setFilled(true);
      ball.setFillColor(new Color(205,133,63));
      ball.setColor(Color.GREEN.brighter());
      add(ball);
   }
		
   public void startTheBall()                   // startTheBall method -- called from the run method
   {
         
      dx = rgen.nextDouble(0.1, 0.3);
      if(!rgen.nextBoolean(0.5));
        {
         dx=-dx;
        }
      dy = 0.35;
         	
   }
	
   private GObject getCollidingObject()            // getCollidingObject -- called from the playBall method
   {  // discovers and returns the object that the ball collided with
   
      if(getElementAt(ball.getX(), ball.getY()) != null)
         return getElementAt(ball.getX(), ball.getY());
      else if(getElementAt(ball.getX()+BALL_RADIUS*2, ball.getY()) != null)
         return getElementAt(ball.getX()+BALL_RADIUS*2, ball.getY());
      else if(getElementAt(ball.getX()+BALL_RADIUS*2, ball.getY()+BALL_RADIUS*2) != null)
         return getElementAt(ball.getX()+BALL_RADIUS*2, ball.getY()+BALL_RADIUS*2);
      else if(getElementAt(ball.getX(), ball.getY()+BALL_RADIUS*2) != null)
         return getElementAt(ball.getX(), ball.getY()+BALL_RADIUS*2);
      else	
         return null;
   }
  
   
   
   public void playBall()           	      // playBall method -- called from the run method
   {
      AudioClip theme = MediaTools.loadAudioClip("theme.wav");
      theme.play();
      theme.loop();
      
      GImage life1 = new GImage("Heart.png", PADDLE_X_OFFSET + WIDTH - HEART_SEP - HEART_WIDTH, HEIGHT - HEART_SEP);
      add(life1);
      GImage life2 = new GImage("Heart.png", PADDLE_X_OFFSET + WIDTH - 2*HEART_SEP - 2*HEART_WIDTH, HEIGHT - HEART_SEP);
      add(life2);
      GImage life3 = new GImage("Heart.png", PADDLE_X_OFFSET + WIDTH - 3*HEART_SEP - 3*HEART_WIDTH, HEIGHT - HEART_SEP);
      add(life3);
      GLabel MessageBox = new GLabel(" ", WIDTH/2 + PADDLE_X_OFFSET -30, 400);
      GLabel MessageBox2 = new GLabel(" ", WIDTH/2 + PADDLE_X_OFFSET-30, 425);

      while( lives == 3)                     // game loop - play continues as long as player has lives left
       {
        
         ball.move(dx, dy);
      	//pause
         pause(1);
      	
      	//check for contact along the outer walls
         
         
         if(ball.getY() <= HEIGHT_OFFSET + BALL_RADIUS)
           {
            dy= -dy;
           }
         else if(ball.getX()>= WIDTH + PADDLE_X_OFFSET)
           {
            dx=-dx;
           }
         else if(ball.getX()<= PADDLE_X_OFFSET + BALL_RADIUS)
           {
            dx=-dx;
           }
         else if(ball.getY() >= HEIGHT)
           {
          ball.setLocation(WIDTH/2 + PADDLE_X_OFFSET,HEIGHT/2-BALL_RADIUS);
          lives--;
          MessageBox.setLabel("Click To Continue");
          MessageBox.setColor(Color.WHITE);
          add(MessageBox);
          waitForClick();
          remove(MessageBox);
           }
         else
           {
         //check for collisions with bricks or paddle
            GObject collider = getCollidingObject();
         
         //if the ball collided with the paddle 
            if(collider == paddle)
              {
            //reverse the y velocity
               dy = -1.01*dy;
               if( dy >= 1.14 )
               {
                  dy = 1.0;
               }
              }
            //otherwise if the ball collided with a brick
            else if(collider instanceof Brick) 
              {
            //reverse y velocity
               points++;
               lblPoints.setLabel("POINTS:" + points);
               dy = -dy;
                
            //remove the brick
               remove(collider);
              }
           }     
       }
      
      if(lives == 2)
      {
         remove(life3);
      }
         
      

      
     while( lives == 2)                     // game loop - play continues as long as player has lives left
      {
        
         ball.move(dx, dy);
      	//pause
         pause(1);
      	
      	//check for contact along the outer walls
         
         
         if(ball.getY() <= HEIGHT_OFFSET + BALL_RADIUS)
         {
            dy= -dy;
         }
         else if(ball.getX()>= WIDTH + PADDLE_X_OFFSET)
         {
            dx=-dx;
         }
         else if(ball.getX()<= PADDLE_X_OFFSET + BALL_RADIUS)
         {
            dx=-dx;
         }
         else if(ball.getY() >= HEIGHT)
         {
          
          ball.setLocation(WIDTH/2 + PADDLE_X_OFFSET,HEIGHT/2-BALL_RADIUS);
          lives--;
          MessageBox.setLabel("Click To Continue");
          MessageBox.setColor(Color.WHITE);
          add(MessageBox);
          waitForClick();
          remove(MessageBox);
         }
         else
         {
         //check for collisions with bricks or paddle
            GObject collider = getCollidingObject();
         
         //if the ball collided with the paddle 
            if(collider == paddle)
            {
            //reverse the y velocity
               dy = -1.02*dy;
               if( dy >= 1.14 )
               {
                  dy = 1.0;
               }
            }
            //otherwise if the ball collided with a brick
            else if(collider instanceof Brick) 
            {
            //reverse y velocity
               points++;
               lblPoints.setLabel("POINTS:" + points);
               dy = -dy;
                
            //remove the brick
               remove(collider);
             
            }
         } 
         
      if(lives == 1)
      {
         remove(life2);
      }    
         
      while( lives == 1)                     // game loop - play continues as long as player has lives left
      {
        
         ball.move(dx, dy);
      	//pause
         pause(1);
      	
      	//check for contact along the outer walls
         
         
         if(ball.getY() <= HEIGHT_OFFSET + BALL_RADIUS)
         {
            dy= -dy;
         }
         else if(ball.getX()>= WIDTH + PADDLE_X_OFFSET)
         {
            dx=-dx;
         }
         else if(ball.getX()<= PADDLE_X_OFFSET + BALL_RADIUS)
         {
            dx=-dx;
         }
         else if(ball.getY() >= HEIGHT)
         {
          ball.setLocation(WIDTH/2 + PADDLE_X_OFFSET,HEIGHT/2-BALL_RADIUS);
          lives--;
          remove(life1);
          theme.stop();
          AudioClip death = MediaTools.loadAudioClip("Death.wav");
          death.play();
          MessageBox.setLabel("Game Over!");
          MessageBox2.setLabel("Click to Exit");
          MessageBox.setColor(Color.WHITE);
          MessageBox2.setColor(Color.WHITE);
          add(MessageBox);
          add(MessageBox2);
          waitForClick();
          System.exit(1);
          
         }
         else
         {
         //check for collisions with bricks or paddle
            GObject collider = getCollidingObject();
         
         //if the ball collided with the paddle 
            if(collider == paddle)
            {
            //reverse the y velocity
               dy = -1.03*dy;
               if( dy >= 1.14 )
               {
                  dy = 1.0;
               }
            }
            //otherwise if the ball collided with a brick
            else if(collider instanceof Brick) 
            {
            //reverse y velocity
               points++;
               lblPoints.setLabel("POINTS:" + points);
               dy = -dy;
                
            //remove the brick
               remove(collider);
             
            }
            

         }     
      }
      }
      
      if(lives == 0 && nBricks == 0)
      {
      remove(ball);
      add(MessageBox);
      MessageBox.setColor(Color.WHITE);
      MessageBox.setLabel("Congratulations!");
      }


   }
	

   
   public void mouseMoved(MouseEvent e)         // mouseMoved method -- called by the mouseListener
   {  // triggered whenever the mouse is moved anywhere within the boundaries of the run window
   
   	//only move the paddle every 5th mouse event, otherwise the play slows every time the mouse moves
      if(toggle == 5 && (e.getX() >= PADDLE_X_OFFSET && e.getX() <= (PADDLE_X_OFFSET+WIDTH)))
      {
      
         paddle.setLocation(e.getX()-PADDLE_WIDTH/2, HEIGHT-PADDLE_Y_OFFSET);
      	/*******************************************************************
         *                       				  
      	* move the paddle so that it lines up with the current position 
         * of the mouse 
         *
         * make sure the paddle stops at the left
         * and right walls
      	*                      				  
      	*******************************************/
      	
      	//reset toggle to 1 
         toggle = 1;
      }
      else if(toggle == 5 && e.getX() < PADDLE_X_OFFSET)
      {
         paddle.setLocation(PADDLE_X_OFFSET, HEIGHT - PADDLE_Y_OFFSET);
         toggle = 1;
      }
      else if(toggle == 5 && e.getX() > (PADDLE_X_OFFSET + WIDTH))
      {
         paddle.setLocation(PADDLE_X_OFFSET + WIDTH - PADDLE_WIDTH/2, HEIGHT - PADDLE_Y_OFFSET);
         toggle = 1;
      }
         else
      {
      	//increment toggle by 1
      	//(when toggle gets to 5 the code will move the paddle 
      	// and reset toggle back to 1)
         toggle++;
      }
   }
}

