   import javax.swing.JFrame;
   
    public class DriverBreakout
   {
       public static void main(String[] args)
      { 
         JFrame frame = new JFrame("Breakout");
         frame.setSize(408, 438);
         frame.setLocation(0, 0);
         frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
       	frame.setContentPane(new SWATHIBreakout_Shell());
         frame.setVisible(true);
         
      }
   }