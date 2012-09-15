<!-- #include file="Menu.html" -->
<title>PIDIRECT - Tus Comisiones Pagadas por Movimiento</title>
<%
'Comprobamos que hayan iniciado sesion
if session("dni")=Empty or session("nombre")=Empty then
	response.Redirect("../abierta/Login.php?id=cli&msg=noid")
else
	dni=session("dni")
	nombre=session("nombre")
	'recibo tambien los parametros
	fIni=request.queryString("fInicio")
	fFin=request.queryString("fFinal")
end if
%>
<br><center><font size="6" color="#D60000">Cliente <%=nombre%></font> <a href="Logout.asp">Salir</a><center>
<table border="0" width="50%" align="center">
	<tr>
		<td colspan="3">
		<p align="center"><img src="../imagenes/comisionesCli.jpg"></td>
	</tr>
	<tr>
		<td align="center" bgcolor="#666666"><b><font color="#FFFFFF">TIPO DE MOVIMIENTO</font></b></td>
		<td align="center" bgcolor="#666666"><b><font color="#FFFFFF">%</font></b></td>
		<td align="center" bgcolor="#666666"><b><font color="#FFFFFF">COMISIÓN PAGADA</font></b></td>
	</tr>
	
<%
'Hay que mostrar las comisiones del cliente

 ' Creamos conexion y dos recorset

  Set Conn = Server.CreateObject("ADODB.Connection")
  Set Rs   = Server.CreateObject("ADODB.RecordSet")

 ' Abrimos conexion
 Conn.Open "pidirect"

  ' preparamos la consulta
  sSQL = "select ComisionesCli.dni as dni, ComisionesCli.movimiento as movimiento, ComisionesCli.Comision as Comision, Sum(ComisionesCli.total) AS total "
  sSQL = sSQL & "from ComisionesCli where ((movimientos.fecha>=#"&fIni&"#) and (movimientos.fecha<=#"&fFin&"#)) and dni='"&dni&"' "
  sSQL = sSQL & "group by ComisionesCli.dni, ComisionesCli.movimiento, ComisionesCli.Comision"

  ' La ejecutamos en el recorset creado
  Set Rs = Conn.Execute(sSQL)

  'Comprobamos que haya algun resultado
  if Rs.EOF then
     Response.Write("<tr><td colspan='3' align='center' bgcolor='#C0C0C0'>No hay Comisiones para mostrar</td></tr>")
  else
     ' Pintamos resultados
     Do While NOT Rs.EOF
       mov=Rs.Fields("movimiento").value
       com=Rs.Fields("total").value
       tipo=Rs.Fields("Comision").value
       Response.Write("<tr>")
       Response.Write("<td align='center' bgcolor='#C0C0C0'>"&mov&"</td>")
       Response.Write("<td align='center' bgcolor='#C0C0C0'>"&tipo&"</td>")
       Response.Write("<td align='center' bgcolor='#C0C0C0'>"&com&" €</td>")
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
<br><br>
<form action="Comisiones.asp" method="GET" name="FormComisiones" onsubmit="return EnviaFormComisiones();">
<table border="0" align="center" width="600">
	<tr>
		<td bgcolor="#666666" align="center"><button type="submit">Ver Comisiones Pagadas</button></td>
		<td bgcolor="#666666"><font color="#FFFFFF">&nbsp; entre la fecha &nbsp;<input type="text" name="fInicio" size="10"></font></td>
		<td bgcolor="#666666"><font color="#FFFFFF">&nbsp; y la fecha &nbsp;<input type="text" name="fFinal" size="10"></font></td>
	</tr>
</table>
</form>

</body>
</html>

