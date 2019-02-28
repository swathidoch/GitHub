/***************************
Swathi Dochibhotla
Chapter 4: Soda Can - Tester
January 2016
****************************/

public class SodaCanTester
{
   public static void main(String args[])
   {
      //create three new SodaCans
      SodaCan sodaCan1 = new SodaCan();   
      SodaCan sodaCan2 = new SodaCan(13.20, 10.00);
      SodaCan sodaCan3 = new SodaCan(20.10, 13.42);
      
      //print SodaCans
      System.out.println(sodaCan1); 
      System.out.println(sodaCan2);
      System.out.println(sodaCan3);
   }
}