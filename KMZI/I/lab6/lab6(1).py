def xor(bitset1, bitset2):
    return [bitset1[i] ^ bitset2[i] for i in range(len(bitset1))]
def get_sum(bitset):
    bitset += [0,0,0,0]
    temp = bitset[0:5]
    chastnoe = []
    for i in range(5, len(bitset), 1):
        print(temp,"delim")
        if temp[0] == 1:
            print([1,0,0,1,1],"delit")
            temp = xor(temp, [1,0,0,1,1])
            chastnoe.append(1)
        else:
            print([0,0,0,0,0],"delit")
            temp = xor(temp, [0,0,0,0,0])
            chastnoe.append(0)
        print()
        temp.pop(0)
        temp += [bitset[i]]
    print(temp,"delim")
    if temp[0] == 1:
            print([1,0,0,1,1],"delit")
            temp = xor(temp, [1,0,0,1,1])
            chastnoe.append(1)
    else:
            print([0,0,0,0,0],"delit")
            temp = xor(temp, [0,0,0,0,0])
            chastnoe.append(0)
    temp.pop(0)
    return chastnoe, temp

str = "11000101"
print(get_sum([int(x) for x in str]))