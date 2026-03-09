/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.rozprosz_test;

/**
 *
 * @author Jarek
 */
public class Consumer implements Runnable {
    private final Salon salon;
    private final String name;
    private final String preferredBrand;

    public Consumer(Salon salon, String name, String preferredBrand) {
        this.salon = salon;
        this.name = name;
        this.preferredBrand = preferredBrand;
    }

    @Override
    public void run() {
        try {
            while (!Thread.interrupted()) {
                salon.takeSpecific(name, preferredBrand);
                Thread.sleep(4000); // Klient cieszy się autem przed następnym zakupem
            }
        } catch (InterruptedException e) {
            System.out.println("Klient " + name + " zakończył zakupy.");
        }
    }
}
