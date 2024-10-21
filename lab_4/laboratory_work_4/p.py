import matplotlib.pyplot as plt

def find_maximums(filename) -> [list[(int, float)]]:
    # int: N
    # float: value for that N
    datalist = []

    with open(filename, 'r') as file:
        line = file.readline()

        while line != "":
            value = (int(line.split()[0]), float(line.split()[1]))
            datalist.append(value)
            line = file.readline()
            
    return datalist

if __name__ == '__main__':
    data = find_maximums('data.txt')
    parallels = find_maximums('parallel.txt') #16 threads
    seq = find_maximums('sequent.txt') #1 thread

    # x = [x[0] for x in data]
    # y = [x[1] for x in data]
    # for line in data:
    #     print(f"{line[0]} & {line[1]} \\\\\n \\hline")

    # plt.title('Зависимость времени работы от количества потоков')
    plt.title('Зависимость времени работы от количества страниц')

    plt.xlabel('Количество страниц')
    plt.ylabel('Время (мс)')

    # plt.xticks(x)
    # plt.yticks(y)
    plt.xticks([x + 1 for x in range(5)])
    plt.yticks([y for y in range(0, int(seq[4][1]) + 1, 1000)])

    plt.plot([x[0] for x in parallels], [x[1] for x in parallels], color='red', marker='o', label='16 потоков')
    plt.plot([x[0] for x in seq], [x[1] for x in seq], color='blue', marker='o', label='1 поток')

    plt.grid(True)
    plt.legend()
    plt.show()

    for line1, line2 in zip(parallels, seq):
        print(f"{line1[0]} & {line1[1]} & {line2[1]} \\\\\n \\hline")