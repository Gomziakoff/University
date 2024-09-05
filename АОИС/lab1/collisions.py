import matplotlib.pyplot as plt
from hash_table import HashTable

table = HashTable(41,'','')
table.read_csv('table.csv')
data = table.get_collisions()

x = [data[i][0] for i in range(len(data))]
y = [data[i][1] for i in range(len(data))]

plt.plot(x, y)
plt.xlabel('rows')
plt.ylabel('collisions')
plt.title("Collisios for each row")
plt.show()