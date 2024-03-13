from hash_table import HashTable
import os


def menu():
    while True:
        choice = input('Do you want to load info from file?(y/n)\n')
        if choice == 'y':
            size = input("Enter the size of hash table\n")
            while True:
                filename = input("Enter full filename\n")
                try:
                    table = HashTable(int(size),'','')
                    table.read_csv(filename)
                    key_field,fields = table.get_fields()
                    break
                except:
                    print("File not found")
                    exit_choice = input("Want again?(y/n)")
                    if exit_choice.lower() == 'n':
                        break
            break
        elif choice == 'n':
            size = input("Enter the size of hash table")
            key_field = input("Enter the name of key field\n")
            fields = input('Enter the names of the other fields (comma-separated): \n').split(',')
            table = HashTable(size, key_field, fields)
            os.system('cls')
            break
        os.system('cls')
    os.system('cls')
    while True:
        print("\n_=_=_ Main Menu _=_=_")
        print("1. Add Record")
        print("2. Remove Record")
        print("3. Update Record")
        print("4. Search Record")
        print("5. Show Table")
        print("6. Save Table")
        print("7. Exit")
        choice = input("Enter your choice\n")
        match choice.split():
            case ['1']:
                os.system('cls')
                values = [input(f"Enter value for {key_field}: \n")]
                for field in fields:
                    values.append(input(f"Enter value for {field}: \n"))
                table.add_record(values)
                os.system('cls')
            case ['2']:
                os.system('cls')
                print("Table:")
                table.show_records()
                record = None
                while not record:
                    key_value = input(f"Enter {key_field} of record to remove: \n")
                    record = table.search_record(key_value)
                    if record:
                        table.remove_record(key_value)
                    else:
                        print("Record not found.")
                        exit_choice = input("Want again?(y/n)")
                        if exit_choice.lower() == 'n':
                            break
                os.system('cls')

            case ['3']:
                os.system('cls')
                print("Table:")
                table.show_records()
                record = None
                while not record:
                    key_value = input(f"Enter {key_field} of record to update: \n")
                    record = table.search_record(key_value)
                    if record:
                        print(f"Current values for record {key_value}:")
                        print(f"{key_field}: {record[0]}")
                        for i, field in enumerate(fields):
                            print(f"{field}: {record[i + 1]}")
                        new_values = [key_value]
                        for field in fields:
                            new_values.append(input(f"Enter new value for {field}: \n"))
                        table.change_record(key_value, new_values)
                        break
                    else:
                        print("Record not found.")
                        exit_choice = input("Want again?(y/n)")
                        if exit_choice.lower() == 'n':
                            break
                os.system('cls')

            case ['4']:
                os.system('cls')
                print("Table:")
                table.show_records()
                record = None
                while not record:
                    key_value = input(f"Enter {key_field} of record to search: \n")
                    record = table.search_record(key_value)
                    if record:
                        os.system('cls')
                        print(f"Record found:")
                        print(f"{key_field}: {record[0]}")
                        for i, field in enumerate(fields):
                            print(f"{field}: {record[i + 1]}")
                    else:
                        print("Record not found.")
                        exit_choice = input("Want again?(y/n)")
                        if exit_choice.lower() == 'n':
                            break
                os.system('cls')
            case ['5']:
                os.system('cls')
                table.show_records()

            case ['6']:
                os.system('cls')
                filename = input("Enter name of save file\n")
                table.write_csv(filename)

            case ['7']:
                os.system('cls')
                print("Goodbye!")
                break
            case _:
                os.system('cls')
                print("Sorry, this command doesnt exist")


if __name__ == "__main__":
    menu()
