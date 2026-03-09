import java.util.LinkedList;
import java.util.Queue;

public class Salon {
    private final int capacity;
    private final Queue<Car> inventory = new LinkedList<>();

    public Salon(int capacity) {
        this.capacity = capacity;
    }

    // Metoda dla Producenta
    public synchronized void put(Car car, String producerName) throws InterruptedException {
        while (inventory.size() == capacity) {
            System.out.println("Salon pełny! " + producerName + " wstrzymuje produkcję.");
            wait(); // Czekaj, aż zwolni się miejsce
        }
        inventory.add(car);
        System.out.println("PRODUKCJA: " + producerName + " wstawił " + car);
        notifyAll(); // Powiadom konsumentów, że jest auto
    }

    // Metoda dla Konsumenta (na tym etapie: kupuje co jest lub wychodzi)
    public synchronized Car take(String consumerName) {
        if (inventory.isEmpty()) {
            System.out.println("KLIENT: " + consumerName + " zastał pusty salon i wychodzi.");
            return null; 
        }
        Car car = inventory.poll();
        System.out.println("ZAKUP: " + consumerName + " kupił " + car);
        notifyAll(); // Powiadom producentów, że zwolniło się miejsce
        return car;
    }
}