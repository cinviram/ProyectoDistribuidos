import matplotlib.pyplot as plt
from matplotlib.ticker import StrMethodFormatter
import seaborn as sns

# Create some data

archivo1='tiempo.txt'
archivo2='tiempo2.txt'



cantidad_de_Elementos=[]
tiempo_segundos=[]


f = open(archivo1, 'r')
try:
    for linea in f:
        x=linea.split(",")[0]
        cantidad_de_Elementos.append(int(x))
        y=linea.split(",")[1]
        y= y.replace('\r', '').replace('\n', '')
        tiempo_segundos.append(float(y))
finally:
    f.close()

print(tiempo_segundos)

cantidad_de_Elementos2=[]
tiempo_segundos2=[]


f = open(archivo2, 'r')
try:
    for linea in f:
        x=linea.split(",")[0]
        cantidad_de_Elementos2.append(int(x))
        y=linea.split(",")[1]
        y= y.replace('\r', '').replace('\n', '')
        tiempo_segundos2.append(float(y))
finally:
    f.close()


# fig = plt.figure()
# ax = fig.add_subplot(111)
#
# x = np.linspace(300, 3000, 500)
# y = np.linspace(300,3, 1)
#
#
# ax.plot(x,y)


sns.lineplot(cantidad_de_Elementos,tiempo_segundos)
sns.lineplot(cantidad_de_Elementos2,tiempo_segundos2)

plt.title('Serie de Tiempo del método Get')
# Set x-axis label
plt.xlabel('Elementos procesados')

# Set y-axis label
plt.ylabel('Tiempo [s]')

plt.legend(title='Computadoras', loc='upper left', labels=['1', '2'])
plt.axis([0, 3500, 0, 1])
plt.gca().yaxis.set_major_formatter(StrMethodFormatter('{x:,.3f}')) # 2 decimal places
plt.grid(True)
plt.show()