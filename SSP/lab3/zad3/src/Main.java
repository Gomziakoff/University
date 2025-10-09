import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

public class Main extends JFrame {

    private DefaultListModel<String> listModel;
    private JList<String> list;
    private DefaultListModel<String> resultListModel;
    private JList<String> resultList;
    private JButton processButton;

    public Main() {
        setTitle("Multi-Select App");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(500, 300);
        setLayout(new BorderLayout());

        listModel = new DefaultListModel<>();
        list = new JList<>(listModel);
        list.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        for (int i = 1; i <= 20; i++) {
            listModel.addElement("Item " + i);
        }
        JScrollPane listScrollPane = new JScrollPane(list);

        processButton = new JButton("Обработать выбор");
        processButton.addActionListener(new ProcessButtonActionListener());

        resultListModel = new DefaultListModel<>();
        resultList = new JList<>(resultListModel);
        JScrollPane resultListScrollPane = new JScrollPane(resultList);

        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new GridLayout(1, 2));
        mainPanel.add(listScrollPane);
        mainPanel.add(resultListScrollPane);

        add(mainPanel, BorderLayout.CENTER);
        add(processButton, BorderLayout.SOUTH);
    }

    private class ProcessButtonActionListener implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            List<String> selectedItems = list.getSelectedValuesList();
            StringBuilder sb = new StringBuilder();

            for (String item : selectedItems) {
                sb.append(item).append(", ");
            }

            if (sb.length() > 2) {
                sb.setLength(sb.length() - 2);
            }

            String resultString = sb.toString();

            if (resultString.length() > 100) {
                JOptionPane.showMessageDialog(Main.this,
                        "Суммарное количество символов превышает 100!",
                        "Предупреждение",
                        JOptionPane.WARNING_MESSAGE);
            }

            resultListModel.addElement(resultString);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            Main app = new Main();
            app.setVisible(true);
        });
    }
}
