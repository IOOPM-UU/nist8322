public class Customer {
    private int bornTime;
    private int groceries;
    
    public Customer(int bTime, int groc) {
        this.bornTime = bTime;
        this.groceries = groc;
    }

    public int getBornTime() {
    return this.bornTime;
}
    
    public void serve() {
        this.groceries--;
    }
    
    public boolean isDone() {
        if (this.groceries == 0){
            return true;
        }
        return false;
    }
}