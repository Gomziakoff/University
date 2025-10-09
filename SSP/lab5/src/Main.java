import java.util.*;

abstract class Component {
    String name;
    Map<String, String> characteristics;

    Component(String name, Map<String, String> characteristics) {
        this.name = name;
        this.characteristics = characteristics;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(name).append(": ");
        for (Map.Entry<String, String> entry : characteristics.entrySet()) {
            sb.append(entry.getKey()).append(": ").append(entry.getValue()).append(", ");
        }
        return sb.substring(0, sb.length() - 2);
    }
}

class CPU extends Component {
    CPU(String name, String frequency, int cores) {
        super(name, new HashMap<String, String>() {{
            put("частота", frequency);
            put("ядра", String.valueOf(cores));
        }});
    }
}

class GPU extends Component {
    GPU(String name, String memory, String frequency) {
        super(name, new HashMap<String, String>() {{
            put("память", memory);
            put("частота", frequency);
        }});
    }
}

class PSU extends Component {
    PSU(String name, String power, String efficiency) {
        super(name, new HashMap<String, String>() {{
            put("мощность", power);
            put("эффективность", efficiency);
        }});
    }
}

class PC {
    List<Component> components;

    PC(List<Component> components) {
        this.components = components;
    }

    void addComponent(Component component) {
        components.add(component);
    }

    boolean removeComponent(Component component) {
        return components.remove(component);
    }


    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (Component component : components) {
            sb.append(component.toString()).append("\n");
        }
        return sb.toString();
    }
}

class Inventory {
    List<Component> components = new ArrayList<>();
    Map<Integer, PC> pcs = new HashMap<>();

    void addComponent(Component component) {
        components.add(component);
    }

    void addPC(PC pc) {
        pcs.put(pcs.size() + 1, pc);
    }

    void removeItem(String inventoryType) {
        Scanner scanner = new Scanner(System.in);
        if (inventoryType.equals("компоненты") || inventoryType.equals("1")) {
            System.out.print("Введите название компонента для удаления: ");
            String itemName = scanner.nextLine();
            components.removeIf(component -> component.name.equals(itemName));
        } else if (inventoryType.equals("пк") || inventoryType.equals("2")) {
            System.out.print("Введите индекс пк для удаления: ");
            int pcIndex = scanner.nextInt();
            pcs.remove(pcIndex);
        }
    }

    void showInventory(String choice) {
        if (choice.equals("компоненты") || choice.equals("1")) {
            System.out.println("Компоненты:");
            for (Component component : components) {
                System.out.println(component);
            }
        } else if (choice.equals("пк") || choice.equals("2")) {
            System.out.println("ПК:");
            for (Map.Entry<Integer, PC> entry : pcs.entrySet()) {
                System.out.println("Индекс: " + entry.getKey());
                System.out.println(entry.getValue());
            }
        }
    }

    PC findPcWithHighestCpuFrequency() {
        PC highestFrequencyPc = null;
        double highestFrequency = 0;
        for (PC pc : pcs.values()) {
            for (Component component : pc.components) {
                if (component instanceof CPU) {
                    // Заменяем запятую на точку и удаляем единицы измерения (например, "GHz")
                    String frequencyStr = component.characteristics.get("частота").replace(",", ".").split(" ")[0];
                    double frequency = Double.parseDouble(frequencyStr);
                    if (frequency > highestFrequency) {
                        highestFrequency = frequency;
                        highestFrequencyPc = pc;
                    }
                }
            }
        }
        return highestFrequencyPc;
    }
}

public class Main {
    static PC createPc(List<Component> components) {
        List<CPU> cpuOptions = new ArrayList<>();
        List<GPU> gpuOptions = new ArrayList<>();
        List<PSU> psuOptions = new ArrayList<>();

        for (Component component : components) {
            if (component instanceof CPU) {
                cpuOptions.add((CPU) component);
            } else if (component instanceof GPU) {
                gpuOptions.add((GPU) component);
            } else if (component instanceof PSU) {
                psuOptions.add((PSU) component);
            }
        }

        if (!cpuOptions.isEmpty() && !gpuOptions.isEmpty() && !psuOptions.isEmpty()) {
            CPU cpu = cpuOptions.get(new Random().nextInt(cpuOptions.size()));
            GPU gpu = gpuOptions.get(new Random().nextInt(gpuOptions.size()));
            PSU psu = psuOptions.get(new Random().nextInt(psuOptions.size()));
            PC pc = new PC(Arrays.asList(cpu, gpu, psu));
            components.remove(cpu);
            components.remove(gpu);
            components.remove(psu);
            return pc;
        } else {
            System.out.println("Недостаточно компонентов для сборки ПК.");
            return null;
        }
    }

    public static void main(String[] args) {
        Inventory inventory = new Inventory();
        Random random = new Random();

        for (int i = 0; i < 3; i++) {
            inventory.addComponent(new CPU("CPU_" + random.nextInt(100), String.format("%.2f GHz", 2 + random.nextDouble() * 3), 2 + random.nextInt(15)));
            inventory.addComponent(new GPU("GPU_" + random.nextInt(100), (4 + random.nextInt(9)) + " GB", (1000 + random.nextInt(1001)) + " MHz"));
            inventory.addComponent(new PSU("PSU_" + random.nextInt(100), (400 + random.nextInt(601)) + " W", (80 + random.nextInt(16)) + "%"));
        }

        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("\n1. Показать инвентарь\n2. Создать ПК\n3. Удалить предмет\n4. Найти ПК с самым высокочастотным процессором\n5. Выход");
            System.out.print("Что делаем?: ");
            String choice = scanner.nextLine();

            if (choice.equals("1")) {
                System.out.print("Введите тип предмета (компоненты/пк): ");
                String inventoryType = scanner.nextLine();
                inventory.showInventory(inventoryType);
            } else if (choice.equals("2")) {
                PC pc = createPc(inventory.components);
                if (pc != null) {
                    inventory.addPC(pc);
                    System.out.println("ПК создан.");
                    System.out.println(pc);
                }
            } else if (choice.equals("3")) {
                System.out.print("Введите тип предмета (компоненты/пк): ");
                String inventoryType = scanner.nextLine();
                inventory.removeItem(inventoryType);
            } else if (choice.equals("4")) {
                PC pcWithHighestFrequency = inventory.findPcWithHighestCpuFrequency();
                if (pcWithHighestFrequency != null) {
                    System.out.println("ПК с самым высокочастотным процессором:\n" + pcWithHighestFrequency);
                } else {
                    System.out.println("ПК не найден.");
                }
            } else if (choice.equals("5")) {
                System.out.println("Выход...");
                break;
            } else {
                System.out.println("Неверный выбор.");
            }
        }
    }
}