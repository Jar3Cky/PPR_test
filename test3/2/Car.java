public class Car {
    private final String vin;
    private final String brand;

    public Car(String vin, String brand) {
        this.vin = vin;
        this.brand = brand;
    }

    @Override
    public String toString() {
        return brand + " [VIN: " + vin + "]";
    }
}