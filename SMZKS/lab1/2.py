from qiskit import QuantumCircuit, transpile
from qiskit_aer import Aer
import matplotlib.pyplot as plt

# Классическая функция
def classical_F(a, b, c, d, e):
    return (a or b or c) and (not (d and e))

def build_circuit(a, b, c, d, e):
    qc = QuantumCircuit(9, 1)

    # подготовка входов
    if a: qc.x(0)
    if b: qc.x(1)
    if c: qc.x(2)
    if d: qc.x(3)
    if e: qc.x(4)

    # (A or B)
    qc.cx(1,5)
    qc.cx(0,5)
    qc.ccx(0,1,5)

    # (A or B or C)
    qc.cx(5,6)
    qc.cx(2,6)
    qc.ccx(2,5,6)

    # not (D and E)
    qc.ccx(3,4,7)
    qc.x(7)

    # (A ∨ B ∨ C) ∧ ¬(D ∧ E)
    qc.ccx(6,7,8)

    qc.measure(8,0)
    qc.draw(output='mpl')
    plt.show()
    return qc

# Симуляция схемы
def simulate(qc):
    backend = Aer.get_backend("aer_simulator")
    tqc = transpile(qc, backend)
    job = backend.run(tqc, shots=1)
    result = job.result()
    counts = result.get_counts()
    return int(max(counts, key=counts.get))

# Проверка всех входов
print("A B C D E | Classical | Quantum")
print("-------------------------------")
for x in range(32):
    a = (x >> 0) & 1
    b = (x >> 1) & 1
    c = (x >> 2) & 1
    d = (x >> 3) & 1
    e = (x >> 4) & 1

    classical = classical_F(a,b,c,d,e)
    qc = build_circuit(a,b,c,d,e)
    quantum = simulate(qc)

    print(f"{a} {b} {c} {d} {e} |     {classical}     |    {quantum}")
