import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;

public class Main extends JFrame {

    private DefaultListModel<String> listModel;
    private JList<String> list;
    private JCheckBox oddCheckBox;
    private JCheckBox evenCheckBox;
    private JComboBox<String> comboBox;

    public Main() {
        setTitle("List Manager");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(400, 300);
        setLayout(new BorderLayout());

        // Инициализация списка
        listModel = new DefaultListModel<>();
        list = new JList<>(listModel);
        list.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        for (int i = 1; i <= 10; i++) {
            listModel.addElement("Item " + i);
        }
        JScrollPane listScrollPane = new JScrollPane(list);

        // Флажки для выбора четных и нечетных строк
        oddCheckBox = new JCheckBox("Выбрать нечетные строки");
        evenCheckBox = new JCheckBox("Выбрать четные строки");

        // Панель для флажков
        JPanel checkBoxPanel = new JPanel();
        checkBoxPanel.setLayout(new FlowLayout());
        checkBoxPanel.add(oddCheckBox);
        checkBoxPanel.add(evenCheckBox);

        comboBox = new JComboBox<>();
        JPanel comboBoxPanel = new JPanel();
        comboBoxPanel.setLayout(new FlowLayout());
        comboBoxPanel.add(new JLabel("Выбранные строки:"));
        comboBoxPanel.add(comboBox);

        add(listScrollPane, BorderLayout.CENTER);
        add(checkBoxPanel, BorderLayout.NORTH);
        add(comboBoxPanel, BorderLayout.SOUTH);

        oddCheckBox.addActionListener(new CheckBoxActionListener());
        evenCheckBox.addActionListener(new CheckBoxActionListener());
    }

    private class CheckBoxActionListener implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            comboBox.removeAllItems();
            list.clearSelection();

            List<String> selectedItems = new ArrayList<>();

            for (int i = 0; i < listModel.size(); i++) {
                boolean isOdd = (i % 2 == 0);
                String item = listModel.getElementAt(i);

                if (oddCheckBox.isSelected() && !isOdd) {
                    selectedItems.add(item);
                    list.addSelectionInterval(i, i);
                }

                if (evenCheckBox.isSelected() && isOdd) {
                    selectedItems.add(item);
                    list.addSelectionInterval(i, i);
                }
            }

            for (String item : selectedItems) {
                comboBox.addItem(item);
            }
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            Main app = new Main();
            app.setVisible(true);
        });
    }
}
