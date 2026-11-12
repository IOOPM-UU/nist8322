import java.util.Scanner;

public class NameOrder {
    
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Name: ");
        String name1 = sc.next();
        System.out.print("Name: ");
        String name2 = sc.next();

        if (name1.compareTo(name2) < 0) {
            System.out.println("Första namnet i alfabetiskordning: " + name1); 
            System.out.println("Andra namnet: " + name2);
        } else {
            System.out.println("Första namnet i alfabetiskordning: " + name2);
            System.out.println("Andra namnet: " + name1);
        }

        sc.close();
    }
}