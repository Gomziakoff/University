import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        String text = "Яблоко,Груша,Банан,Апельсин,Киви";
        Scanner scanner = new Scanner(System.in);
        System.out.print("Введите разделитель: ");
        String delimiter = scanner.nextLine();
        String[] words = text.split(delimiter);
        System.out.println("Результат разделения:");
        for (String word : words) {
            System.out.println(word);
        }
        scanner.close();
    }
}
