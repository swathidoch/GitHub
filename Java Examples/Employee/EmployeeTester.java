/***********************
Swathi Dochibhotla
Chapter 4: Employee Tester
December 2015
***********************/

public class EmployeeTester
{
   public static void main(String[] args)
   {
      Employee divya = new Employee("Hacker, Divya", 70000);
      divya.raiseSalary(10);	  // Divya gets a 10% raise
      System.out.println(divya);
   }
}