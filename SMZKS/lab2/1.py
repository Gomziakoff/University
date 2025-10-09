import matplotlib.pyplot as plt

from qiskit import QuantumCircuit, ClassicalRegister, QuantumRegister
from qiskit_aer import Aer

backend = Aer.get_backend('qasm_simulator')
shots = 2000
prob_of_ans = []

def oracle(qc, database, oracle, aux, target="0111010"):
    for i, bit in enumerate(target[::-1]):
        if bit == '0':
            qc.x(database[i])
    qc.mcx(database, oracle[0], aux[:])
    for i, bit in enumerate(target[::-1]):
        if bit == '0':
            qc.x(database[i])

def diffuser(qc, database, aux):
    qc.h(database[:])
    qc.x(database[:])
    qc.h(database[-1])
    qc.mcx(database[0:-1], database[-1], aux[:])
    qc.h(database[-1])
    qc.x(database[:])
    qc.h(database[:])

target = "0111010"

for x in range(15):
    database = QuantumRegister(7)
    oracle_reg = QuantumRegister(1)
    aux = QuantumRegister(2)
    cr = ClassicalRegister(7)
    qc = QuantumCircuit(database, oracle_reg, aux, cr)

    qc.h(database[:])
    qc.x(oracle_reg[0])
    qc.h(oracle_reg[0])

    for j in range(x):
        oracle(qc, database, oracle_reg, aux, target)
        diffuser(qc, database, aux)

    qc.h(oracle_reg[0])
    qc.x(oracle_reg[0])
    qc.measure(database, cr)

    job = backend.run(qc, shots=shots, seed_simulator=12345)
    result = job.result()
    counts = result.get_counts()

    prob = counts.get(target, 0) / shots
    prob_of_ans.append(prob)
print(prob_of_ans)

iteration = [i for i in range(15)]
correct = prob_of_ans
plt.bar(iteration, correct)
plt.xlabel('# of iteration')
plt.ylabel('# of times the solution was obtained')
plt.show()