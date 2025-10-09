import java.io.*;
import java.util.*;

public class Main{
    public static boolean isSorted(List<Integer> numbers, boolean ascending) {
        for (int i = 1; i < numbers.size(); i++) {
            if (ascending) {
                if (numbers.get(i) < numbers.get(i - 1)) {
                    return false;
                }
            } else {
                if (numbers.get(i) > numbers.get(i - 1)) {
                    return false;
                }
            }
        }
        return true;
    }

    public static void generateRandomNumbers(String outputFileName, int quantity) {
        Random random = new Random();

        try {
            BufferedWriter writer = new BufferedWriter(new FileWriter(outputFileName));

            for (int i = 0; i < quantity; i++) {
                int randomNumber = random.nextInt(1001); // Генерация числа от 0 до 1000000 включительно
                writer.write(Integer.toString(randomNumber));
                writer.newLine(); // Переход на следующую строку
            }

            writer.close();
            System.out.println("Файл " + outputFileName + " успешно заполнен " + quantity + " случайными числами.");

        } catch (IOException e) {
            System.out.println("Ошибка при записи в файл: " + e.getMessage());
        }
    }

    public static void main(String[] args){
        String inputFileName = "input.txt";
        String outputFileName = "output.txt";

        try {
            //generateRandomNumbers("input.txt",1000000);
            BufferedReader reader = new BufferedReader(new FileReader(inputFileName));
            List<Integer> numbers = new ArrayList<>();
            String sortOrder = reader.readLine().trim();
            String line;
            while ((line = reader.readLine()) != null) {
                numbers.add(Integer.parseInt(line));
            }
            reader.close();

            if (sortOrder.equalsIgnoreCase("UP")){
                Collections.sort(numbers);
            } else if (sortOrder.equalsIgnoreCase("DOWN")) {
                numbers.sort(Collections.reverseOrder());
            }

            BufferedWriter writer = new BufferedWriter(new FileWriter(outputFileName));
            for (int number : numbers){
                writer.write(Integer.toString(number));
                writer.newLine();
            }
            writer.close();
            System.out.println("Sort ended");

            boolean sortedCorrectly = isSorted(numbers, sortOrder.equalsIgnoreCase("UP"));
            if (sortedCorrectly) {
                System.out.println("Массив отсортирован правильно.");
            } else {
                System.out.println("Ошибка: массив отсортирован неправильно.");
            }

        } catch (IOException e){
            System.out.println("Error:" + e.getMessage());
        }


    }
}