import java.util.Random;

public class Main {
    public static void main(String[] args) {
        int N = 10;
        String consonants = "BCDFGHJKLMNPQRSTVWXYZ";
        Random random = new Random();
        StringBuilder result = new StringBuilder();
        for (int i = 0; i < N; i++) {
            char letter = consonants.charAt(random.nextInt(consonants.length()));
            result.append(letter);
        }
        System.out.println("Случайная последовательность: " + result.toString());
    }
}
