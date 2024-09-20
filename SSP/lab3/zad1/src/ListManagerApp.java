import javax.swing.*;
import java.awt.*;

public class ListManagerApp extends JFrame {
    private DefaultListModel<String> listModel1, listModel2, listModel3;
    private JList<String> list1, list2, list3;
    private JButton moveButton1, moveButton2, moveButton3, addButton, editButton, removeButton;

    public ListManagerApp() {
        setTitle("List Manager App");
        setSize(600, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        listModel1 = new DefaultListModel<>();
        listModel2 = new DefaultListModel<>();
        listModel3 = new DefaultListModel<>();

        listModel1.addElement("Item 1-1");
        listModel1.addElement("Item 1-2");
        listModel2.addElement("Item 2-1");
        listModel3.addElement("Item 3-1");

        list1 = new JList<>(listModel1);
        list2 = new JList<>(listModel2);
        list3 = new JList<>(listModel3);

        JPanel panel1 = createListPanel("List 1", list1, moveButton1 = new JButton("Move to List 2"));
        JPanel panel2 = createListPanel("List 2", list2, moveButton2 = new JButton("Move to List 3"));
        JPanel panel3 = createListPanel("List 3", list3, moveButton3 = new JButton("Move to List 1"));

        JPanel controlPanel = new JPanel();
        controlPanel.setLayout(new FlowLayout());
        addButton = new JButton("Add to List 2");
        editButton = new JButton("Edit Selected");
        removeButton = new JButton("Remove Selected");
        controlPanel.add(addButton);
        controlPanel.add(editButton);
        controlPanel.add(removeButton);

        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new GridLayout(1, 3));
        mainPanel.add(panel1);
        mainPanel.add(panel2);
        mainPanel.add(panel3);

        add(mainPanel, BorderLayout.CENTER);
        add(controlPanel, BorderLayout.SOUTH);

        moveButton1.addActionListener(e -> moveItem(list1, listModel1, listModel2));
        moveButton2.addActionListener(e -> moveItem(list2, listModel2, listModel3));
        moveButton3.addActionListener(e -> moveItem(list3, listModel3, listModel1));

        addButton.addActionListener(e -> addItemToList(listModel2));
        editButton.addActionListener(e -> editSelectedItem(list2, listModel2));
        removeButton.addActionListener(e -> removeSelectedItem(list2, listModel2));
    }

    private JPanel createListPanel(String title, JList<String> list, JButton moveButton) {
        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());
        panel.add(new JLabel(title), BorderLayout.NORTH);
        panel.add(new JScrollPane(list), BorderLayout.CENTER);
        panel.add(moveButton, BorderLayout.SOUTH);
        return panel;
    }

    private void moveItem(JList<String> fromList, DefaultListModel<String> fromModel, DefaultListModel<String> toModel) {
        int selectedIndex = fromList.getSelectedIndex();
        if (selectedIndex != -1) {
            String selectedItem = fromModel.getElementAt(selectedIndex);
            fromModel.remove(selectedIndex);
            toModel.addElement(selectedItem);
        }
    }

    private void addItemToList(DefaultListModel<String> model) {
        String newItem = JOptionPane.showInputDialog(this, "Enter new item:");
        if (newItem != null && !newItem.trim().isEmpty()) {
            model.addElement(newItem);
        }
    }

    private void editSelectedItem(JList<String> list, DefaultListModel<String> model) {
        int selectedIndex = list.getSelectedIndex();
        if (selectedIndex != -1) {
            String currentItem = model.getElementAt(selectedIndex);
            String newItem = JOptionPane.showInputDialog(this, "Edit item:", currentItem);
            if (newItem != null && !newItem.trim().isEmpty()) {
                model.setElementAt(newItem, selectedIndex);
            }
        }
    }

    private void removeSelectedItem(JList<String> list, DefaultListModel<String> model) {
        int selectedIndex = list.getSelectedIndex();
        if (selectedIndex != -1) {
            model.remove(selectedIndex);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            ListManagerApp app = new ListManagerApp();
            app.setVisible(true);
        });
    }
}
