# Proyecto I: Sistemas Operativos

Programa que desmuestra el uso de distintos tipos de servidores en C. El cliente puede
solicitar a cualquiera de los servidores un archivo plano y el servidor se lo enviará. 
El cliente puede ser un Browser, Telnet o el que ha sido diseñado en este proyecto. Para
el envío de la información se utilizan sockets y el protocolo HTTP. 

##### Modo de uso
_________________________

 
 * -c : Cliente 
 * -f : Servidor FIFO
 * -k : Servidor FORK
 * -t : Servidor THREAD
 * -p : Servidor PTHREAD
 
 Para ejecutar un sevidor se debe indicar el puerto, por ejemplo: 
 
 ```
 ./C_Server -f 5000
 ```
 
 En caso del cliente se debe especificar el ip del servidor (puede ser localhost) 
 y el puerto. Ejemplo:
 
  ```
  ./C_Server -c localhost 5000
  ```
