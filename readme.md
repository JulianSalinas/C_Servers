# Proyecto I: Sistemas Operativos

Se puede acceder a la página principal desde [aquí][Pagina]

[Pagina]: https://drive.google.com/drive/folders/0B3BuxMUBBTzuT3JNRmh4Vm1PNjg?usp=sharing

El programa que desmuestra el uso de distintos tipos de servidores en C. El cliente puede
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
 
 Para ejecutar un servidor se debe indicar el puerto, por ejemplo: 
 
 ```
 ./C_Server -f 5000
 ```
 
 El servidor de Pre-Thread es especial pues requiere el número de Threads máximo como parámetro: 
  
  ```
  ./C_Server -p 5000 4
  ```
  
 En caso del cliente se debe especificar el ip o nombre del servidor 
 (puede ser localhost), el puerto y los archivos que se desean obtener
 separados por comas. 
 Ejemplo:
 
  ```
  ./C_Server -c localhost 5000 "test1.txt","test2.html"
  ```
  
  Desde el browser se puede obtener solamente un archivo a la vez. Para
  hacerlo se tiene el siguiente ejemplo: 
  
  ```
  http://localhost:5000/test3.png
  ```
  
