/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.rozprosz_test;

/**
 *
 * @author Jarek
 */
public class Car {
    private final String vin;
    private final String brand;

    public Car(String vin, String brand) {
        this.vin = vin;
        this.brand = brand;
    }

    public String getBrand() {
        return brand;
    }

    @Override
    public String toString() {
        return brand + " [VIN: " + vin + "]";
    }
}