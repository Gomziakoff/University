import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class Main {
    public static void main(String[] args) {

        String[] dates = {
                "01/09/2022", "14/02/2023", "25/12/2021", "01/01/2024", "30/07/2022",
                "05/11/2023", "21/06/2022", "10/10/2023", "22/08/2021", "31/12/2023"
        };

        SimpleDateFormat inputFormat = new SimpleDateFormat("dd/MM/yyyy");
        SimpleDateFormat outputFormat = new SimpleDateFormat("dd MMMM yyyy", new Locale("ru"));

        for (String dateString : dates) {
            try {
                Date date = inputFormat.parse(dateString);
                String formattedDate = outputFormat.format(date);
                System.out.println(formattedDate);
            } catch (ParseException e) {
                System.out.println("Ошибка: неверный формат даты - " + dateString);
            }
        }
    }
}
