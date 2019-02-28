/***********************
Swathi Dochibhotla
Chapter 4: Lucky Number
November 2015
***********************/

import java.util.Scanner;
import java.util.*;

public class LuckyNumber
{
   public static void main(String[] args)
   {
      //enter lucky number
      Scanner lucky = new Scanner(System.in);
      System.out.println("Enter a lucky number between 2 and 12:");
      int num = lucky.nextInt();
      while(num < 2 || num > 12)
      {
         System.out.println("Sorry. The number must between 2 and 12. Try again:");
         num = lucky.nextInt();
      }
      
      //roll 2 dice 10,000 times & count number of times sum is lucky number
      Die a = new Die();
      Die b = new Die();
      int count = 0;
      
      for(int c=0; c<10000; c++)
      {
         a.roll();
         b.roll();
         
         if(a.roll() + b.roll() == num)
         {
            count++;
         }
      }
      
      //print out number of times lucky num was rolled
      System.out.println("The Lucky Number was rolled " + count + " times.");
   }
}