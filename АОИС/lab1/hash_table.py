from table import random_string
import csv


class HashTable:
    def __init__(self,size,key_field,fields):
        self.size = size
        self.key_field = key_field
        self.fields = fields
        self.table = [None]*size
        self.collisions_count = 0
        self.search_count = 0
        self.n_records = 0

    def hash_func(self,key):
        hash_code = 0
        div = self.size
        for char in key:
            hash_code = (hash_code << 1) ^ ord(char)
        hash_code %= div
        i = 0
        #self.collisions_count = 0
        while i < self.size:
            if self.table[hash_code] is None or self.table[hash_code][0] == key:
                self.search_count+=1
                break
            else:
                self.search_count += 1
                self.collisions_count += 1
                i += 1
                hash_code = (hash_code + i**2) % self.size

        return hash_code

    def add_record(self, values):
        if self.n_records == self.size:
            print("Hash table is full")
            return
        self.n_records += 1
        h = self.hash_func(values[0])
        self.table[h] = values

        self.table[h].append([self.n_records, self.collisions_count])

    def remove_record(self, key_value):
        h = self.hash_func(key_value)
        self.table[h] = None
        self.n_records -= 1

    def change_record(self, key_value, new_values):
        h = self.hash_func(key_value)
        self.table[h] = new_values

    def search_record(self, key_value):
        h = self.hash_func(key_value)
        return self.table[h]

    def write_csv(self, filename):
        with open(filename, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow([self.key_field] + self.fields)
            for record in self.table:
                if record is not None:
                    writer.writerow(record)

    def read_csv(self, filename):
        with open(filename, 'r', newline='') as csvfile:
            reader = csv.reader(csvfile)
            self.key_field, *self.fields = next(reader)
            self.table = [None] * self.size
            for row in reader:
                self.add_record(row)

    def create_records(self, n, numerical=True):
        for i in range(n):
            self.add_record([str(i + 1) if numerical else random_string(),
                             *[random_string() for _ in range(len(self.fields))]])

    def collision_probability(self):
        n = 0
        for i in range(len(self.table)):
            if self.table[i] is None:
                continue
            else:
                n += 1
        return self.collisions_count / n

    def average_search_count(self):
        total_search_count = 0
        self.search_count = 0
        for record in self.table:
            if record is not None:
                key_value = record[0]
                self.search_record(key_value)
                total_search_count += self.search_count
                self.search_count = 0
        return total_search_count / len(self.table)

    def show_records(self):
        print(self.key_field, ' '.join(map(str, self.fields)),"[num,collision]")
        print('_' * len(self.key_field + ' '.join(map(str, self.fields))+"[num,collision]"))
        for record in self.table:
            print(record)

    def get_fields(self):
        return [self.key_field,self.fields]

    def get_collisions(self):
        data = []
        for record in self.table:
            if record:
                data.append(record[-1])
        return sorted(data)

if __name__ == '__main__':
    N = 41
    hash_table = HashTable(N, 'key', ['first', 'second', 'third'])

    hash_table.read_csv('table.csv')
    hash_table.write_csv('hash_table.csv')

    print(f"collision_probability: {hash_table.collision_probability()}")
    print(f"average_search_count: {hash_table.average_search_count()}")



