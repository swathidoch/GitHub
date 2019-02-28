/***************************
Swathi Dochibhotla
Chapter 4: Soda Can - Driver
January 2016
****************************/

public class SodaCanDriver
{
   public static void main(String args[])
   {
      //create 3 circles of diam: 1, 10, & 13.42
      Circle circle1 = new Circle();   
      Circle circle2 = new Circle(10.0);
      Circle circle3 = new Circle(13.42);
      
      //print info for circles
      System.out.println(circle1);  
      System.out.println(circle2);
      System.out.println(circle3);
      
   }
}