<!doctype html public "-//w3c//dtd html 4.0 transitional//en">
<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
   <meta name="Author" content="Julian Nicolas">
   <meta name="Copyright" content="© Julian Nicolas">
   <meta http-equiv="Content-Script-Type" content="JavaScript">
   <title>Pagina de LOGIN (Identifícate)</title>
   <script src="../javascript/ValidaForm.js"></script>
</head>
<body>
<?   //Muestro el Menu Inicial
include("../abierta/Menu.html");
     //recojo los datos en variables
$id = $_REQUEST['id'];
$mostrar = "";
if($id=="cli") $mostrar = "Acceso a CLIENTES";
if($id=="emp") $mostrar = "Acceso a EMPLEADOS";
?>
<br><center><font face="Arial Black" size="6" color="#0000FF"><?=$mostrar?></font></center><br>
<!-- FORMULARIO DE LOGIN - POST PARA NO MOSTRAR LA CONTRASENYA -->
<form name="FormLogin"  action="../privada/Login.asp?id=<?=$id?>"  method="post" onsubmit="return EnviaFormLogin();">
<table border="0" width="30%" align="center">
<tr>
       <td align="center"> <font size="4" color="Gray"> <b> D.N.I. </b> </font> </td>
       <td align="center"> <input name="dni" type="text"> </td>
</tr>
<tr>
       <td align="center"> <font size="4" color="Black"> <b> Contraseña </b> </font> </td>
       <td align="center"> <input name="password" type="password"> </td>
</tr>
<tr>
	<td align="center" colspan="2">
		<button style="font-weight: bold" type="submit">ENTRAR</button>
	</td>
</tr>
</table>
</form>
<? if($_REQUEST['msg']=="nocli")
      print("<script>alert(\"DNI o contrasenya de CLIENTE INCORRECTA\");</script>");
   if($_REQUEST['msg']=="noemp")
      print("<script>alert(\"DNI o contrasenya de EMPLEADO INCORRECTA\");</script>");
   if($_REQUEST['msg']=="noid")
      print("<script>alert(\"Ha intentado acceder a una pagina de uso privado. Identifíquese\");</script>");
?>
</body>
</html>
