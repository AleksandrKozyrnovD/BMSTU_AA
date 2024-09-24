import matplotlib.pyplot as plt 
import string
import random
from time import perf_counter

from lsht import lvsh_dynamic, lvsh_recursive, dlvsh_dynamic

COUNT = 500
SIZES = [x for x in range(1, 10)]

def get_random_string(size):
    letters = string.ascii_lowercase

    return "".join(random.choice(letters) for _ in range(size))


def get_time_of(func, size):
    
    time_res = 0
    for _ in range(COUNT):
        str1 = get_random_string(size)
        str2 = get_random_string(size)

        time_start = perf_counter()
        func(str1, str2)
        time_end = perf_counter()

        time_res += (time_end - time_start)


    return time_res / COUNT

def rec_dyn(recursive_time, dynamic_time):

    fig = plt.figure(figsize=(10, 7))
    plot = fig.add_subplot()

    plot.plot(SIZES, recursive_time, label = "Рекурсивный Левенштейн" , color='b')
    plot.plot(SIZES, dynamic_time, label = "Динамический Левенштейн", color='r')

    plt.legend()
    plt.grid()
    plt.title("Временные характеристики")
    plt.ylabel("Затраченное время (с)")
    plt.xlabel("Длина (букв)")
    
    plt.show()

# Вывести график сравнения Лев(дин) и ДамЛев(дин)
def dyn_dam_and_nodam(dynamic_time, dam_dynamic_time):

    fig = plt.figure(figsize=(10, 7))
    plot = fig.add_subplot()

    plot.plot(SIZES, dynamic_time, label = "Динамический Левенштейн", color='r')
    plot.plot(SIZES, dam_dynamic_time, label="Динамический Дамерау-Левенштейн", color='g')

    plt.legend()
    plt.grid()
    plt.title("Временные характеристики")
    plt.ylabel("Затраченное время (с)")
    plt.xlabel("Длина (букв)")
    
    plt.show()

# Функция для вывода графиков всех трёх алгоритмов
def all_alg(recursive_time, dynamic_time, dam_dynamic_time):

    fig = plt.figure(figsize=(10, 7))
    plot = fig.add_subplot()

    plot.plot(SIZES, recursive_time, label="Рекурсивный Левенштейн", color='b')
    plot.plot(SIZES, dynamic_time, label="Динамический Левенштейн", color='r')
    plot.plot(SIZES, dam_dynamic_time, label="Динамический Дамерау-Левенштейн", color='g')

    plt.legend()
    plt.grid()
    plt.title("Временные характеристики всех алгоритмов")
    plt.ylabel("Затраченное время (с)")
    plt.xlabel("Длина (букв)")
    
    plt.show()


def experiment():
    recursive_time = []
    dynamic_time = []
    dam_dynamic_time = []

    for num in range(len(SIZES)):
        print(num)
        recursive_time.append(get_time_of(lvsh_recursive, num))
        dynamic_time.append(get_time_of(lvsh_dynamic, num))
        dam_dynamic_time.append(get_time_of(dlvsh_dynamic, num))

    rec_dyn(recursive_time, dynamic_time)
    dyn_dam_and_nodam(dynamic_time, dam_dynamic_time)
    all_alg(recursive_time, dynamic_time, dam_dynamic_time)


    for num in range(len(SIZES)):
        print(" %8d & %.2e & %.2e & %.2e \\\\ \n \\hline" %(num, \
            recursive_time[num], \
            dynamic_time[num], \
            dam_dynamic_time[num]))

if __name__ == "__main__":
    experiment()