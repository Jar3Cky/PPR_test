import java.util.ArrayList;
import java.util.List;

public class Ksiazka {
    public int nrBiblioteczny;
    public String autor;
    public String tytul;
    public String dataWydania;
    public String tresc;
    public String wlasciciel;

    public boolean czyZablokowana = false;
    public List<String> aktualniCzytelnicy = new ArrayList<>();

    public Ksiazka(int nr, String autor, String tytul, String dataWydania, String tresc, String wlasciciel) {
        this.nrBiblioteczny = nr;
        this.autor = autor;
        this.tytul = tytul;
        this.dataWydania = dataWydania;
        this.tresc = tresc;
        this.wlasciciel = wlasciciel;
    }

    @Override
    public String toString() {
        return String.format("ID: %d | '%s' autorstwa %s (Wydano: %s) | Właściciel: %s | Zablokowana: %b | Czytelnicy: %s", 
                nrBiblioteczny, tytul, autor, dataWydania, wlasciciel, czyZablokowana, aktualniCzytelnicy);
    }
}
