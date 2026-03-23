import java.io.*;
import java.net.*;
import java.util.*;


public class Serwer {
    private static List<Ksiazka> listaKsiazek = new ArrayList<>();
    private static int generatorId = 1;

    public static void main(String[] args) throws IOException {
        ServerSocket serwer = new ServerSocket(5000);
        System.out.println("=== SERWER BIBLIOTEKI URUCHOMIONY NA PORCIE 5000 ===");

        while (true) {
            Socket klient = serwer.accept();
            new Thread(() -> obsluzZgloszenie(klient)).start();
        }
    }

    private static void obsluzZgloszenie(Socket s) {
        try (BufferedReader wejscie = new BufferedReader(new InputStreamReader(s.getInputStream()));
             PrintWriter wyjscie = new PrintWriter(s.getOutputStream(), true)) {

            String zadanie;
            while ((zadanie = wejscie.readLine()) != null) {
                String[] p = zadanie.split(";");
                if (p.length < 2) continue;
                
                String komenda = p[0].toUpperCase();
                String uzytkownik = p[1];

                synchronized (listaKsiazek) {
                    wyjscie.println(procesujKomende(komenda, uzytkownik, p));
                }
            }
        } catch (Exception e) { 
            System.out.println("Klient rozłączony."); 
        }
    }

    private static String procesujKomende(String komenda, String uzytkownik, String[] p) {
        try {
            switch (komenda) {
                // a. Wstawianie książki 
                case "DODAJ": 
                    Ksiazka nowa = new Ksiazka(generatorId++, p[2], p[3], p[4], p[5], uzytkownik);
                    listaKsiazek.add(nowa);
                    System.out.println("LOG: Pisarz " + uzytkownik + " dodal ksiazke ID: " + nowa.nrBiblioteczny);
                    return "OK: Dodano ksiazke ID " + nowa.nrBiblioteczny;

                // b. Blokowanie książki
                case "BLOKUJ": 
                    Ksiazka kb = znajdz(Integer.parseInt(p[2]));
                    if (kb != null && kb.wlasciciel.equals(uzytkownik)) {
                        kb.czyZablokowana = true;
                        System.out.println("LOG: Pisarz " + uzytkownik + " zablokowal ID: " + kb.nrBiblioteczny);
                        return "OK: Ksiazka zablokowana do edycji.";
                    }
                    return "Błąd: Brak uprawnień lub książki.";

                // c. Test, czy wszyscy zwrócili
                case "TEST_ZWROTU": 
                    Ksiazka kt = znajdz(Integer.parseInt(p[2]));
                    if (kt != null) {
                        return kt.aktualniCzytelnicy.isEmpty() ? "OK: Wszyscy zwrocili." : "INFO: Nadal ktoś czyta.";
                    }
                    return "Błąd: Brak książki.";

                // d. Niszczenie książki
                case "USUN": 
                    Ksiazka ku = znajdz(Integer.parseInt(p[2]));
                    if (ku != null && ku.wlasciciel.equals(uzytkownik) && ku.aktualniCzytelnicy.isEmpty()) {
                        listaKsiazek.remove(ku);
                        System.out.println("LOG: Pisarz " + uzytkownik + " usunal ksiazke ID: " + p[2]);
                        return "OK: Usunieto ksiazke.";
                    }
                    return "Błąd: Nie można usunąć (brak uprawnień lub ktoś czyta).";

                // e. Aktualizacja książki
                case "AKTUALIZUJ": 
                    Ksiazka ka = znajdz(Integer.parseInt(p[2]));
                    if (ka != null && ka.wlasciciel.equals(uzytkownik) && ka.czyZablokowana && ka.aktualniCzytelnicy.isEmpty()) {
                        ka.tresc = p[3];
                        ka.czyZablokowana = false; // Automatyczne odblokowanie po edycji
                        System.out.println("LOG: Pisarz " + uzytkownik + " zaktualizowal ID: " + p[2]);
                        return "OK: Zaktualizowano tresc i odblokowano.";
                    }
                    return "Błąd: Musisz najpierw zablokowac ksiazke i poczekac az wszyscy zwroca.";

                // f. Szukanie wg słowa kluczowego
                case "SZUKAJ": 
                    StringBuilder wyniki = new StringBuilder("Wyniki: ");
                    for (Ksiazka k : listaKsiazek) {
                        if (k.tytul.contains(p[2]) || k.autor.contains(p[2])) {
                            wyniki.append("[").append(k.nrBiblioteczny).append("] ").append(k.tytul).append(", ");
                        }
                    }
                    return wyniki.toString();

                // g. Wypożyczenie książki 
                case "WYPOZYCZ": 
                    Ksiazka kw = znajdz(Integer.parseInt(p[2]));
                    if (kw == null) return "Błąd: Brak książki.";
                    if (kw.czyZablokowana) return "Błąd: Książka zablokowana przez pisarza.";
                    kw.aktualniCzytelnicy.add(uzytkownik);
                    System.out.println("LOG: Czytelnik " + uzytkownik + " wypozyczyl ID: " + kw.nrBiblioteczny);
                    return "TRESC: " + kw.tresc;

                // h. Oddanie książki
                case "ODDAJ": 
                    Ksiazka ko = znajdz(Integer.parseInt(p[2]));
                    if (ko != null) {
                        ko.aktualniCzytelnicy.remove(uzytkownik);
                        System.out.println("LOG: Czytelnik " + uzytkownik + " oddal ID: " + ko.nrBiblioteczny);
                        return "OK: Zwrocono.";
                    }
                    return "Błąd: Nie znaleziono.";

                // Ogólny raport
                case "RAPORT": 
                    StringBuilder raport = new StringBuilder("\n--- RAPORT STATUSU BIBLIOTEKI ---\n");
                    for (Ksiazka k : listaKsiazek) raport.append(k.toString()).append("\n");
                    return raport.toString() + "[KONIEC]";
            }
        } catch (Exception e) { return "Błąd formatu komendy."; }
        return "Błąd: Nieznana komenda.";
    }

    private static Ksiazka znajdz(int id) {
        for (Ksiazka k : listaKsiazek) if (k.nrBiblioteczny == id) return k;
        return null;
    }
}
