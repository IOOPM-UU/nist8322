import java.util.Scanner;

public class Die {
  private int numberOfSides;
  private int value;

  public Die() {
      this.numberOfSides = 6;
      this.value = 1;
  }

  public Die(int numberOfSides) {
    this.value = 1;
    if (numberOfSides < 1) {
        throw new IllegalArgumentException(numberOfSides + " is an illegal number of sides for a die!");
    }
    this.numberOfSides = numberOfSides;
  }

  public int roll() {
    this.value = (int) (Math.random() * numberOfSides) + 1;
    return this.get();
  }

  public int get() {
      return value;
  }

  public String toString() {
    return "Die(" + value + ")";
    }

  public boolean equals(Die otherDie) {
    return this.value == otherDie.value && this.numberOfSides == otherDie.numberOfSides;
    }       
  public static void main(String [] args) {
      Scanner sc = new Scanner(System.in);
      System.out.print("Number of sides: ");
      int sides = sc.nextInt();
      Die d = new Die(sides);
      System.out.println("You rolled a: " + d.roll());
      sc.close();
  }
}
