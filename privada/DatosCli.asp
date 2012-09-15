<!-- #include file="Menu.html" -->
<title>PIDIRECT - Datos del CLIENTE</title>
<%
'Comprobamos que hayan iniciado sesion
if session("dni")=Empty or session("nombre")=Empty then
	response.Redirect("../abierta/Login.php?id=cli&msg=noid")
else
	dni=session("dni")
	nombre=session("nombre")
end if

'Hay que mostrar los datos del cliente

 ' Creamos conexion y dos recorset

  Set Conn = Server.CreateObject("ADODB.Connection")
  Set Rs   = Server.CreateObject("ADODB.RecordSet")

 ' Abrimos conexion
 Conn.Open "pidirect"


  ' preparamos la consulta
  sSQL = "select * from clientes where dni='" & dni & "'"

  ' La ejecutamos en el recorset creado
  Set Rs = Conn.Execute(sSQL)

  'Comprobamos que haya algun resultado
  if Rs.EOF then
     Response.Write("<br><center>Ha ocurrido algun error en el servidor</center>")
     Response.Write("<br><center><a href='PrincipalCli.asp'>VOLVER A INTENTAR</a></center>")
  else
%>
<br><center><font size="6" color="#D60000">Cliente <%=nombre%></font> <a href="Logout.asp">Salir</a><center>
<form action="ActDatosCli.asp" name="FormActDatosCli" method="GET" onsubmit="return EnviaFormActDatosCli();">
<table border="0" width="60%" align="center">
	<tr>
		<td colspan="2" align="center"><img src="../imagenes/datosCli.jpg"></td>
	</tr>

<%
  ' Pintamos resultados
  Do While NOT Rs.EOF
     dir=Rs.Fields("direccion").value
     pob=Rs.Fields("poblacion").value
     cp=Rs.Fields("codigopostal").value
     prov=Rs.Fields("provincia").value
     pw=Rs.Fields("contrasenya").value
     Response.Write("<td align='center' bgcolor='#C3C3C3'><b><font color='#000000'>Dirección</font></b></td>")
     Response.Write("<td><input type='text' name='direccion' size='30' value='"&dir&"'></td>")
     Response.Write("</tr>")
     Response.Write("<tr>")
     Response.Write("<td align='center' bgcolor='#C3C3C3'><b><font color='#000000'>Población</font></b></td>")
     Response.Write("<td><input type='text' name='poblacion' size='25' value='"&pob&"'></td>")
     Response.Write("</tr>")
     Response.Write("<tr>")
     Response.Write("<td align='center' bgcolor='#C3C3C3'><b><font color='#000000'>Codigo Postal</font></b></td>")
     Response.Write("<td><input type='text' name='codigopostal' size='5' value='"&cp&"'></td>")
     Response.Write("</tr>")
     Response.Write("<tr>")
     Response.Write("<td align='center' bgcolor='#C3C3C3'><b><font color='#000000'>Provincia</font></b></td>")
     Response.Write("<td><input type='text' name='provincia' size='25' value='"&prov&"'></td>")
     Response.Write("</tr>")
     Response.Write("<tr>")
     Response.Write("<td align='center' bgcolor='#C3C3C3'><b><font color='#000000'>Contrasenya</font></b></td>")
     Response.Write("<td><input type='text' name='contrasenya' size='12' value='"&pw&"'></td>")
     Response.Write("</tr>")
     Rs.MoveNext
  Loop
%>

<tr><td colspan="2" align="center"><button type="submit" style="font-size:18">Modificar Datos</button></td></tr>
</table>
</form>

<%
  ' Finalmente cerramos el RecordSet y lo destruimos
   Rs.Close
   Set Rs = Nothing

  ' Cerramos tambien la conexion
  Conn.Close
  Set Conn = Nothing

end if
%>

</body>
</html>

