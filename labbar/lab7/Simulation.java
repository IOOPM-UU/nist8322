import java.util.Random;


public class Simulation {

    private Store store;
    private int time;
    private int intensity;
    private int maxGroceries;
    private int thresholdForNewRegister;
    
    // statistiken
    private int totalWaitTime;
    private int customersServed;
    
    private Random random = new Random();

    
    public Simulation(int registers, int intensity, int maxGroceries, int threshold) {
        this.store = new Store(registers); 
        this.time = 0;
        this.intensity = intensity;
        this.maxGroceries = maxGroceries;
        this.thresholdForNewRegister = threshold;

        this.totalWaitTime = 0;
        this.customersServed = 0;
    }

    public void step() {
        this.time++;
        this.store.step();

        int slump = this.random.nextInt(100);
        
        if (slump < this.intensity) {
            int slumpadeVaror = 1 + this.random.nextInt(this.maxGroceries);
            Customer nyKund = new Customer(this.time, slumpadeVaror);
            this.store.newCustomer(nyKund);
        }
        
        if (this.store.getAverageQueueLength() > this.thresholdForNewRegister) {
            this.store.openNewRegister();
        }
    
        Customer[] done = this.store.getDoneCustomers();
        for (int i = 0; i < done.length; i++) {
            this.customersServed++;
            int vantetid = this.time - done[i].getBornTime();
            this.totalWaitTime += vantetid;
        }
    }
    
    
    public int getCustomersServed() {
        return this.customersServed;
    }

    
    public int getAverageWaitTime() {
        if (this.customersServed == 0) {
            return 0;
        }
        return this.totalWaitTime / this.customersServed;
    }
    
    public String toString() {
        return "Varuhuset live: \n" +
               "Tid (steg): " + this.time + "\n" +
               "Färdiga kunder: " + this.customersServed + "\n" +
               "Snittväntetid: " + this.getAverageWaitTime() + " tidssteg\n";
    }
}