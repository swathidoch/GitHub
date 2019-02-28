/************************
Swathi Dochibhotla
November 2015
Chapter 4: Account v2.0
************************/

import java.text.NumberFormat;

public class AccountTester
{
   private NumberFormat fmt = NumberFormat.getCurrencyInstance();
   
   private final double RATE = 0.035;
   
   private int acctNumber;
   private double balance;
   private String name;
   
	//Sets up an account defining the owner, account number, and initial balance
   public AccountTester(String owner, int account, double intial)
   {
      name = owner;
      acctNumber = account;
      balance = intial;
   }
	
   //overload
   public static String overload(String owner, int account)
   {
      return owner;
      
   }
	//Validates the transaction, deposits the specified amount, and returns the new balance
   public double deposit(double amount)
   {
      if(amount<0)
      {
         System.out.println();
         System.out.println("Error: Deposit amount is invalid."); 
         System.out.println(acctNumber + " " + fmt.format(amount));
      }
      else
         balance = balance + amount; 
      return balance; 
   	
   }
	
	//Validates the transaction, withdraws, the specified amount, and returns the new balance
   public double withdraw(double amount, double fee)
   {
      amount += fee;
   	
      if(amount < 0)
      {
         System.out.println();
         System.out.println("Error: Withdraw amount is invalid.");
         System.out.println("Account: " + acctNumber);
         System.out.println("Requested: " + fmt.format(amount));
      }
      else
         if(amount > balance)
         {
            System.out.println(); 
            System.out.println("Error: Insufficeint funds.");
            System.out.println("Account: " + acctNumber);
            System.out.println("Requested: " + fmt.format(amount));
            System.out.println("Available: " + fmt.format(balance)); 
         	 
         }
         else
            balance = balance - amount;
      return balance;
   
   }
   
   //Validates the transaction.  If sufficient funds exist,withdraws the specified from the “from” account and 
   //deposits it into the “to” account returning true,
   //otherwise does nothing and returns false.
   //@param amount
   //@param fee
   //@param from
   //@param to
   //@return if there were sufficient funds in the “from” 
   //account to make the transfer
   public static boolean transfer(double amount, double fee, Account from, Account to)
   {
      amount += fee; 
      
      if(amount < from.getBalance())
      {
         System.out.println(); 
         System.out.println("Suffiencient Funds.");
         System.out.println("Account: " + from.getAccountNumber()); 
         System.out.println("Requested: " + amount);  
         System.out.println(to.deposit(amount));
         System.out.println(to.getBalance());
      }
      else 
         return false;
      return true;   
   }
   
	//Adds interest to the account and returns the new balance.
   public double addInterest()
   {
      balance += (balance * RATE);
      return balance;
   }
	
	//Returns the current balance of the account.
   public double getBalance()
   {
      return balance; 
   }
	
	//Returns the account number
   public int getAccountNumber()
   {
      return acctNumber;
   }
	
	//Returns a one-line description of the account as a string.
   public String toString()
   {
      return(acctNumber + "\t" + name + "\t" + fmt.format(balance));
   }
}
