import random
import time
import matplotlib.pyplot as plt
import networkx as nx

def generate_dag(n, edge_prob=0.3):
    adj_matrix = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            if random.random() < edge_prob:
                adj_matrix[i][j] = 1
    return adj_matrix

def topological_sort_recursive(adj_matrix):
    n = len(adj_matrix)
    visited = [0] * n
    order = []
    has_cycle = [False]
    def dfs(v):
        if visited[v] == 1:
            has_cycle[0] = True
            return False
        if visited[v] == 2:
            return True
        visited[v] = 1
        for u in range(n):
            if adj_matrix[v][u]:
                if not dfs(u):
                    return False
                if has_cycle[0]:
                    return False
        visited[v] = 2
        order.append(v)
        return True
    for i in range(n - 1, -1, -1):
        if visited[i] == 0:
            if not dfs(i):
                return None
    order.reverse()
    return order

def topological_sort_iterative(adj_matrix):
    n = len(adj_matrix)
    in_degree = [0] * n
    for i in range(n):
        for j in range(n):
            if adj_matrix[i][j]:
                in_degree[j] += 1
    queue = []
    for i in range(n):
        if in_degree[i] == 0:
            queue.append(i)
    order = []
    count = 0
    while queue:
        u = queue.pop(0)
        order.append(u)
        count += 1
        for v in range(n):
            if adj_matrix[u][v]:
                in_degree[v] -= 1
                if in_degree[v] == 0:
                    queue.append(v)
    if count != n:
        return None
    return order

def draw_graph(adj_matrix, order=None):
    n = len(adj_matrix)
    G = nx.DiGraph()
    for i in range(n):
        for j in range(n):
            if adj_matrix[i][j]:
                G.add_edge(i, j)
    pos = nx.spring_layout(G, seed=42)
    plt.figure(figsize=(8, 6))
    nx.draw(G, pos, with_labels=True, node_color="lightblue", node_size=700, arrows=True)
    if order:
        plt.title(f"Топологический порядок: {order}")
    plt.show()

def benchmark(n=1000, edge_prob=0.01):
    adj_matrix = generate_dag(n, edge_prob)
    start = time.time()
    order1 = topological_sort_iterative(adj_matrix)
    t1 = time.time() - start
    start = time.time()
    order3 = topological_sort_recursive(adj_matrix)
    t3 = time.time() - start
    print(f"Вершин: {n}, плотность рёбер: {edge_prob}")
    print(f"Алгоритм Кана: {t1:.6f} сек")
    print(f"Рекурсивный DFS: {t3:.6f} сек")

def test_with_cycle():
    print("\nТестирование на графе с циклом:")
    adj_matrix = [
        [0, 1, 0],
        [0, 0, 1],
        [1, 0, 0]
    ]
    print("Рекурсивный DFS:", topological_sort_recursive(adj_matrix))
    print("Алгоритм Кана:", topological_sort_iterative(adj_matrix))

if __name__ == "__main__":
    print("Пример с маленьким графом:")
    adj = [
        [0, 1, 1, 0, 0, 0],
        [0, 0, 0, 1, 1, 0],
        [0, 0, 0, 0, 1, 0],
        [0, 0, 0, 0, 0, 1],
        [0, 0, 0, 0, 0, 1],
        [0, 0, 0, 0, 0, 0]
    ]
    order_r = topological_sort_recursive(adj)
    order_k = topological_sort_iterative(adj)
    print("Рекурсивный DFS:", order_r)
    print("Алгоритм Кана:", order_k)
    draw_graph(adj, order_r)
    print("\nТест производительности:")
    benchmark(2000, 0.1)
    test_with_cycle()