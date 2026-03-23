import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Pisarz {
    public static void main(String[] args) {
        String ipSerwera = "localhost"; 
        
        try (Socket gniazdo = new Socket(ipSerwera, 5000);
             PrintWriter wyjscie = new PrintWriter(gniazdo.getOutputStream(), true);
             BufferedReader wejscie = new BufferedReader(new InputStreamReader(gniazdo.getInputStream()));
             Scanner skaner = new Scanner(System.in)) {

            System.out.println("=== PANEL PISARZA ===");
            System.out.print("Podaj swoj login (Pisarza): ");
            String login = skaner.nextLine();

            while (true) {
                System.out.println("\nUsługi Pisarza:");
                System.out.println("1. DODAJ książkę (Format: Autor;Tytuł;Data;Treść)");
                System.out.println("2. BLOKUJ książkę przed edycją (Format: ID_Książki)");
                System.out.println("3. TEST_ZWROTU - sprawdź czy można edytować (Format: ID_Książki)");
                System.out.println("4. AKTUALIZUJ książkę (Format: ID_Książki;NowaTreść)");
                System.out.println("5. USUN książkę (Format: ID_Książki)");
                System.out.println("6. RAPORT STATUSU BIBLIOTEKI");
                System.out.println("0. WYJDŹ");
                System.out.print("Twój wybór: ");
                
                String wybor = skaner.nextLine();
                String komenda = "";

                switch (wybor) {
                    case "1": System.out.print("Dane: "); komenda = "DODAJ;" + login + ";" + skaner.nextLine(); break;
                    case "2": System.out.print("ID: "); komenda = "BLOKUJ;" + login + ";" + skaner.nextLine(); break;
                    case "3": System.out.print("ID: "); komenda = "TEST_ZWROTU;" + login + ";" + skaner.nextLine(); break;
                    case "4": System.out.print("ID;NowaTreść: "); komenda = "AKTUALIZUJ;" + login + ";" + skaner.nextLine(); break;
                    case "5": System.out.print("ID: "); komenda = "USUN;" + login + ";" + skaner.nextLine(); break;
                    case "6": komenda = "RAPORT;" + login; break;
                    case "0": System.exit(0);
                    default: System.out.println("Nieprawidłowy wybór."); continue;
                }

                wyjscie.println(komenda);

                if (wybor.equals("6")) {
                    String linia;
                    while (!(linia = wejscie.readLine()).contains("[KONIEC]")) System.out.println(linia);
                } else {
                    System.out.println("\nSerwer: " + wejscie.readLine());
                }
            }
        } catch (IOException e) { System.err.println("Błąd: " + e.getMessage()); }
    }
}
