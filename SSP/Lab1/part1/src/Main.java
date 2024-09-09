import java.util.Random;

public class Main {
    public static void main(String[] args) {
            String months[] = {
                    "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
                    "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"
            };

            double values[] = new double[12];
            Random random = new Random();

            double sum = 0;
            for (int i = 0;i < 12; i++){
                values[i] = random.nextDouble() * 100;
                System.out.printf("%s: %.2f%n", months[i], values[i]);
                sum += values[i];
            }

            double average = sum/12;
            System.out.printf("Среднее значение: %f",average);
    }
}
