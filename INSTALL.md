# The Guardian Journey

## Compilación del proyecto

El software debe ser compilado en el IDE Dev-C++. En caso de algún problema con el compilador, intente realizar la siguiente operación:

* Abrir el IDE Dev-C++
* Seleccionar Tools/Herramientas en la barra superior
* Seleccionar Compiler options.../Opciones de compilador...
* Seleccionar la pestaña General
* Marcar la casilla Add the following commands when calling the compiler/Agregue los siguientes comandos al llamar al compilador
* En el cuadro de texto de la casilla escriba: -std=c++11
* Seleccione OK e intente ejecutar nuevamente el programa

Sobre los archivos de lectura:
* Deben estar en formato .txt
* Deben tener encabezados en el siguiente orden y formato:
	* Archivo de aldeas: Aldea,AldeaConectada
	* Archivo de guardianes: Nombre,Poder,Maestro,Aldea
* Note que los datos están separados por una coma y sin espacios entre ellos. En caso de que exista un espacio, el programa podría no funcionar correctamente
* El programa distingue entre mayúsculas y minúsculas, por lo que un cambio de este tipo es relevante a la hora de las validaciones internas

Los archivos tienen algunas restricciones. Sin embargo, existen las validaciones internas que detendrán la lectura en caso de que se detecta la invalidación de alguna;

* Archivo de aldeas:
	* El archivo debe contener la ciudad principal (En este caso Tesla. Si se quisiera cambiar, el código cuenta con variables globales que se pueden modificar para dicho propósito)
	* Una aldea no puede estar conectada consigo misma
	* Cada aldea debe tener 2 conexiones como mínimo
	* El archivo debe contener 3 aldeas como mínimo
* Archivo de guardianes:
	* Restricciones del maestro principal:
		* El maestro principal debe estar en la ciudad principal (Tesla)
		* El maestro principal no debe tener maestro
		* El maestro principal debe tener un nivel de poder que fluctúe entre el rango definido exclusivamente para él
	* Restricciones de los demás guardianes:
		* La aldea del guardián debe existir
		* El maestro del guardián debe existir
		* Los puntos de poder del guardián deben ser menor al poder de su maestro y mayores a un nivel de poder mínimo definido dentro del programa
		* Un maestro no puede tener un nivel de poder igual al mínimo definido dentro del programa
		* Deben existir al menos 2 guardianes en una misma aldea
		* Cada maestro debe tener al menos 1 aprendiz en la misma aldea

Como se mencionó anteriormente, cada restricción anterior está validada dentro del programa, por lo que se indicará el error al ejecutarlo y, en la medida que sea posible, el archivo y la línea del error. Los archivos presentes en el repositorio son los ideales, sin embargo, se pueden cambiar considerando los puntos anteriores.

Observación: La lista de guardianes se lee de forma lineal, por lo que el maestro de un guardián debe estar antes que él. Como consecuencia de esto, el maestro supremo debe estar directamente después de los encabezados.
