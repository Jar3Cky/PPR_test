/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.rozprosz_test;

/**
 *
 * @author Jarek
 */
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
        while (inventory.size() >= capacity) {
            System.out.println("[SALON] Pełny! " + producerName + " wstrzymuje produkcję.");
            wait();
        }
        inventory.add(car);
        System.out.println("[PRODUKCJA] " + producerName + " dostarczył: " + car);
        notifyAll(); // Budzimy wszystkich (klientów czekających na marki)
    }

    // Metoda dla Konsumenta (Punkt 3: Czekanie na konkretny model)
    public synchronized Car takeSpecific(String consumerName, String preferredBrand) throws InterruptedException {
        while (true) {
            Car foundCar = null;
            for (Car car : inventory) {
                if (car.getBrand().equalsIgnoreCase(preferredBrand)) {
                    foundCar = car;
                    break;
                }
            }

            if (foundCar != null) {
                inventory.remove(foundCar);
                System.out.println("[ZAKUP] " + consumerName + " kupił wymarzone: " + foundCar);
                notifyAll(); // Powiadom producentów, że jest miejsce
                return foundCar;
            }

            // Jeśli nie ma marki, klient czeka (Punkt 3)
            System.out.println("[KLIENT] " + consumerName + " czeka na markę: " + preferredBrand);
            wait();
        }
    }
}