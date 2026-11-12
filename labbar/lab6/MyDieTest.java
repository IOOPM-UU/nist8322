import java.util.Scanner;

public class MyDieTest {

    public static void main(String [] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Number of sides: ");
        int sides = sc.nextInt();

        Die myDie = new Die(sides); 
        
        int sum = 0;
        
        for (int i = 0; i < 10; i++) {
            int currentRoll = myDie.roll();
            sum += currentRoll;
        }
        System.out.println("Summan av 10 slag blev: " + sum);
        sc.close();
    } 

}