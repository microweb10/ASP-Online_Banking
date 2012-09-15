<!doctype html public "-//w3c//dtd html 4.0 transitional//en">
<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
   <meta name="Author" content="Julian Nicolas">
   <meta name="Copyright" content="© Julian Nicolas">
   <meta http-equiv="Content-Script-Type" content="JavaScript">
   <title>PIDIRECT - Envio Solicitud de Cliente</title>
</head>
<body>
<?
//Muestro el Menu Inicial
include("../abierta/Menu.html");
//recojo los datos en variables
$dni = $_REQUEST['dni'];
$nombre = $_REQUEST['nombre'];
$direccion = $_REQUEST['direccion'];
$poblacion = $_REQUEST['poblacion'];
$codpostal = $_REQUEST['codpostal'];
$provincia = $_REQUEST['provincia'];
$email = $_REQUEST['email'];
$observaciones = $_REQUEST['observaciones'];

//creo la conexion y ejecuto el insert
if(!($conexion = odbc_connect("pidirect","",""))){
   die("Error: No se ha podido conectar a la base de datos");
}
else{
   $query1 = "insert into solicitudes (dni,nombre,direccion,poblacion,codigopostal,provincia,email,observaciones) values ('$dni','$nombre','$direccion','$poblacion','$codpostal','$provincia','$email','$observaciones')";

   //comprobamos que no haya enviado antes una solicidud con el mismo DNI
   $query2 = "select * from solicitudes where dni='$dni'";
   $rs2 = odbc_exec($conexion,$query2);
   if(odbc_fetch_row($rs2)){
      print ("<center><font face=\"Arial Black\" size=\"6\" color=\"#FF0000\">Los Datos NO Se Han Enviado</font></center><br>");
      print ("<center>Ya se habia realizado antes una solicitud con el mismo DNI</center><br>");
      print ("<center><a href=\"Principal.php\">REALIZAR UNA SOLICITUD NUEVA</a></center><br>");
   }
   else{
      if(!(odbc_exec($conexion, $query1))){
         print ("<center><font face=\"Arial Black\" size=\"6\" color=\"#FF0000\">Los Datos NO Se Han Enviado</font></center><br>");
         print ("<center>Ha ocurrido un error interno, disculpe las molestias</center><br>");
         print ("<center><a href=\"Principal.php\">INTENTAR DE NUEVO</a></center><br>");
      }
      else{
         print ("<center><font face=\"Arial Black\" size=\"6\" color=\"#0000FF\">Datos Enviados Correctamente</font></center><br>");
         print("<center> DNI: <b> $dni </b> </center><br>");
         print("<center> Nombre: <b> $nombre </b> </center><br>");
         print("<center> Direccion: <b> $direccion </b> </center><br>");
         print("<center> Poblacion: <b> $poblacion </b> </center><br>");
         print("<center> CP: <b> $codpostal </b> </center><br>");
         print("<center> Provincia: <b> $provincia </b> </center><br>");
         print("<center> Email: <b> $email </b> </center><br>");
         print("<center> Observaciones: <b> $observaciones </b> </center><br>");
      }
   }
   odbc_close($conexion);
}
?>
</body>
</html>
