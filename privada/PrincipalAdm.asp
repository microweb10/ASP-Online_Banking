<!doctype html public "-//w3c//dtd html 4.0 transitional//en">
<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
   <meta name="Author" content="Julian Nicolas">
   <meta name="Copyright" content="© Julian Nicolas">
   <meta http-equiv="Content-Script-Type" content="JavaScript">
   <title>Pagina Principal del ADMINISTRADOR</title>
</head>
<body>
<!-- MUESTRO LA CABECERA Y EL MENU GENERAL-->
<table border="0" width="980" height="155">
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
</table>
<%
'Comprobamos que hayan iniciado sesion
if session("id_usuario")=Empty then
	response.Redirect("../abierta/Login.php?id=emp&msg=noid")
else
%>
<br><center><font size="6" color="#D60000">Ha entrado como ADMINISTRADOR </font> <a href="Logout.asp">Salir</a><center>
<br><center><img src="../imagenes/underConstruc.gif"></center>
<%
end if
%>

</body>
</html>

