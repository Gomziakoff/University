import numpy as np


def word_to_binary(word):
    return ''.join(format(ord(char), '08b') for char in word)


def expand_mask(mask_short):
    mask_long = ""
    for bit in mask_short:
        mask_long += bit * (8)
    return mask_long


class AM:
    def __init__(self,size,bit_depth):
        self.bit_array = np.zeros((size,bit_depth+1),dtype=np.uint8)
        self.size = size
        self.bit_depth = bit_depth+1

    def add_word(self, word):
        binary_word = word_to_binary(word)
        word_length = len(binary_word)
        if word_length > self.bit_depth:
            raise ValueError("Слово слишком длинное для записи в строку.")

        for row in self.bit_array:
            if row[0] == 0:
                row[0] = 1
                for i in range(word_length):
                    row[i + 1] = int(binary_word[i])
                break

    def read_word(self, row):
        binary_word = ''
        for i in range(1, len(self.bit_array[0])):
            binary_word += str(self.bit_array[row, i])

        # Разделяем бинарную строку по 8 бит и преобразуем в символы
        word = ''.join(chr(int(binary_word[i:i + 8], 2)) for i in range(0, len(binary_word), 8))
        return word.rstrip('\x00')

    def search(self, word, mask=None):
        if len(word) * 8 > self.bit_depth:
            raise ValueError("Слово слишком длинное")
        address_indicator = [1] * self.size
        word = '1' + word_to_binary(word) + "0" * (self.bit_depth - len(word) * 8)
        if mask is None:
            mask = "1" * self.bit_depth
        else:
            mask = '1' + expand_mask(mask) + "0" * (self.bit_depth - len(mask) * 8)

        print(f"Word binary: {word}")
        print(f"Mask binary: {mask}")

        for i in range(len(self.bit_array)):
            if self.bit_array[i][0] == 0:
                address_indicator[i] = 0

        for bit_position in range(self.bit_depth):
            if mask[bit_position] == '0':
                continue
            print("iteration for bit: ", bit_position)
            for cell_index in range(self.size):
                if address_indicator[cell_index] == 0:
                    continue
                comparison_result = int(self.bit_array[cell_index][bit_position]) == int(word[bit_position])
                print(f"{self.read_word(cell_index)}|Comparing cell {cell_index}, bit position {bit_position}: "
                      f"{int(self.bit_array[cell_index][bit_position])} == {int(word[bit_position])} -> {comparison_result}")
                if not comparison_result:
                    address_indicator[cell_index] = 0

        # Возвращаем индексы ячеек памяти, которые остались в единичном состоянии
        result_indices = [(index, self.read_word(index)) for index, indicator in enumerate(address_indicator) if
                          indicator == 1]
        return result_indices

    def compare_eq(self, word):
        result_indices = [(index, self.read_word(index)) for index in range(self.size) if self.read_word(index) == word]
        return result_indices

    def compare_gt(self, word):
        result_indices = [(index, self.read_word(index)) for index in range(self.size) if self.read_word(index) > word and self.read_word(index)]
        return result_indices

    def compare_ge(self, word):
        result_indices = [(index, self.read_word(index)) for index in range(self.size) if self.read_word(index) >= word and self.read_word(index)]
        return result_indices

    def compare_lt(self, word):
        result_indices = [(index, self.read_word(index)) for index in range(self.size) if self.read_word(index) < word and self.read_word(index)]
        return result_indices

    def compare_le(self, word):
        result_indices = [(index, self.read_word(index)) for index in range(self.size) if self.read_word(index) <= word and self.read_word(index)]
        return result_indices

    def compare_ne(self, word):
        result_indices = [(index, self.read_word(index)) for index in range(self.size) if self.read_word(index) != word and self.read_word(index)]
        return result_indices

memory = AM(100,256)
with open('dict.txt', 'r', encoding='utf-8') as file:
    content = file.read()
words = content.split(',')
for word in words:
    memory.add_word(word)

a = memory.search("Alexey","101")
print(a)
print(memory.compare_ne("Alexey"))
