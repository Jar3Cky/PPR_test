/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

package com.mycompany.rozprosz_test;

/**
 *
 * @author Jarek
 */
public class Main {

    public static void main(String[] args) {
       Salon salon = new Salon(3);

        // Tworzymy producentów różnych marek
        Thread p1 = new Thread(new Producer(salon, "Toyota"));
        Thread p2 = new Thread(new Producer(salon, "BMW"));

        // Tworzymy konsumentów z konkretnymi oczekiwaniami
        Thread c1 = new Thread(new Consumer(salon, "Jan (Fan Toyoty)", "Toyota"));
        Thread c2 = new Thread(new Consumer(salon, "Anna (Fan BMW)", "BMW"));
        Thread c3 = new Thread(new Consumer(salon, "Marek (Fan Toyoty)", "Toyota"));

        p1.start();
        p2.start();
        c1.start();
        c2.start();
        c3.start();
    }
}
