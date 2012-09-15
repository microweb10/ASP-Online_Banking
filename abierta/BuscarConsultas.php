<!doctype html public "-//w3c//dtd html 4.0 transitional//en">
<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
   <meta name="Author" content="Julian Nicolas">
   <meta name="Copyright" content="© Julian Nicolas">
   <meta http-equiv="Content-Script-Type" content="JavaScript">
   <title>PIDIRECT - Buscador de Consultas </title>
   <script src="../javascript/ValidaForm.js"></script>
</head>
<body>
<?
//Muestro el Menu Inicial
include("../abierta/Menu.html");
?>
<br><center><font face="Arial Black" size="6" color="#0000FF">Resultados de la Busqueda</font></center><br>
<?
//recojo los datos en variables
$asunto = $_REQUEST['asunto'];
$empleado = $_REQUEST['empleado'];
$pregunta = $_REQUEST['pregunta'];
$respuesta = $_REQUEST['respuesta'];

//creo la conexion y ejecuto el insert
if(!($conexion = odbc_connect("pidirect","",""))){
   die("Error: No se ha podido conectar a la base de datos");
}
else{
   // Primero me monto la select
   $query1 = "select * from BuscarConsultas where ";
   $cant = 0;
   if(($asunto != null)&&($asunto != "")){ //si se ha introducido el asunto
      $cant = 1;
      $query1 = $query1."asunto like '%".$asunto."%' ";
   }
   if(($empleado != null)&&($empleado != "")){ //si se ha introducido el empleado
      if($cant == 0){
         $cant = 1;
         $query1 = $query1."nombreEmp like '%".$empleado."%' ";
      }
      else{
         $query1 = $query1."and nombreEmp like '%".$empleado."%' ";
      }
   }
   if(($pregunta != null)&&($pregunta != "")){//si se ha introducido la pregunta
      if($cant == 0){
         $cant = 1;
         $query1 = $query1."txtpregunta like '%".$pregunta."%' ";
      }
      else{
         $query1 = $query1."and txtpregunta like '%".$pregunta."%' ";
      }
   }
   if(($respuesta != null)&&($respuesta != "")){ //si se ha introducido la respuesta
      if($cant == 0){
         $cant = 1;
         $query1 = $query1."txtRespuesta like '%".$respuesta."%' ";
      }
      else{
         $query1 = $query1."and txtRespuesta like '%".$respuesta."%' ";
      }
   }
   
   if(!($rs1=odbc_exec($conexion, $query1))){
      print ("<center>Ha ocurrido un error interno, disculpe las molestias</center><br>");
      print ("<center><a href=\"Principal.php\">INTENTAR DE NUEVO</a></center><br>");
   }
   else{
      if(odbc_fetch_row($rs1)){ // si la select ha devuelto filas de datos
         print ("<table border=1 width='100%'>");
         print ("<tr><td><b>FECHA ENVIO</b></td><td><b>CLIENTE QUE LA ENVIÓ</b></td>
               <td><b>EMPLEADO QUE LA RESPONDIÓ</b></td><td><b>ASUNTO</b></td><td><b>PREGUNTA</b></td>
               <td><b>RESPUESTA</b></td><td><b>RESPONDIDA EL</b></td></tr>");
         do{
            $rs1_fechaenv=odbc_result($rs1, "fechaEnvio");
            $rs1_nomcli=odbc_result($rs1, "nombreCli");
            $rs1_nomemp=odbc_result($rs1, "nombreEmp");
            $rs1_asun=odbc_result($rs1, "asunto");
            $rs1_preg=odbc_result($rs1, "txtpregunta");
            $rs1_resp=odbc_result($rs1, "txtRespuesta");
            $rs1_fechares=odbc_result($rs1, "fechaRespuesta");
            print ("<tr>");
            print ("<td>$rs1_fechaenv</td>");
            print ("<td>$rs1_nomcli</td>");
            print ("<td>$rs1_nomemp</td>");
            print ("<td>$rs1_asun</td>");
            print ("<td>$rs1_preg</td>");
            print ("<td>$rs1_resp</td>");
            print ("<td>$rs1_fechares</td>");
            print ("</tr>");
         }while (odbc_fetch_row($rs1));
         print ("</table>");
      }
      else{
	 print ("<p align=\"center\"><b><font color=\"#ff0000\">No Hay Consultas que coincidan con los datos introducidos.</font></b></p><br>");
      }
   }
   odbc_close($conexion);
}
?>
<br>
<table border="0" width="490" align="center">
   <tr>
      <td height="111" width="476" colspan="2" style="background-color: #C0C0C0">
	 <form name="FormBuscar" method="GET" action="BuscarConsultas.php" onsubmit="return EnviaFormBuscar();">
	 <table border="0" height="111" width="460" align="center">
            <tr>
               <td colspan="2">&nbsp;&nbsp;&nbsp;&nbsp; <b><u>BUSCADOR DE CONSULTAS</u></b>
               <font color="#424142">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	       (escriba las palabras clave)</font></td>
            </tr>
            <tr>
               <td width="221">asunto<input type="text" name="asunto" size="26"></td>
               <td width="223">empleado<input type="text" name="empleado" size="23"></td>
            </tr>
            <tr>
               <td width="221">pregunta<input type="text" name="pregunta" size="24"></td>
               <td width="223">respuesta<input type="text" name="respuesta" size="23"></td>
            </tr>
            <tr>
               <td width="444" colspan="2">
               <p align="center"><button type="submit" style="font-weight: bold">BUSCAR</button></td>
            </tr>
         </table>
         </form>
      </td>
   </tr>
</table>
</body>
</html>
