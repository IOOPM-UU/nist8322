public class Store {
    private Register[] registers;

    public Store(int numberOfRegisters) {
        this.registers = new Register[numberOfRegisters];

        for (int i = 0; i < this.registers.length; i++) {
            this.registers[i] = new Register();
        }

        this.registers[0].open();
    }

    public double getAverageQueueLength() {
        int totalLength = 0;
        
        for(Register r : this.registers) {
            totalLength = totalLength + r.getQueueLength();
        }
        
        return (double) totalLength / this.registers.length;
    }

    void newCustomer(Customer c) {
        Register shortestReg = this.registers[0];

        for(int i = 1; i < this.registers.length; i++) {
            if(this.registers[i].isOpen()) {
        
                if (shortestReg.getQueueLength() > this.registers[i].getQueueLength()) {
                    shortestReg = this.registers[i]; 
                }
            }
        }

        shortestReg.addToQueue(c);
    }
    
    public void step() {
    
        for (int i = 0; i < this.registers.length; i++) {
        
            if (this.registers[i].isOpen()) {
                this.registers[i].step();
            }
        }
    }

    

    public void openNewRegister() {
        for (int i = 0; i < this.registers.length; i++) {
            if (!this.registers[i].isOpen()) {
                this.registers[i].open(); 
                break;
            }
        }
    }

    public Customer[] getDoneCustomers() {
        int doneCount = 0;
        for (int i = 0; i < this.registers.length; i++) {

            if (this.registers[i].isOpen() && this.registers[i].currentCustomerIsDone()) {
                doneCount++;
            }
        }

        Customer[] doneArray = new Customer[doneCount];

        int index = 0;
        for (int i = 0; i < this.registers.length; i++) {
            if (this.registers[i].isOpen() && this.registers[i].currentCustomerIsDone()) {

                doneArray[index] = this.registers[i].removeCurrentCustomer();
                index++;
            }
        }
        return doneArray;
        }
    }
