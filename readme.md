# Proyecto I: Sistemas Operativos

Se puede acceder a la página principal desde [aquí][Pagina].

[Pagina]: https://juliansalinas.github.io/C_Servers/

El programa que desmuestra el uso de distintos tipos de servidores en C. El cliente puede
solicitar a cualquiera de los servidores un archivo plano y el servidor se lo enviará. 
El cliente puede ser un Browser, Telnet o el que ha sido diseñado en este proyecto. Para
el envío de la información se utilizan sockets y el protocolo HTTP. 

### Compilación
_________________________

Para compilar el proyecto es necesario contar con **make** y **cmake 3.7**
como mínimo, de lo contrario será necesario abrir una terminal con 
*Ctrl + Alt + T* y ejecutar los siguientes comandos para realizar su 
instalación: 

 >sudo apt-get install make
 
 *Nota: Estos programas están disponibles por defecto en el Ubuntu 17.10.*
 
 Una vez obtenidas estas dependencias, es necesario abrir una terminal 
 ubicada dentro del proyecto. En esta colocamos el siguiente comando 
 para crear una carpeta que almacene el ejecutable, en este caso *Bin*: 
  
 >mkdir Bin
  
 Ingresamos a la carpeta que se acaba de crear: 
  
 >cd Bin
  
 Luego ejecutamos el siguiente comando para generar el archivo *Makefile*,
 necesario para el siguiente paso: 
 
 >cmake ..
  
 Para finalizar, compilamos el proyecto ingresando simplemente: 
  
 >make
  
 *Nota: El proyecto ya contiene la carpeta Bin, por tanto, 
 para ejecutar el programa solamente es necesario accederla.*
  
### Ejecución
_________________________

 
 * -c : Cliente 
 * -f : Servidor FIFO
 * -k : Servidor FORK
 * -t : Servidor THREAD
 * -p : Servidor PTHREAD
 
 Para ejecutar un servidor se debe indicar el puerto, por ejemplo: 
 
 >./C_Server -f 5000
 
 El servidor de Pre-Thread es especial pues requiere el número de Threads máximo como parámetro: 
  

 >./C_Server -p 5000 4

  
 En caso del cliente se debe especificar el ip o nombre del servidor 
 (puede ser localhost), el puerto, la ruta de guardado y 
 los archivos que se desean obtener
 separados por comas (sin espacios). 
 Ejemplo:
 
 >./C_Server -c localhost 5000 /home/julian/tests test1.txt, test2.html
  
 Desde el browser se puede obtener solamente un archivo a la vez. Para
 hacerlo se tiene el siguiente ejemplo: 
  
 >http://localhost:5000/test3.png
 
