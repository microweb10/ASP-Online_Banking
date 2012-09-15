<!doctype html public "-//w3c//dtd html 4.0 transitional//en">
<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
   <meta name="Author" content="Julian Nicolas">
   <meta name="Copyright" content="© Julian Nicolas">
   <meta http-equiv="Content-Script-Type" content="JavaScript">
   <title>Bienvenido a PIDIRECT</title>
   <script src="../javascript/ValidaForm.js"></script>
</head>
<body>
<table border="0" width="980" height="555">
   <tr>
      <td height="114" width="980" colspan="5">
         <p align="center"><img src="../imagenes/logo.jpg"></p>
      </td>
   </tr>
   <tr>
      <td height="40" width="974" colspan="5">
         <p align="center">
	    <a href="../abierta/Principal.php" style="color:#000000;font-weight:bold;">INICIO</a>&nbsp;&nbsp;
	    <a href="../abierta/Somos.php" style="color:#000000;font-weight:bold;">¿Quienes Somos?</a>&nbsp;&nbsp;
	    <a href="../abierta/LLegar.php" style="color:#000000;font-weight:bold;">¿Como LLegar?</a>&nbsp;&nbsp;
	    <a href="../abierta/Contacta.php" style="color:#000000;font-weight:bold;">Contacta Con Nosotros</a>
      </td>
   </tr>
   <tr>
      <td height="26" width="193"></td>
      <td height="26" width="280">
         <p align="right"><a href="Login.php?id=cli&msg=" style="font-weight: bold; background-color: #000080; color:#FFFF00; text-decoration:none;">Acceso a Clientes</a>
      </td>
      <td height="26" width="40"></td>
      <td height="26" width="190">
         <a href="Login.php?id=emp&msg=" style="font-weight: bold; background-color: #000080; color:#FFFF00; text-decoration:none;">Acceso a Empleados</a>
      </td>
      <td height="26" width="256"></td>
   </tr>
   <tr>
      <td height="277" width="476" colspan="2" style="background-color: #DBDBDB">
         <img border="0" src="../imagenes/quieres.jpg" height="130" style="position: absolute; left: 300; top: 214; width: 179px">
         <p style="margin-top: 1px; margin-bottom: 0; font-weight:bold;">
         <form name="FormSolicitud" method="POST" action="EnviarSolicitud.php" onsubmit="return EnviaFormSolicitud();">
         DNI:
         <input type="text" name="dni" size="10"><br>
         Nombre:
         <input type="text" name="nombre" size="25"><br>
         Dirección:
         <input type="text" name="direccion" size="30"><br>
         Población:
         <input type="text" name="poblacion" size="25"><br>
         Codigo Postal:
         <input type="text" name="codpostal" size="5"><br>
         Provincia:
         <input type="text" name="provincia" size="20"><br>
         Email:
         <input type="text" name="email" size="30"><br>
         Observaciones:<textarea rows="4" name="observaciones" cols="50"></textarea><br>
         <button type="submit" style="font-weight: bold">Enviar Solicitud</button>&nbsp;&nbsp;&nbsp;
         Envie sus datos y pronto recibirá respuesta
         </form></p>
      </td>
      <td height="277" width="40"></td>
      <td height="277" width="460" colspan="2">
         <table border="1" width="100%">
            <tr>
               <td colspan="2">
	          <p align="center"><img src="../imagenes/servicios.jpg"></p>
	       </td>
            </tr>
            <tr>
               <td><p align="center"><b>Tipo de Cuenta</b></p></td>
               <td><p align="center"><b>Movimientos permitidos para la Cuenta</b></p></td>
            </tr>
<?
if(!($conexion = odbc_connect("pidirect","",""))){
   die("Error: No se ha podido conectar a la base de datos");
}
else{
   $query1 = "select * from tiposCuentas";

   $rs1 = odbc_exec($conexion, $query1);

   if (odbc_num_rows($rs1) == 0){
       print ("<CENTER><B>No hay Servicios Disponibles</B></CENTER>");
   }
   else{
      while (odbc_fetch_row($rs1)){
         $rs1_id=odbc_result($rs1, "id");
         $rs1_nom=odbc_result($rs1, "nombreTipoCuenta");
         print ("<tr><td>$rs1_nom</td>");
         print ("<td>");
	    $query2 = "select * from Servicios where id=$rs1_id";
	    $rs2 = odbc_exec($conexion, $query2);
	    if (odbc_num_rows($rs2) == 0){
               print ("<CENTER><B>No hay movimientos para la cuenta</B></CENTER>");
            }
            else{
	       while (odbc_fetch_row($rs2)){
		  $rs2_nom=odbc_result($rs2,"nombreTipoMov");
                  print ("-$rs2_nom-");
	       }
	    }
         print ("</td></tr>");
      }
   }
   odbc_close($conexion);
}
?>
         </table>
      </td>
   </tr>
   <tr>
      <td height="111" width="476" colspan="2" style="background-color: #C0C0C0">
	 <form name="FormBuscar" method="GET" action="BuscarConsultas.php" onsubmit="return EnviaFormBuscar();">
	 <table border="0" height="111" width="460">
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
               <td width="221">pregunta<input type="text" name="pregunta" size="23"></td>
               <td width="223">respuesta<input type="text" name="respuesta" size="23"></td>
            </tr>
            <tr>
               <td width="444" colspan="2">
               <p align="center"><button type="submit" style="font-weight: bold">BUSCAR</button></td>
            </tr>
         </table>
         </form>
      </td>
      <td height="111" width="-6"></td>
      <td height="111" width="460" colspan="2" style="background-color: #000000">
         <form name="FormComision" method="GET" action="../cgi/Comision.exe" onsubmit="return EnviaFormComision();">
         <table border="0" height="111" width="460">
            <tr>
               <td rowspan="3"><img border="0" src="../imagenes/comisiones.jpg"></td>
               <td><font color="#FFFFFF">Tipo de Comision&nbsp;</font>
                  <select name="comision" size="1">
                     <option value="10"> A (10%) </option>
                     <option value="14"> B (14%) </option>
                     <option value="19"> C (19%) </option>
                  </select>
	       </td>
            </tr>
            <tr>
               <td width="284"><font color="#FFFFFF">Cantidad en Euros&nbsp;</font>
                  <input type="text" name="cantidad" size="20">
	       </td>
            </tr>
            <tr>
               <td width="284">
               <p align="center"><button type="submit" style="font-weight: bold">CALCULAR COMISION</button></td>
            </tr>
         </table>
         </form>
      </td>
   </tr>
</table>

</body>
</html>

