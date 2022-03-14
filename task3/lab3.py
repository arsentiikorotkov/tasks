import numpy as np
import matplotlib.pyplot as plt
import time

tic = time.perf_counter()

def P(x, y):
    return ((x ** 2) * y)

def Q(x, y):
    return -1 * ((y ** 2) * x)

def Fdxdy(x, y):
    return -1 * ((x ** 2) + (y ** 2))

def G(x, y):
    return (-2 / 3) * ((x ** 2) * (y ** 2))

def arc(l):
    return (6 * np.arcsin(l / 6))

x = -3
y = 0
arrx = [x]
arry = [y]

eps = 1e-4
while x < (3 - eps):
    x = x + eps
    y = np.sqrt(9 - (x**2))
    arrx.append(x)
    arry.append(y)

arrx.append(3)
arry.append(0)

x = -3
y = 0
lomx = [x]
lomy = [y]

delta = 0.001 #Разность абсцисс между вершинами ломаной
while x < (3 - delta):
    x = x + delta
    y = np.sqrt(9 - (x**2))
    lomx.append(x)
    lomy.append(y)

lomx.append(3)
lomy.append(0)
size = len(lomx) - 1
x = 3
while x >= -3 + delta:
    x = x - delta
    lomx.append(x)
    lomy.append(0)

fig, ax = plt.subplots()
ax.set_aspect('equal')
ax.set_title("График: ", fontsize = 20)
ax.set_xlabel("Ось x", fontsize = 15)
ax.set_ylabel("Ось y", fontsize = 15)
ax.set_xticks(np.arange(-3, 3.5, 0.5))
ax.set_yticks(np.arange(0, 3.5, 0.5))
ax.grid(linewidth = 1, linestyle = '--')

ax.plot(arrx, arry, color = 'deepskyblue', label = "ABC", linewidth = 2)
ax.plot([-3, 3], [0, 0], color = 'darkgreen', label = "CA", linewidth = 2)
ax.scatter(3, 0, label = "A", color = 'red', marker = '*', s = 250)
ax.scatter(0, 3, label = "B", color = 'fuchsia', marker = '*', s = 250)
ax.scatter(-3, 0, label = "C", color = 'darkviolet', marker = '*', s = 250)

ax.plot(lomx, lomy, linewidth = 1, label = "Ломаная", color = 'darkblue')
ax.scatter(lomx, lomy, color = 'darkblue', s = 20)

ax.legend(loc = 'best')
plt.show()

IntSum1 = 0
for i in range (len(lomx) - 1) :
    IntSum1 = IntSum1 - ((P(lomx[i], lomy[i]) * (lomx[i + 1] - lomx[i])) + (Q(lomx[i], lomy[i]) * (lomy[i + 1] - lomy[i])))

Value = -(81 / 4) * np.pi #Точное значение интеграла
toc = time.perf_counter()
print("\n№2.1\nИнтегральная сумма: " + str(IntSum1) + "\nМодуль разницы с точным значением: "
      + str(np.abs(Value - IntSum1)) + "\nВремя вычисления: " + str(toc - tic))

tic = time.perf_counter()
IntSum2 = 0
IntSumMin = 0
IntSumMax = 0
a = 0.01 #Сторона квадратика
y = 0
while y < 3:
    x = -np.sqrt(9 - (y ** 2))
    while x < np.sqrt(9 - (y ** 2)):
        x0 = x + (a / 2)
        y0 = y + (a / 2)
        if ((x0 ** 2) + (y0 ** 2)) <= 9:
            IntSum2 = IntSum2 + ((a ** 2) * Fdxdy(x0, y0))
        if ((((x + a) ** 2) + ((y+ a) ** 2)) <= 9) & (((x ** 2) + ((y + a) ** 2)) <= 9):
            IntSumMin = IntSumMin + ((a ** 2) * Fdxdy(x0, y0))
        if (((x ** 2) + (y ** 2)) <= 9) | ((((x + a) ** 2) + (y ** 2)) <= 9):
            IntSumMax = IntSumMax + ((a ** 2) * Fdxdy(x0, y0))
        x = x + a
    y = y + a

toc = time.perf_counter()
print("\n№2.2\nИнтегральная сумма: " + str(IntSum2) + "\nМодуль разницы с точным значением: "
      + str(np.abs(Value - IntSum2)) + "\nВремя вычисления: " + str(toc - tic))

print("\n№2.4\nМодуль разность сумм: " + str(np.abs(IntSumMax - IntSumMin))
      + "\nВремя вычисления: " + str(toc - tic))

#tic = time.perf_counter()
#IntSum = 0
#for i in range (len(lomx) - 1):
#    IntSum = IntSum + np.sqrt(((lomx[i + 1] - lomx[i]) ** 2) + ((lomy[i + 1] - lomy[i]) ** 2)) * G(lomx[i], lomy[i])
#
#toc = time.perf_counter()
#print("\n№2.5\nИнтегральная сумма: " + str(IntSum) + "\nМодуль разницы с точным значением: "
#      + str(np.abs(Value - IntSum)) + "\nВремя вычисления: " + str(toc - tic))

tic = time.perf_counter()
IntSum = 0
for i in range (len(lomx) - 1):
    if i < size :
        IntSum = IntSum + arc(np.sqrt(((lomx[i + 1] - lomx[i]) ** 2) + ((lomy[i + 1] - lomy[i]) ** 2))) * G(lomx[i], lomy[i])
    elif i >= size :
        IntSum = IntSum + np.sqrt(((lomx[i + 1] - lomx[i]) ** 2) + ((lomy[i + 1] - lomy[i]) ** 2)) * G(lomx[i], lomy[i])


toc = time.perf_counter()
print("\n№2.5\nИнтегральная сумма: " + str(IntSum) + "\nМодуль разницы с точным значением: "
      + str(np.abs(Value - IntSum)) + "\nВремя вычисления: " + str(toc - tic))


