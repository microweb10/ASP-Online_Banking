#include "qdecoder.c"
#include <stdlib.h>

int main(){
  char *comision,*cantidad;
  int cant,com;
  float res;
  
  comision = qValue("comision");
  cantidad = qValue("cantidad");
  com = atoi(comision);
  cant = atoi(cantidad);
  res = (float)(cant*com);
  res = res/100;
  
  /* la cabecera */
  printf("Content-type: text/html\n");
  printf("\n");
  /* El cuerpo */
  printf("<html><body>");
  printf("<center><font size='10'>Cálculo de Comisiones</font><br><br>\n");
  printf("<center><b> CANTIDAD: %d € </b></center><br>",cant);
  printf("<center><b> TIPO INTERES: %d%% </b></center><br>",com);
  printf("<center><b> --------------------------------- </b></center><br>");
  printf("<center><b><font color='#FF0000'> TOTAL COMISION: %f Euros </font></b></center><br>",res);  
  printf("<center><a href='../abierta/Principal.php'>VOLVER A LA PAGINA PRINCIPAL</a><center>");
  printf("</body></html>");
return 0;
}
