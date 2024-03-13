import csv
import string
import random as rand
import pprint


def random_string():
    return ''.join(rand.choices(string.ascii_letters, k=rand.randint(5, 10)))


class Table:
    def __init__(self, key_field, fields):
        self.key_field = key_field
        self.fields = fields
        self.table = []

    def add_record(self, values):
        self.table.append(values)

    def create_records(self, n, nums=True):
        for i in range(n):
            self.add_record(
                [str(i + 1) if nums else random_string(), *[random_string() for _ in range(len(self.fields))]])

    def remove_record(self, key_value):
        for i in range(len(self.table)):
            if self.table[i][0] == key_value:
                self.table.pop(i)
                break

    def change_record(self, key_value, new_values):
        for i in range(len(self.table)):
            if self.table[i][0] == key_value:
                self.table[i] = new_values

    def search_record(self, key_value):
        for record in self.table:
            if record[0] == key_value:
                return record
        return None

    def write_csv(self, filename):
        with open(filename, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow([self.key_field]+self.fields)
            for record in self.table:
                writer.writerow(record)

    def read_csv(self, filename):
        with open(filename, 'r', newline='') as csvfile:
            reader = csv.reader(csvfile)
            self.key_field, *self.fields = next(reader)
            self.table = []
            for row in reader:
                self.table.append(row)

    def show_records(self):
        print(self.key_field,' '.join(map(str, self.fields)))
        print('_'*len(self.key_field+' '.join(map(str, self.fields))))
        for record in self.table:
            print(f"{record}")


if __name__ == '__main__':
    n = 35
    table = Table('key',['first','second','third'])
    table.create_records(n,False)
    table.write_csv('table.csv')
    table.show_records()



