function ordenarTablaCant(nomTabla,modo) {

 tbody = document.getElementById(nomTabla);
 filas = tbody.getElementsByTagName("tr");
 filasCabecera = 4;

 if(modo == "desc"){ // Si queremos ordenar Descentdentemente

   if (filas.length > filasCabecera){     // Vemos si existen filas en la tabla
      arrayOrd = new Array();   // creo un nuevo array donde iran las filas ya ordenadas
      i = filasCabecera;
      while (i<filas.length){  // Recorremos todas las filas (menos las de cabecera)
	 j = filasCabecera;
	 maxim = 0;
	 while (j<filas.length){  // Obtengo el numero fila con el valor maximo de cantidad
	    columnas = filas[j].getElementsByTagName("td");
            cant = parseInt(columnas[3].firstChild.nodeValue);
            if (maxim == 0){
               maxim = cant;
	       fila = j;
            }else
	       if ((cant != 0)&&(cant > maxim)){
                  maxim = cant;
	          fila = j;
               }
	 j += 1;
	 }
	 // Obtengo la fila en si
	 columnas = filas[fila].getElementsByTagName("td");
	 // creamos una fila igual a la que tiene mayor cantidad
	 // Primero todas las celdas
	 celda0 = document.createElement("td");
	 celda0.appendChild(document.createTextNode(columnas[0].firstChild.nodeValue));
	 celda1 = document.createElement("td");
	 celda1.appendChild(document.createTextNode(columnas[1].firstChild.nodeValue));
         celda2 = document.createElement("td");
	 celda2.appendChild(document.createTextNode(columnas[2].firstChild.nodeValue));
         celda3 = document.createElement("td");
	 celda3.appendChild(document.createTextNode(columnas[3].firstChild.nodeValue));
	 celda4 = document.createElement("td");
	 celda4.appendChild(document.createTextNode(columnas[4].firstChild.nodeValue));
	 
	 // ahora lo meto en el array Ordenado en su posicion corrspondiente
	 arrayOrd[i-filasCabecera] = document.createElement("tr");
	 arrayOrd[i-filasCabecera].appendChild(celda0);
	 arrayOrd[i-filasCabecera].appendChild(celda1);
	 arrayOrd[i-filasCabecera].appendChild(celda2);
	 arrayOrd[i-filasCabecera].appendChild(celda3);
	 arrayOrd[i-filasCabecera].appendChild(celda4);
	 // pongo el valor de cantidad para esa fila en el documento a -1 para no volver a copiar la misma fila
	 columnas[3].firstChild.nodeValue = "0";
         i += 1;
      }
      // ahora tengo que meter las filas ordenadas del array en el documento
      while (filas.length > filasCabecera) // Elimino las filas actuales
         tbody.removeChild(filas[filasCabecera]);
      i = 0;
      while (i<arrayOrd.length){ // Introduzco ordenadas las filas en el documento
         tbody.appendChild(arrayOrd[i]);
         i += 1;
      }
   }
 }
 else
 if (modo == "asc"){    // Si queremos ordenar Ascendentemente
   if (filas.length > filasCabecera){     // Vemos si existen filas en la tabla
      arrayOrd = new Array();   // creo un nuevo array donde iran las filas ya ordenadas
      i = filasCabecera;
      while (i<filas.length){  // Recorremos todas las filas (menos las de cabecera)
         j = filasCabecera;
         minim = 0;
         while (j<filas.length){  // Obtengo el numero fila con el valor minimo de cantidad
            columnas = filas[j].getElementsByTagName("td");
            cant = parseInt(columnas[3].firstChild.nodeValue);
            if (minim == 0){
               minim = cant;
	       fila = j;
            }else
	       if ((cant != 0)&&(cant < minim)){
                  minim = cant;
	          fila = j;
	       }
	    j += 1;
         }
         // Obtengo la fila en si
         columnas = filas[fila].getElementsByTagName("td");
         // creamos una fila igual a la que tiene mayor cantidad
         // Primero todas las celdas
         celda0 = document.createElement("td");
         celda0.appendChild(document.createTextNode(columnas[0].firstChild.nodeValue));
         celda1 = document.createElement("td");
         celda1.appendChild(document.createTextNode(columnas[1].firstChild.nodeValue));
         celda2 = document.createElement("td");
         celda2.appendChild(document.createTextNode(columnas[2].firstChild.nodeValue));
         celda3 = document.createElement("td");
         celda3.appendChild(document.createTextNode(columnas[3].firstChild.nodeValue));
         celda4 = document.createElement("td");
         celda4.appendChild(document.createTextNode(columnas[4].firstChild.nodeValue));

         // ahora lo meto en el array Ordenado en su posicion corrspondiente
         arrayOrd[i-filasCabecera] = document.createElement("tr");
         arrayOrd[i-filasCabecera].appendChild(celda0);
         arrayOrd[i-filasCabecera].appendChild(celda1);
         arrayOrd[i-filasCabecera].appendChild(celda2);
         arrayOrd[i-filasCabecera].appendChild(celda3);
         arrayOrd[i-filasCabecera].appendChild(celda4);

        // pongo el valor del cantidad para esa fila en el documento a 0 para no volver a copiar la misma fila
        columnas[3].firstChild.nodeValue = "0";
        i += 1;
      }
      // ahora tengo que meter las filas ordenadas del array en el documento
      while (filas.length > filasCabecera) // Elimino las filas actuales
      tbody.removeChild(filas[filasCabecera]);
      i = 0;
      while (i<arrayOrd.length){ // Introduzco ordenadas las filas en el documento
         tbody.appendChild(arrayOrd[i]);
         i += 1;
      }
   }
 }
}
