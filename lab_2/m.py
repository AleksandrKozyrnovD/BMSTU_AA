import matplotlib.pyplot as plt

sizes = []
classic = []
vinograd = []
vinograd_opt = []
with open("data.txt", "r") as f:

    line = f.readline()
    # print(line)
    while line:
        test = line.split(" ")
        line = f.readline()

        sizes.append(int(test[0]))
        classic.append(float(test[1]))
        vinograd.append(float(test[2]))
        vinograd_opt.append(float(test[3].rstrip()))



plt.plot(sizes, classic, color="r", label="Классический алгоритм умножения матриц")
plt.plot(sizes, vinograd, color="g", label="Алгоритм умножения матриц Винограда")
plt.plot(sizes, vinograd_opt, color="b",  label="Оптимизированный алгоритм умножения матриц Винограда")


plt.xlabel("Размер матрицы")
plt.ylabel("Время выполнения (тики процессора)")
plt.legend()

plt.show()