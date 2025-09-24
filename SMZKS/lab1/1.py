from qiskit import QuantumCircuit
from qiskit_aer import Aer
import matplotlib.pyplot as plt

qc = QuantumCircuit(5, 2)

qc.x(0)
qc.x(2)

qc.cx(0, 3)  # S = A
qc.cx(1, 3)  # S = A ⊕ B
qc.cx(2, 3)  # S = A ⊕ B ⊕ X

qc.ccx(0, 1, 4)   # A & B
qc.cx(0, 1)       # временный XOR (A ⊕ B)
qc.ccx(1, 2, 4)   # (X & (A ⊕ B))
qc.cx(0, 1)       # возвращаем B обратно

qc.measure(3, 0)
qc.measure(4, 1)

backend = Aer.get_backend('qasm_simulator')
job = backend.run(qc, shots=1000)
result = job.result()
counts = result.get_counts()
print(counts)

qc.draw(output='mpl')
plt.show()
