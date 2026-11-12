public class PairOfDice {
    private Die die1;
    private Die die2;

    public PairOfDice(int numberOfSides) {
        this.die1 = new Die(numberOfSides);
        this.die2 = new Die(numberOfSides);
    }

    public int roll() {
        return this.die1.roll() + this.die2.roll();
    }

    public int getDie1Value() {
        return this.die1.get();
    }

    public int getDie2Value() {
        return this.die2.get();
    }

    public String toString() {
        return "PairOfDice: " + this.die1.toString() + " and " + this.die2.toString();
    }
    public static void main(String[] args) {
    // Skapa ett par sexsidiga tärningar
    PairOfDice pair = new PairOfDice(6);
    
    // Skriv ut paret för att testa din toString()
    System.out.println("Nyskapade tärningar: " + pair);
    
    // Slå paret och spara summan
    int sum = pair.roll();
    System.out.println("Du rullade totalt: " + sum);
    
    // Skriv ut paret igen för att se att de nya värdena sparades
    System.out.println("Efter kastet: " + pair);
}

}