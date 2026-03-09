/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.rozprosz_test;

/**
 *
 * @author Jarek
 */
// PRODUCENT
public class Producer implements Runnable {
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
            while (!Thread.interrupted()) {
                String vin = brand.toUpperCase() + "-" + System.currentTimeMillis() + "-" + count++;
                Car car = new Car(vin, brand);
                salon.put(car, brand + "-Factory");
                Thread.sleep(2000); // Produkcja trwa 2 sekundy
            }
        } catch (InterruptedException e) {
            System.out.println(brand + " Factory wyłączona.");
        }
    }
}