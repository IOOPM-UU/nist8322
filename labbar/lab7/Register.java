public class Register {
    private boolean open;
    private Queue queue;

    public Register() {
        open = false;
        queue = new Queue();
    }

    public void open() {
        this.open = true;
    }

    public void close() {
        this.open = false;
    }

    public boolean isOpen() {
        return open;
    }

    public void step() {
        if (this.hasCustomers()) {
            this.queue.first().serve();
        }
    }

    public boolean hasCustomers() {
         return (this.queue.length() > 0);
    }

    public boolean currentCustomerIsDone() {
        if (this.queue.length() == 0) {
            return false;
        }

        return (this.queue.first().isDone());
    }

    public void addToQueue(Customer c) {
        this.queue.enqueue(c);
    }

    public Customer removeCurrentCustomer() {
        Customer c = this.queue.first();
        this.queue.dequeue();
        return c;
    }

    public int getQueueLength() {
        return (this.queue.length());
    }
}