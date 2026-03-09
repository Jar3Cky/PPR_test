class Producer implements Runnable {
    private final Salon salon;
    private final String brand;
    private int count = 1;

    public Producer(Salon salon, String brand) {
        this.salon = salon;
        this.brand = brand;
    }

    @Override
    public void run() {
        try {
            while (true) {
                String vin = brand + "-" + System.currentTimeMillis() + "-" + count++;
                Car car = new Car(vin, brand);
                salon.put(car, brand + "_Factory");
                Thread.sleep(2000); // Symulacja czasu produkcji
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

class Consumer implements Runnable {
    private final Salon salon;
    private final String name;

    public Consumer(Salon salon, String name) {
        this.salon = salon;
        this.name = name;
    }

    @Override
    public void run() {
        try {
            while (true) {
                salon.take(name);
                Thread.sleep(3000); // Klient zagląda co 3 sekundy
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}