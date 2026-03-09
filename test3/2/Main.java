public class Main {

    public static void main(String[] args) {
       Salon salon = new Salon(5); // Salon na 5 aut

        // Tworzymy producentów (każdy innej marki)
        Thread p1 = new Thread(new Producer(salon, "Toyota"));
        Thread p2 = new Thread(new Producer(salon, "BMW"));

        // Tworzymy konsumentów
        Thread c1 = new Thread(new Consumer(salon, "Jan Kowalski"));
        Thread c2 = new Thread(new Consumer(salon, "Anna Nowak"));

        p1.start();
        p2.start();
        c1.start();
        c2.start();
    }
}
