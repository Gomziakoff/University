class HashTable:
    def __init__(self, size=32):
        self.size = size
        self.table = [[] for _ in range(size)]
        self.collisions = 0

    def hash_function(self, key):
        if not key:
            return 0
        return ord(key[0].lower()) % self.size

    def insert(self, key):
        index = self.hash_function(key)
        bucket = self.table[index]
        if bucket:
            self.collisions += 1
        if key not in bucket:
            bucket.append(key)

    def search(self, key):
        index = self.hash_function(key)
        bucket = self.table[index]
        return key in bucket

    def load_from_file(self, filename):
        try:
            with open(filename, 'r', encoding='utf-8') as file:
                for line in file:
                    identifier = line.strip()
                    if identifier and len(identifier) <= 32:
                        self.insert(identifier)
                    elif identifier:
                        print(f"Пропущен идентификатор '{identifier}': длина превышает 32 символа")
        except FileNotFoundError:
            print(f"Ошибка: файл '{filename}' не найден")
            return False
        except Exception as e:
            print(f"Ошибка при чтении файла: {e}")
            return False
        return True

    def display_stats(self):
        total_identifiers = sum(len(bucket) for bucket in self.table)
        empty_buckets = sum(1 for bucket in self.table if not bucket)
        print("\n=== СТАТИСТИКА ХЕШ-ТАБЛИЦЫ ===")
        print(f"Общий размер таблицы: {self.size}")
        print(f"Количество идентификаторов: {total_identifiers}")
        print(f"Количество коллизий: {self.collisions}")
        print(f"Пустых ячеек: {empty_buckets}")
        print(f"Коэффициент заполнения: {total_identifiers / self.size:.2f}")
        print("\nРаспределение элементов по ячейкам:")
        for i, bucket in enumerate(self.table):
            if bucket:
                print(f"Ячейка {i}: {len(bucket)} элементов")

    def display_table(self, max_buckets=10):
        print("\n=== СОДЕРЖИМОЕ ХЕШ-ТАБЛИЦЫ ===")
        count = 0
        for i, bucket in enumerate(self.table):
            if bucket:
                print(f"Ячейка {i} (хэш: {i}): {bucket}")
                count += 1
                if count >= max_buckets:
                    print("... (вывод ограничен)")
                    break

def main():
    print("Программа для работы с хеш-таблицей методом цепочек")
    print("Хеш-функция: код первой буквы идентификатора")
    hash_table = HashTable()
    filename = input("Введите имя файла с идентификаторами: ")
    if not hash_table.load_from_file(filename):
        return
    hash_table.display_stats()
    while True:
        print("\n" + "=" * 50)
        print("1 - Поиск идентификатора")
        print("2 - Показать статистику")
        print("3 - Показать содержимое таблицы")
        print("4 - Выход")
        choice = input("Выберите действие: ").strip()
        if choice == '1':
            identifier = input("Введите идентификатор для поиска: ").strip()
            if len(identifier) > 32:
                print("Ошибка: идентификатор слишком длинный (макс. 32 символа)")
                continue
            if hash_table.search(identifier):
                hash_value = hash_table.hash_function(identifier)
                print(f"Идентификатор '{identifier}' НАЙДЕН в ячейке {hash_value}")
            else:
                print(f"Идентификатор '{identifier}' НЕ НАЙДЕН")
        elif choice == '2':
            hash_table.display_stats()
        elif choice == '3':
            hash_table.display_table()
        elif choice == '4':
            print("Выход из программы")
            break
        else:
            print("Неверный выбор. Попробуйте снова.")
if __name__ == "__main__":
    main()