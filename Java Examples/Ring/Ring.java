/***********************
Swathi Dochibhotla
Chapter 4: Ring - Class
January 2016
***********************/

import javax.swing.JComponent;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Color;
import java.awt.Component;
import java.awt.*;
import java.applet.*;
import javax.swing.*;

public class Ring
{
   public int diameter;
   public Color col;
   public int xCoor;      
   public int yCoor;

   //default
   public Ring()
   {          
      col = Color.BLACK;
      diameter = 100;
      xCoor = 0;
      yCoor = 0; 
   }
   
   //constructors
   public Ring(Color c, int d, int x, int y)
   {
      c = col; 
      d = diameter;
      x = xCoor;
      y = yCoor;
   }
   
   //draw method
   public void draw(Graphics g)
   { 
      Graphics2D g2d = (Graphics2D) g;
      g.setColor(col);
      g.drawOval(xCoor, yCoor, diameter, diameter);
   }

}