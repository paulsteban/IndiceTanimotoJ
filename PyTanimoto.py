import threading
import time


def lstEArrobas(a):
    lsta = []
    lsta.append(a[0])
    count = 0
    for i in range(1, len(a)):
        if (lsta[-1] != '@' or a[i] != '@'):
            lsta.append(a[i])
        else:
            if(count<=0):
                count+=1
                lsta.append(a[i])

    return lsta


def intCantidadElementos(lista, elemento):
    contador = 0
    for elementos in lista:
        if (elementos == elemento):
            contador = contador + 1

    return contador


def lstElementosSinRepeticion(lista):
    elementosSinRepeticion = []
    for i in range(0, len(lista)):
        if (intCantidadElementos(elementosSinRepeticion, lista[i]) == 0 and lista[i] != '(' and lista[
            i] != ')' and lista[i] != '[' and lista[i] != ']' and lista[i] != '=' and lista[i] != '-' and lista[
            i] != '+' and not lista[i].isdigit() and lista[i] != 'n'):
            elementosSinRepeticion.append(lista[i])
    return elementosSinRepeticion


def intnumElementos(lista):
    nroElementos = 0;
    elementosSinRepeticion = lstElementosSinRepeticion(lista)
    for i in range(0, len(elementosSinRepeticion)):
        nroElementos = nroElementos + intCantidadElementos(lista, elementosSinRepeticion[i])

    return nroElementos


def intElementosComunes(lista1, lista2):
    nroElementosComunes = 0;
    elementosSinRepeticion = lstElementosSinRepeticion(lista1)
    for i in range(0, len(elementosSinRepeticion)):
        nroElementosComunes = nroElementosComunes + min(
            intCantidadElementos(lista1, elementosSinRepeticion[i]),
            intCantidadElementos(lista2, elementosSinRepeticion[i]))
    return nroElementosComunes


def fltIndiceJaccardTanimoto(cadena1, cadena2):
    cadena1 = cadena1.replace("Cl", "*").replace("Br", "$")
    cadena2 = cadena2.replace("Cl", "*").replace("Br", "$")
    Na = intnumElementos(lstEArrobas(cadena1))
    Nb = intnumElementos(lstEArrobas(cadena2))
    Nc = intElementosComunes(lstEArrobas(cadena1), lstEArrobas(cadena2))

    return float(Nc) / float(Na + Nb - Nc)


'''
INCIO
'''

indicadores = [];
keys = [];
archivoEntrada = open("ZINC_chemicals.tsv", "r")
START = time.time()
for i, linea in enumerate(archivoEntrada):
    if i >= 1:
        indicadores.append(linea[linea.find('\t') + 1:linea.find('\t') + 13])
        keys.append(linea[linea.rfind('\t') + 1:len(linea) - 1])

archivoEntrada.close()


def Salidad(a, b):
    archivoSalida = open("IndiceTanimoto.txt", "w");
    for i in range(a, b):
        for j in range(a, (len(keys) - 1)):
            if (i < j):
                archivoSalida.write(
                    indicadores[i] + '\t' + indicadores[j] + '\t' + "%.2f" % fltIndiceJaccardTanimoto(keys[i], keys[
                        j]) + '\n')


T3 = threading.Thread(target=Salidad, args=(0, 6072,))
T4 = threading.Thread(target=Salidad, args=(6073, 8587,))
T5 = threading.Thread(target=Salidad, args=(8588, 10517,))
T6 = threading.Thread(target=Salidad, args=(10518, 12423,))
T3.start()
T4.start()
T5.start()
T6.start()
T3.join()
T4.join()
T5.join()
T6.join()

end = time.time()
print end - START