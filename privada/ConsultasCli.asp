<!-- #include file="Menu.html" -->
<title>PIDIRECT - Consultas</title>
<%
'Comprobamos que hayan iniciado sesion
if session("dni")=Empty or session("nombre")=Empty then
	response.Redirect("../abierta/Login.php?id=cli&msg=noid")
else
	dni=session("dni")
	nombre=session("nombre")
end if

'Comprobamos si se quiere ver las consultas o hacer una nueva
sID=request.queryString("id")

if (sID="ver") then
  'Si queremos ver las consultas que hemos hecho
  'Hay que mostrar los datos de las consultas hechas por ese cliente

  ' Creamos conexion y recorset

  Set Conn = Server.CreateObject("ADODB.Connection")
  Set Rs   = Server.CreateObject("ADODB.RecordSet")

  ' Abrimos conexion
  Conn.Open "pidirect"


  ' preparamos la consulta
  sSQL = "select * from BuscarConsultas where dniCli='" & dni & "'"

  ' La ejecutamos en el recorset creado
  Set Rs = Conn.Execute(sSQL)

  'Comprobamos que haya algun resultado
  if Rs.EOF then
     Response.Write("<br><center><font face='Arial Black' size='5' color='FF0000'>NO Hay Consultas Respondidas</font></center><br>")
     Response.Write("<br><center>Usted todavia no ha realizado ninguna consulta o aun no le han contestado</center>")
     Response.Write("<br><center>Si usted desea realizar una consulta haga click <a href='ConsultasCli.asp?id=nueva'>AQUI</a></center>")
  else
     ' Pintamos resultados
     Response.Write("<br><center><font size='6' color='#D60000'>Cliente "&nombre&"</font> <a href='Logout.asp'>Salir</a><center>")
     Response.Write("<table border='0' align='center' width='100%'>")
     Response.Write("<tr><td colspan='6' align='center'><img src='../imagenes/consultasCli.jpg'></td></tr>")
     Response.Write("<tr>")
     Response.Write("<td style='border-style: solid; border-width: 2px;' bordercolor='#000000' align='center'><b>FECHA ENVIO</b></td>")
     Response.Write("<td style='border-style: solid; border-width: 2px;' bordercolor='#000000' align='center'><b>ASUNTO</b></td>")
     Response.Write("<td style='border-style: solid; border-width: 2px;' bordercolor='#000000' align='center'><b>PREGUNTA</b></td>")
     Response.Write("<td style='border-style: solid; border-width: 2px;' bordercolor='#000000' align='center'><b>RESPUESTA</b></td>")
     Response.Write("<td style='border-style: solid; border-width: 2px;' bordercolor='#000000' align='center'><b>EMPLEADO QUE LA RESPONDIÓ</b></td>")
     Response.Write("<td style='border-style: solid; border-width: 2px;' bordercolor='#000000' align='center'><b>RESPONDIDA EL</b></td></tr>")
     Do While NOT Rs.EOF
       fEnv=Rs.Fields("fechaEnvio").value
       asunto=Rs.Fields("asunto").value
       preg=Rs.Fields("txtpregunta").value
       res=Rs.Fields("txtRespuesta").value
       emp=Rs.Fields("nombreEmp").value
       fRes=Rs.Fields("fechaRespuesta").value
       Response.Write("<tr>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&fEnv&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&asunto&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&preg&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&res&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&emp&"</td>")
       Response.Write("<td style='border-style: solid; border-width: 1px;' bordercolor='#000000'>"&fRes&"</td>")
       Response.Write("</tr>")
       Rs.MoveNext
     Loop
     Response.Write("</table>")
  end if
  ' Finalmente cerramos el RecordSet y lo destruimos
   Rs.Close
   Set Rs = Nothing

  ' Cerramos tambien la conexion
  Conn.Close
  Set Conn = Nothing
else
  'Si queremos hacer una nueva consulta
  if (sID="nueva") then
%>

<!-- MOSTRAMOS EL FORMULARIO PARA ENVIAR LA CONSULTA -->
<br><center><font size="6" color="#D60000">Cliente <%=nombre%></font> <a href="Logout.asp">Salir</a><center>
<form action="NuevaConsulta.asp" method="get" name="FormNuevaConsulta" onsubmit="return EnviaFormNuevaConsulta();">
<table border="0" width="70%" align="center">
	<tr>
		<td colspan="2" align="center"><img src="../imagenes/consultaNueva.jpg"></td>
	</tr>
	<tr>
		<td align="right"><font color="#29309C" size="4"><b>Escriba el ASUNTO</b></font></td>
		<td><input type="text" name="asunto" size="45"></td>
	</tr>
	<tr>
		<td align="right"><font color="#29309C" size="4"><b>Escriba la PREGUNTA</b></font></td>
		<td><textarea name="pregunta" cols="45" rows="5"></textarea></td>
	</tr>
	<tr>
		<td colspan="2" align="center"><button type="submit" style="font-size:18">Enviar Consulta</button></td>
	</tr>
</table>
</form>

<%
  else
    if (sID="ok") then
       Response.Write("<br><center><font face='Arial Black' size='5' color='FF0000'>Consulta Enviada Correctamente</font></center>")
       Response.Write("<br><center>La Consulta se ha enviado correctamente, pronto será contestada.</center>")
       Response.Write("<br><center>Si usted desea realizar otra consulta haga click <a href='ConsultasCli.asp?id=nueva'>AQUI</a></center>")
    end if
    if (sID="nook") then
       error=request.queryString("error")
       Response.Write("<br><center><font face='Arial Black' size='5' color='FF0000'>Ha Ocurrido un Error</font></center>")
       Response.Write("<br><center>La Consulta no se ha enviado correctamente, disculpe las molestias.</center>")
       Response.Write("<br><center>"& error &"</center>")
       Response.Write("<br><center>Por favor, inténtelo de nuevo <a href='ConsultasCli.asp?id=nueva'>AQUI</a></center>")
    end if
  end if
end if
%>

</body>
</html>

