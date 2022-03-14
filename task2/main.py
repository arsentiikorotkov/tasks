import math
import matplotlib.pyplot as plt
import time

x0 = float(input("Введите абсциссу стартовой точки: "))
y0 = float(input("Введите ординату стартовой точки: "))
s = str(input("Введите критерий останова (eps или delta): "))

tic = time.perf_counter()

def F(x, y):
    return (x**3) * (y**5) * (4 - x - 7 * y)

def dFx(x, y):
    return (x**2) * (y**5) * (12 - 4 * x - 21 * y)

def dFy(x, y):
    return (x**3) * (y**4) * (20 - 5 * x - 42 * y)

def mod(x, y):
    return math.sqrt((x**2) + (y**2))

max = F(4 / 3, 20 / 63)

arrx = [x0]
arry = [y0]
arrz = [F(x0, y0)]

ak = 10**-4
dz = abs(arrz[0])
norm = mod(x0, y0)
e = 10**-7
d = 10**-9
N = 0
res = ""

ost = 0
if (s == "eps"):
    res = "eps, df < " + str(e)
    ost = e
elif (s == "delta"):
    res = "delta, norm < " + str(d)
    ost = d

value = ost + 1
while value > ost:
    xk = arrx[len(arrx) - 1]
    yk = arry[len(arry) - 1]
    zk = arrz[len(arrz) - 1]

    xn = xk - ak * dFx(xk, yk)
    yn = yk - ak * dFy(xk, yk)
    zn = F(xn, yn)

    if (s == "eps"):
        value = abs(zn - zk)
    elif (s == "delta"):
        value = mod(abs(xn - xk), abs(yn - yk))

    arrx.append(xn)
    arry.append(yn)
    arrz.append(zn)

    N = N + 1
    ak = ak - (10**-3)

fig, ax = plt.subplots()
ax.set_title("График пройденных точек: ", fontsize=20)
ax.set_xlabel("Ось x", fontsize=15)
ax.set_ylabel("Ось y", fontsize=15)
ax.grid(linewidth=1)
ax.scatter(arrx, arry, color='darkblue', label="Точки")
ax.legend(loc='best')
plt.show()

toc = time.perf_counter()

print("\n" + "Критерий останова: " + res + "\n" + "Количество итераций: " + str(N)
      + "\n" + "Координаты точки: (" + str(arrx[len(arrx) - 1])
      + ", " + str(arry[len(arry) - 1]) + ")" + "\n" + "Значение функции: " + str(arrz[len(arrz) - 1]) + "\n"
      + "Время работы программы: " + str(toc - tic) + " sec" + "\n"
      + "Модуль разницы с точным значением: " + str(abs(max - arrz[len(arrz) - 1])))



