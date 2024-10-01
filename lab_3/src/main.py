from random import randint
import matplotlib.pyplot as plt
import numpy as np
from math import log2

def locate(source: list[int], x: int) -> (int, int):
    index = -1
    compare = 0

    for i in range(len(source)):
        compare += 1
        if source[i] == x:
            index = i
            break

    return index, compare

def bin_locate(source: list[int], x: int) -> (int, int):
    answer =-1
    compare = 0

    left = 0
    right = len(source) - 1
    while left <= right:
        compare += 1
        middle = (left + right) // 2
        if source[middle] == x:
            answer = middle
            break
        elif source[middle] < x:
            left = middle + 1
        else:
            right = middle - 1

    return answer, compare

def linear_locate_histogram(source: list[int]) -> None:
    comparisons = [locate(source, x)[0] for x in source]
    indexes = [locate(source, x)[1] for x in source]

    b, a = locate(source, -1e444)
    indexes.append(a)
    comparisons.append(b)

    plt.bar(comparisons, indexes, width=1)

    plt.title("Нахождение элемента алгоритмом полного перебора")
    plt.xlabel("Индекс элемента")
    plt.xticks(np.arange(0, lenght + 5, 5))

    plt.ylabel("Количество выполненных сравнений")
    plt.grid(visible=True)

    plt.show()

def binary_locate_histogram(source: list[int]) -> None:
    source.sort()

    comparisons = [bin_locate(source, x)[0] for x in source]
    indexes = [bin_locate(source, x)[1] for x in source]
    b, a = bin_locate(source, -1e444)
    indexes.append(a)
    comparisons.append(b)

    plt.bar(comparisons, indexes, width=1)

    plt.title("Нахождение элемента алгоритмом двоичного поиска")
    plt.xlabel("Индекс элемента")
    plt.xticks(np.arange(0, lenght + 5, 5))
    plt.ylabel("Количество выполненных сравнений")
    plt.grid(visible=True)

    plt.show()

def binary_locate_histogram_sorted(source: list[int]) -> None:
    source.sort()


    fullist = [bin_locate(source, x) for x in source]

    b, a = bin_locate(source, -1e444)

    comparisons = [x[1] for x in fullist]
    comparisons.append(a)
    comparisons.sort()

    plt.bar([i for i in range(lenght + 1)], comparisons, width=1)

    plt.title("Нахождение элемента алгоритмом двоичного поиска, в порядке возрастания сравнений")
    plt.xlabel("Порядок количества выполненных сравнений")
    plt.xticks(np.arange(0, lenght + 5, 5))
    plt.ylabel("Количество выполненных сравнений")
    plt.grid(visible=True, axis='y')

    plt.show()

if __name__ == '__main__':
    X = 8113 # Номер таски?
    lenght = X % 1000 if X / 8 + ((X >> 2) % 10 == 0) else ((X >> 2) % 10 * (X % 10) + (X >> 1) % 10)

    source = []
    lenn = 0
    while lenn < lenght:
        x = randint(-10000, 10000)
        if x not in source:
            source.append(x)
            lenn += 1

    linear_locate_histogram(source)
    binary_locate_histogram(source)
    binary_locate_histogram_sorted(source)