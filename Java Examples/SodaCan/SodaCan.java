/***************************
Swathi Dochibhotla
Chapter 4: Soda Can - Class
January 2016
****************************/

public class SodaCan
{
   private double height, diameter; 
   private Circle circle;
   
   //default
   public SodaCan() 
   {  
      height = 1;
      diameter = 1;
      circle = new Circle(1);
   }
   
   //SodaCan w/ constuctors
   public SodaCan(double tempHeight, double tempDiameter) 
   { 
      this.height = tempHeight;
      this.diameter = tempDiameter;
      this.circle = new Circle(tempDiameter);
   }
  
   //SodaCan methods
   public double getVolume() {
      double volume = circle.getArea() * height;
      return volume;
   }
   
   public double getSurfaceArea() 
   {
      double surfaceArea = (2 * circle.getArea()) + (circle.getCircumference() * height);
      return surfaceArea;
   }
   
   public String toString() 
   {
      return "Diameter: " + diameter + "\tHeight: " + height + "\tVolume: " + this.getVolume() + "\tSurface Area: " + this.getSurfaceArea();
   }
}