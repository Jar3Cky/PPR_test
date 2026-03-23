import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Czytelnik {
    public static void main(String[] args) {
        String ipSerwera = "localhost"; 
        
        try (Socket gniazdo = new Socket(ipSerwera, 5000);
             PrintWriter wyjscie = new PrintWriter(gniazdo.getOutputStream(), true);
             BufferedReader wejscie = new BufferedReader(new InputStreamReader(gniazdo.getInputStream()));
             Scanner skaner = new Scanner(System.in)) {

            System.out.println("=== PANEL CZYTELNIKA ===");
            System.out.print("Podaj swoj login (Czytelnika): ");
            String login = skaner.nextLine();

            while (true) {
                System.out.println("\nUsługi Czytelnika:");
                System.out.println("1. SZUKAJ książki wg słowa kluczowego");
                System.out.println("2. WYPOŻYCZ książkę (Podaj ID)");
                System.out.println("3. ZWRÓĆ książkę (Podaj ID)");
                System.out.println("4. RAPORT STATUSU BIBLIOTEKI");
                System.out.println("0. WYJDŹ");
                System.out.print("Twój wybór: ");
                
                String wybor = skaner.nextLine();
                String komenda = "";

                switch (wybor) {
                    case "1": System.out.print("Słowo kluczowe: "); komenda = "SZUKAJ;" + login + ";" + skaner.nextLine(); break;
                    case "2": System.out.print("Podaj ID książki: "); komenda = "WYPOZYCZ;" + login + ";" + skaner.nextLine(); break;
                    case "3": System.out.print("Podaj ID książki: "); komenda = "ODDAJ;" + login + ";" + skaner.nextLine(); break;
                    case "4": komenda = "RAPORT;" + login; break;
                    case "0": System.exit(0);
                    default: System.out.println("Nieprawidłowy wybór."); continue;
                }

                wyjscie.println(komenda);

                if (wybor.equals("4")) {
                    String linia;
                    while (!(linia = wejscie.readLine()).contains("[KONIEC]")) System.out.println(linia);
                } else {
                    System.out.println("\nSerwer: " + wejscie.readLine());
                }
            }
        } catch (IOException e) { System.err.println("Błąd: " + e.getMessage()); }
    }
}
