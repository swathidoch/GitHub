/***********************
Swathi Dochibhotla
Chapter 4: Employee
December 2015
***********************/

import java.text.DecimalFormat;
import java.util.Scanner;
import java.text.NumberFormat;

public class Employee
{
   private String name;
   private double salary; 
 
   public Employee(String name, double salary)
   {
      this.name = name;
      checkSalary(salary);
   }
   public String getName() 
   {
      return name; 
   }
   public void setName(String name)
   {
      this.name = name;
   } 
   public double getSalary()
   {
      return salary; 
   } 
   public void raiseSalary(double byPercent)
   {
      double raise = salary * byPercent/100;
      salary += raise;
      checkSalary(salary);
   }
   public String toString()
   {
      NumberFormat money = NumberFormat.getCurrencyInstance();
      return(name + "   " + money.format(salary));
   }
   private void checkSalary(double salary)
   {
      this.salary = salary;
   }
}
