public class Queue {
    
    private class Node {
        Customer element;
        Node next;
        
        public Node(Customer c){
            this.element = c;
            this.next = null;
        }
    }
    
    private Node first;
    private Node last;
    private int length;
    
    public Queue () {
        this.first = null;
        this.last = null;
        this.length = 0;
    }
    
    public int length() {
        return this.length;
    }
    
    public void enqueue(Customer c) {
        Node newNode = new Node(c);
        if (this.first == null) {
            this.first = newNode;
            this.last = newNode;
        } else {
            this.last.next = newNode;
            this.last = newNode;
        }
        this.length++;
    }
    
    public Customer dequeue() {
        if(this.first == null) {
            throw new EmptyQueueException();
        }
        
        Customer c = this.first.element;
        this.first = this.first.next;
        
        if(this.first == null) {
            this.last = null;
        }
        
        this.length--;
        return c;
    }
    
    public Customer first() {
        if(this.first == null) {
            throw new EmptyQueueException();
        }
        return this.first.element;
    }
}