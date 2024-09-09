public class Main {
    public static void main(String[] args) {
        String text = "Текст текст текст текст";
        String vowels = "аеёиоуыэюяАЕЁИОУЫЭЮЯ";
        int vowelCount = 0;
        int spaceCount = 0;
        int letterCount = 0;
        for (int i = 0; i < text.length(); i++) {
            char c = text.charAt(i);
            if (Character.isLetter(c)) {
                letterCount++;
                if (vowels.indexOf(c) != -1) {
                    vowelCount++;
                }
            }
            if (Character.isWhitespace(c)) {
                spaceCount++;
            }
        }
        System.out.println("Количество гласных: " + vowelCount);
        System.out.println("Количество пробелов: " + spaceCount);
        System.out.println("Общее количество букв: " + letterCount);
    }
}
