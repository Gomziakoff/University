import javax.swing.*;
import javax.swing.table.DefaultTableModel; // Для работы с таблицей
import java.io.BufferedWriter; // Для записи данных в файл
import java.io.File; // Для работы с файлами
import java.io.FileWriter; // Для записи в файл
import java.io.IOException;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.*;
import java.util.Vector;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class ClinicVisitsApp extends JFrame {
    private JTextField patientNameField, visitDateField, doctorNameField, diagnosisField, ageField;
    private JComboBox<String> isInsuredComboBox;
    private JTextArea resultArea;
    private JTable table;
    private JButton addButton, deleteButton, editButton, searchButton, sortButton, saveButton;

    private Connection connection;
    private Statement statement;

    public ClinicVisitsApp() {
        super("Учет посещений поликлиники");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);

        // Инициализация компонентов
        patientNameField = new JTextField(20);
        visitDateField = new JTextField(20);
        doctorNameField = new JTextField(20);
        diagnosisField = new JTextField(20);
        ageField = new JTextField(20);
        isInsuredComboBox = new JComboBox<>(new String[]{"Да", "Нет"});

        addButton = new JButton("Добавить");
        deleteButton = new JButton("Удалить");
        editButton = new JButton("Редактировать");
        searchButton = new JButton("Поиск");
        sortButton = new JButton("Сортировать");
        saveButton = new JButton("Сохранить в файл");

        resultArea = new JTextArea();
        table = new JTable();

        // Размещение компонентов на форме
        JPanel inputPanel = new JPanel(new GridLayout(6, 2));
        inputPanel.add(new JLabel("Имя пациента:"));
        inputPanel.add(patientNameField);
        inputPanel.add(new JLabel("Дата посещения (ДД-ММ-ГГГГ):"));
        inputPanel.add(visitDateField);
        inputPanel.add(new JLabel("Имя врача:"));
        inputPanel.add(doctorNameField);
        inputPanel.add(new JLabel("Диагноз:"));
        inputPanel.add(diagnosisField);
        inputPanel.add(new JLabel("Возраст пациента:"));
        inputPanel.add(ageField);
        inputPanel.add(new JLabel("Наличие страховки:"));
        inputPanel.add(isInsuredComboBox);

        JPanel buttonPanel = new JPanel();
        buttonPanel.add(addButton);
        buttonPanel.add(deleteButton);
        buttonPanel.add(editButton);
        buttonPanel.add(searchButton);
        buttonPanel.add(sortButton);
        buttonPanel.add(saveButton);

        add(inputPanel, BorderLayout.NORTH);
        add(new JScrollPane(table), BorderLayout.CENTER);
        add(buttonPanel, BorderLayout.SOUTH);

        // Подключение к базе данных
        connectToDatabase();

        // Загрузка данных в таблицу
        loadData();

        // Обработчики событий
        addButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                addVisit();
            }
        });

        deleteButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                deleteVisit();
            }
        });

        editButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                editVisit();
            }
        });

        searchButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                searchVisits();
            }
        });

        sortButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                sortVisits();
            }
        });

        saveButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                saveToFile();
            }
        });
    }

    private void connectToDatabase() {
        try {
            Class.forName("org.sqlite.JDBC");
            connection = DriverManager.getConnection("jdbc:sqlite:clinic.db");
            statement = connection.createStatement();
            statement.execute("CREATE TABLE IF NOT EXISTS Visits (id INTEGER PRIMARY KEY AUTOINCREMENT, patient_name TEXT, visit_date DATE, doctor_name TEXT, diagnosis TEXT, is_insured BOOLEAN, age INTEGER)");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void loadData() {
        try {
            ResultSet rs = statement.executeQuery("SELECT * FROM Visits");
            Vector<Vector<String>> data = new Vector<>();
            Vector<String> columnNames = new Vector<>();
            columnNames.add("ID");
            columnNames.add("Имя пациента");
            columnNames.add("Дата посещения");
            columnNames.add("Имя врача");
            columnNames.add("Диагноз");
            columnNames.add("Возраст");
            columnNames.add("Наличие страховки");

            while (rs.next()) {
                Vector<String> row = new Vector<>();
                row.add(rs.getString("id"));
                row.add(rs.getString("patient_name"));
                row.add(rs.getString("visit_date"));
                row.add(rs.getString("doctor_name"));
                row.add(rs.getString("diagnosis"));
                row.add(rs.getString("age"));
                row.add(rs.getBoolean("is_insured") ? "Да" : "Нет");
                data.add(row);
            }

            table.setModel(new DefaultTableModel(data, columnNames));
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private void addVisit() {
        String patientName = patientNameField.getText();
        String visitDate = visitDateField.getText();
        String doctorName = doctorNameField.getText();
        String diagnosis = diagnosisField.getText();
        String ageText = ageField.getText();
        boolean isInsured = isInsuredComboBox.getSelectedItem().equals("Да");

        // Проверка корректности данных
        if (!isValidDate(visitDate)) {
            JOptionPane.showMessageDialog(this, "Некорректный формат даты! Используйте ДД-ММ-ГГГГ.", "Ошибка", JOptionPane.ERROR_MESSAGE);
            return;
        }
        if (!isValidAge(ageText)) {
            JOptionPane.showMessageDialog(this, "Некорректный возраст! Введите число от 0 до 120.", "Ошибка", JOptionPane.ERROR_MESSAGE);
            return;
        }

        int age = Integer.parseInt(ageText);

        try {
            PreparedStatement ps = connection.prepareStatement("INSERT INTO Visits (patient_name, visit_date, doctor_name, diagnosis, is_insured, age) VALUES (?, ?, ?, ?, ?, ?)");
            ps.setString(1, patientName);
            ps.setString(2, visitDate);
            ps.setString(3, doctorName);
            ps.setString(4, diagnosis);
            ps.setBoolean(5, isInsured);
            ps.setInt(6, age);
            ps.executeUpdate();
            loadData();
            clearFields();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private void clearFields() {
        patientNameField.setText("");
        visitDateField.setText("");
        doctorNameField.setText("");
        diagnosisField.setText("");
        ageField.setText("");
    }

    private boolean isValidDate(String date) {
        // Указываем формат даты (ДД-ММ-ГГГГ)
        SimpleDateFormat dateFormat = new SimpleDateFormat("dd-MM-yyyy");
        dateFormat.setLenient(false); // Запрещаем "гибкий" парсинг (например, 32 января)

        try {
            // Пытаемся преобразовать строку в дату
            Date parsedDate = dateFormat.parse(date);
            return true; // Если успешно, дата корректна
        } catch (ParseException e) {
            return false; // Если возникает исключение, дата некорректна
        }
    }

    private boolean isValidAge(String ageText) {
        try {
            int age = Integer.parseInt(ageText);
            return age >= 0 && age <= 120;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    private void deleteVisit() {
        int selectedRow = table.getSelectedRow();
        if (selectedRow >= 0) {
            int id = Integer.parseInt(table.getValueAt(selectedRow, 0).toString());
            try {
                PreparedStatement ps = connection.prepareStatement("DELETE FROM Visits WHERE id = ?");
                ps.setInt(1, id);
                ps.executeUpdate();
                loadData();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    private void editVisit() {
        int selectedRow = table.getSelectedRow();
        if (selectedRow >= 0) {
            int id = Integer.parseInt(table.getValueAt(selectedRow, 0).toString());
            String patientName = patientNameField.getText();
            String visitDate = visitDateField.getText();
            String doctorName = doctorNameField.getText();
            String diagnosis = diagnosisField.getText();
            boolean isInsured = isInsuredComboBox.getSelectedItem().equals("Да");

            try {
                PreparedStatement ps = connection.prepareStatement("UPDATE Visits SET patient_name = ?, visit_date = ?, doctor_name = ?, diagnosis = ?, is_insured = ? WHERE id = ?");
                ps.setString(1, patientName);
                ps.setString(2, visitDate);
                ps.setString(3, doctorName);
                ps.setString(4, diagnosis);
                ps.setBoolean(5, isInsured);
                ps.setInt(6, id);
                ps.executeUpdate();
                loadData();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    private void searchVisits() {
        String searchTerm = JOptionPane.showInputDialog(this, "Введите имя пациента или дату посещения:");
        try {
            PreparedStatement ps = connection.prepareStatement("SELECT * FROM Visits WHERE patient_name LIKE ? OR visit_date LIKE ?");
            ps.setString(1, "%" + searchTerm + "%");
            ps.setString(2, "%" + searchTerm + "%");
            ResultSet rs = ps.executeQuery();

            Vector<Vector<String>> data = new Vector<>();
            Vector<String> columnNames = new Vector<>();
            columnNames.add("ID");
            columnNames.add("Имя пациента");
            columnNames.add("Дата посещения");
            columnNames.add("Имя врача");
            columnNames.add("Диагноз");
            columnNames.add("Наличие страховки");

            while (rs.next()) {
                Vector<String> row = new Vector<>();
                row.add(rs.getString("id"));
                row.add(rs.getString("patient_name"));
                row.add(rs.getString("visit_date"));
                row.add(rs.getString("doctor_name"));
                row.add(rs.getString("diagnosis"));
                row.add(rs.getBoolean("is_insured") ? "Да" : "Нет");
                data.add(row);
            }

            table.setModel(new DefaultTableModel(data, columnNames));
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private void sortVisits() {
        String[] options = {"Имя пациента", "Дата посещения"};
        String selectedOption = (String) JOptionPane.showInputDialog(this, "Сортировать по:", "Сортировка", JOptionPane.QUESTION_MESSAGE, null, options, options[0]);

        if (selectedOption != null) {
            String orderBy = selectedOption.equals("Имя пациента") ? "patient_name" : "visit_date";
            try {
                ResultSet rs = statement.executeQuery("SELECT * FROM Visits ORDER BY " + orderBy);
                Vector<Vector<String>> data = new Vector<>();
                Vector<String> columnNames = new Vector<>();
                columnNames.add("ID");
                columnNames.add("Имя пациента");
                columnNames.add("Дата посещения");
                columnNames.add("Имя врача");
                columnNames.add("Диагноз");
                columnNames.add("Наличие страховки");

                while (rs.next()) {
                    Vector<String> row = new Vector<>();
                    row.add(rs.getString("id"));
                    row.add(rs.getString("patient_name"));
                    row.add(rs.getString("visit_date"));
                    row.add(rs.getString("doctor_name"));
                    row.add(rs.getString("diagnosis"));
                    row.add(rs.getBoolean("is_insured") ? "Да" : "Нет");
                    data.add(row);
                }

                table.setModel(new DefaultTableModel(data, columnNames));
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    private void saveToFile() {
        JFileChooser fileChooser = new JFileChooser();
        // Устанавливаем фильтр для выбора только текстовых файлов
        fileChooser.setFileFilter(new javax.swing.filechooser.FileNameExtensionFilter("Text Files (*.txt)", "txt"));
        fileChooser.setFileFilter(new javax.swing.filechooser.FileNameExtensionFilter("CSV Files (*.csv)", "csv"));

        // Показываем диалог сохранения файла
        int userSelection = fileChooser.showSaveDialog(this);

        if (userSelection == JFileChooser.APPROVE_OPTION) {
            // Получаем выбранный файл
            File fileToSave = fileChooser.getSelectedFile();
            String filePath = fileToSave.getAbsolutePath();

            // Добавляем расширение, если его нет
            if (!filePath.endsWith(".txt") && !filePath.endsWith(".csv")) {
                filePath += ".csv"; // По умолчанию сохраняем в формате CSV
                fileToSave = new File(filePath);
            }

            // Сохраняем данные в файл
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileToSave))) {
                // Получаем модель таблицы
                DefaultTableModel model = (DefaultTableModel) table.getModel();

                // Записываем заголовки столбцов
                for (int i = 0; i < model.getColumnCount(); i++) {
                    writer.write(model.getColumnName(i));
                    if (i < model.getColumnCount() - 1) {
                        writer.write(","); // Разделитель для CSV
                    }
                }
                writer.newLine();

                // Записываем данные из таблицы
                for (int row = 0; row < model.getRowCount(); row++) {
                    for (int col = 0; col < model.getColumnCount(); col++) {
                        writer.write(model.getValueAt(row, col).toString());
                        if (col < model.getColumnCount() - 1) {
                            writer.write(","); // Разделитель для CSV
                        }
                    }
                    writer.newLine();
                }

                JOptionPane.showMessageDialog(this, "Данные успешно сохранены в файл: " + filePath, "Успех", JOptionPane.INFORMATION_MESSAGE);
            } catch (IOException e) {
                JOptionPane.showMessageDialog(this, "Ошибка при сохранении файла: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new ClinicVisitsApp().setVisible(true);
            }
        });
    }
}