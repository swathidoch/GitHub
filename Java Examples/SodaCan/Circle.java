/***************************
Swathi Dochibhotla
Chapter 4: Soda Can - Circle
January 2016
****************************/
import java.lang.Math;

public class Circle
{
   private double diameter;
   
   public Circle()
   {
      diameter = 1;
   }
   
   public Circle(double userDiam)//for when user inputs their own diameter
   {
      this.diameter = userDiam;
   }
   
   public double getArea()
   {
      double area = Math.PI * Math.pow((diameter/2),2);
      return area;
   }
   
   public double getCircumference()
   {
      double circumference = 2* Math.PI *(diameter/2); //2piR
      return circumference;
   }
   
   public String toString() 
   {
      return "Diameter: " + diameter + "\tArea: " + this.getArea() + "\tCircumference: " + this.getCircumference();
   }

}