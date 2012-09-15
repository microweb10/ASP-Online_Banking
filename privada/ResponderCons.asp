<!doctype html public "-//w3c//dtd html 4.0 transitional//en">
<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
   <meta name="Author" content="Julian Nicolas">
   <meta name="Copyright" content="© Julian Nicolas">
   <meta http-equiv="Content-Script-Type" content="JavaScript">
   <title>PIDIRECT - Responder Consulta</title>
   <script src="../javascript/ValidaForm.js"></script>
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
if session("dni")=Empty or session("nombre")=Empty then
	response.Redirect("../abierta/Login.php?id=emp&msg=noid")
else
	dni=session("dni")
	nombre=session("nombre")
	' Recibo tambien los parametros
       	cons=request.queryString("idCons")
	id=request.queryString("id")
end if

' Creamos conexion y recorset

Set Conn = Server.CreateObject("ADODB.Connection")
Set Rs   = Server.CreateObject("ADODB.RecordSet")

' Abrimos conexion
Conn.Open "pidirect"


' Vemos si ya esta configurado el movimiento o hay que configurarlo
if (id="ok") then
  ' Tenemos que insertar el movimiento en la base de datos
  ' recibo los demas parametros
  resp=request.queryString("respuesta")

  ' Preparo la sentencia
  sSQL= "update consultas set dniEmp='"&dni&"',fechaRespuesta=#"&Now()&"#,txtRespuesta='"&resp&"' where id="&cons&""

    Conn.execute(sSQL)
    
  on error resume next ' SI OCURRE ALGUN ERROR CONTINUA
  if Err.number <> 0  then
    response.Write("Ha ocurrido el siguiente ERROR: "&Err.description&"")
  else
    response.Redirect("PrincipalEmp.asp")
  end if

  Err.clear
  On error Goto 0  'QUE NO SE OLVIDE ESO PARA QUE VUELVA OTRA VEZ

else
%>
<br><center><font size="6" color="#D60000">Empleado <%=nombre%></font> <a href="Logout.asp">Salir</a><center>
<br><table border="0" width="50%" align="center">
	<tr>
		<td align="center" colspan="2"><img src="../imagenes/responderCons.jpg"></td>
	</tr>
	<form action="ResponderCons.asp" method="GET" name="FormRespCons" onsubmit="return EnviaFormRespCons();">
	<input type="hidden" name="id" value="ok">
	<input type="hidden" name="idCons" value="<%=cons%>">

<%
  ' necesitamos extraer los datos de la consulta

  ' preparamos la consulta
  sSQL = "select * from ConsultasSinResp where id="&cons&""

  ' La ejecutamos en el recorset creado
  Set Rs = Conn.Execute(sSQL)

  if NOT Rs.EOF then
    fEnv=Rs.Fields("FechaEnvio").value
    nomCli=Rs.Fields("nombre").value
    asunto=Rs.Fields("asunto").value
    preg=Rs.Fields("txtpregunta").value
    Response.Write("<tr>")
    Response.Write("<td><b>FECHA DE ENVIO:</b></td>")
    Response.Write("<td>"&fEnv&"</td>")
    Response.Write("</tr>")
    Response.Write("<tr>")
    Response.Write("<td><b>CLIENTE:</b></td>")
    Response.Write("<td>"&nomCli&"</td>")
    Response.Write("</tr>")
    Response.Write("<tr>")
    Response.Write("<td><b>ASUNTO:</b></td>")
    Response.Write("<td>"&asunto&"</td>")
    Response.Write("</tr>")
    Response.Write("<tr>")
    Response.Write("<td><b>PREGUNTA:</b></td>")
    Response.Write("<td>"&preg&"</td>")
    Response.Write("</tr>")
  end if

  ' Cerramos el RecordSet (no lo destruimos, lo haremos despues)
   Rs.Close
%>
	<tr>
		<td><b>RESPUESTA:</b></td>
		<td><textarea name="respuesta" rows="3" cols="45"></textarea></td>
	</tr>
        <tr><td align="center" colspan="2"><button type="submit" style="font-size:18">Responder</button></tr>
</form></table>
<center><a href="PrincipalEmp.asp">ATRÁS</a></center>
<%
end if

  ' Finalmente destruimos el RecordSet
   Set Rs = Nothing

  ' Cerramos tambien la conexion
  Conn.Close
  Set Conn = Nothing
%>
</body>
</html>

