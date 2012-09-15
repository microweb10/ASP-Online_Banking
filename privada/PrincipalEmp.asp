<!doctype html public "-//w3c//dtd html 4.0 transitional//en">
<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
   <meta name="Author" content="Julian Nicolas">
   <meta name="Copyright" content="© Julian Nicolas">
   <meta http-equiv="Content-Script-Type" content="JavaScript">
   <title>Pagina Principal de EMPLEADO</title>
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
end if
%>
<!-- TABLA CON LAS CONSULTAS DE CLIENTES SIN CONTESTAR -->
<br><center><font size="6" color="#D60000">Empleado <%=nombre%></font> <a href="Logout.asp">Salir</a><center>
<br><table border="0" align="center" width="90%">
   <tr><td colspan="5" align="center"><img src="../imagenes/consultasSinResp.jpg"></td></tr>
   <tr>
      <td style="border-style: solid; border-width: 2px;" bordercolor="#000000" align="center"><b>FECHA ENVIO</b></td>
      <td style="border-style: solid; border-width: 2px;" bordercolor="#000000" align="center"><b>CLIENTE</b></td>
      <td style="border-style: solid; border-width: 2px;" bordercolor="#000000" align="center"><b>ASUNTO</b></td>
      <td style="border-style: solid; border-width: 2px;" bordercolor="#000000" align="center"><b>PREGUNTA</b></td>
      <td align="center" bgcolor="#FFFFFF"><b><font color="#FFFFFF">[Responder]</font></b></td>
   </tr>
<%
'Hay que mostrar las consultas sin responder

 ' Creamos conexion y recorset

  Set Conn = Server.CreateObject("ADODB.Connection")
  Set Rs   = Server.CreateObject("ADODB.RecordSet")

  ' Abrimos conexion
  Conn.Open "pidirect"


  ' preparamos la consulta
  sSQL = "select * from ConsultasSinResp"

  ' La ejecutamos en el recorset creado
  Set Rs = Conn.Execute(sSQL)

  'Comprobamos que haya algun resultado
  if Rs.EOF then
     Response.Write("<tr><td align='center' colspan='4' style='border-style: solid; border-width: 1px;' bordercolor='#000000'>NO Hay Consultas Sin Responder</td>")
  else
     ' Pintamos resultados
     Do While NOT Rs.EOF
       idCons=Rs.Fields("id").value
       fEnv=Rs.Fields("fechaEnvio").value
       nomCli=Rs.Fields("nombre").value
       asunto=Rs.Fields("asunto").value
       preg=Rs.Fields("txtpregunta").value
       Response.Write("<tr>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&fEnv&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&nomCli&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&asunto&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&preg&"</td>")
       Response.Write("<td align='center'><a href='ResponderCons.asp?idCons="&idCons&"&id=conf'>[Responder]</a></td>")
       Response.Write("</tr>")
       Rs.MoveNext
     Loop
  end if
  Response.Write("</table>")
  ' Cerramos el RecordSet (no lo destruimos, lo utilizaremos despues)
  Rs.Close
%>
<!-- TABLA CON LAS CONSULTAS RESPONDIDAS POR EL EMPLEADO -->
<br><br><table border="0" align="center" width="100%">
   <tr><td colspan="6" align="center"><img src="../imagenes/consultasResp.jpg"></td></tr>
   <tr>
      <td style="border-style: solid; border-width: 2px;" bordercolor="#000000" align="center"><b>FECHA ENVIO</b></td>
      <td style="border-style: solid; border-width: 2px;" bordercolor="#000000" align="center"><b>CLIENTE</b></td>
      <td style="border-style: solid; border-width: 2px;" bordercolor="#000000" align="center"><b>ASUNTO</b></td>
      <td style="border-style: solid; border-width: 2px;" bordercolor="#000000" align="center"><b>PREGUNTA</b></td>
      <td style="border-style: solid; border-width: 2px;" bordercolor="#000000" align="center"><b>MI RESPUESTA</b></td>
      <td align="center" bgcolor="#FFFFFF"><b><font color="#FFFFFF">[Modificar]</font></b></td>
   </tr>
<%
'Hay que mostrar las consultas sin responder

 ' Creamos conexion y recorset

  Set Conn = Server.CreateObject("ADODB.Connection")
  Set Rs   = Server.CreateObject("ADODB.RecordSet")

  ' Abrimos conexion
  Conn.Open "pidirect"


  ' preparamos la consulta
  sSQL = "select * from BuscarConsultas where dniEmp='" & dni & "'"

  ' La ejecutamos en el recorset creado
  Set Rs = Conn.Execute(sSQL)

  'Comprobamos que haya algun resultado
  if Rs.EOF then
     Response.Write("<tr><td align='center' colspan='5' style='border-style: solid; border-width: 1px;' bordercolor='#000000'>NO Hay Consultas Respondidas por usted</td>")
  else
     ' Pintamos resultados
     Do While NOT Rs.EOF
       idCons=Rs.Fields("id").value
       fEnv=Rs.Fields("fechaEnvio").value
       nomCli=Rs.Fields("nombreCli").value
       asunto=Rs.Fields("asunto").value
       preg=Rs.Fields("txtpregunta").value
       resp=Rs.Fields("txtRespuesta").value
       Response.Write("<tr>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&fEnv&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&nomCli&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&asunto&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&preg&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&resp&"</td>")
       Response.Write("<td align='center'><a href='ModificarCons.asp?idCons="&idCons&"&id=conf'>[Modificar]</a></td>")
       Response.Write("</tr>")
       Rs.MoveNext
     Loop
  end if
  Response.Write("</table>")

  ' Finalmente cerramos el RecordSet y lo destruimos
   Rs.Close
   Set Rs = Nothing

  ' Cerramos tambien la conexion
  Conn.Close
  Set Conn = Nothing
%>

</body>
</html>

