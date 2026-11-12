public class CustomerTest {

    public static void main(String args[]) {
    
        Customer c = new Customer(0, 3);
        c.serve();
        c.serve();
        c.serve();
        if(c.isDone()) {
             System.out.println("The customer is done, just as expected!");
          } else {
             System.out.println("The customer is not done, but should be...");
         }
    }

}