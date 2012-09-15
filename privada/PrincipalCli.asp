<!-- #include file="Menu.html" -->
<title>Pagina Principal de CLIENTE</title>
<%
'Comprobamos que hayan iniciado sesion
if session("dni")=Empty or session("nombre")=Empty then
	response.Redirect("../abierta/Login.php?id=cli&msg=noid")
else
	dni=session("dni")
	nombre=session("nombre")
end if

'Hay que mostrar las cuentas del cliente

 ' Creamos conexion y dos recorset

  Set Conn = Server.CreateObject("ADODB.Connection")
  Set Rs   = Server.CreateObject("ADODB.RecordSet")
  Set Rs2   = Server.CreateObject("ADODB.RecordSet")

 ' Abrimos conexion
 Conn.Open "pidirect"


  ' preparamos la consulta
  sSQL = "select * from CuentasCli where dni='" & dni & "'"

  ' La ejecutamos en el recorset creado
  Set Rs = Conn.Execute(sSQL)

  'Comprobamos que haya algun resultado
  if Rs.EOF then
     Response.Write("<br><center>Usted No tiene Ninguna Cuenta en PIDIRECT</center>")
  else
%>
<br><center><font size="6" color="#D60000">Cliente <%=nombre%></font> <a href="Logout.asp">Salir</a><center>
<table border="0" width="90%" align="center">
	<tr>
		<td colspan="5" align="center"><img src="../imagenes/cuentas.jpg"></td>
	</tr>
	<tr>
		<td align="center" bgcolor="#0000FF" style="border-style: outset; border-width: 3px" bordercolor="#000000">
		<b><font color="#FFFFFF">Tipo De Cuenta</font></b></td>
		<td align="center" bgcolor="#0000FF" style="border-style: outset; border-width: 3px" bordercolor="#000000">
		<b><font color="#FFFFFF">Numero De Cuenta</font></b></td>
		<td align="center" bgcolor="#0000FF" style="border-style: outset; border-width: 3px" bordercolor="#000000">
		<b><font color="#FFFFFF">Saldo de la Cuenta</font></b></td>
		<td align="center" bgcolor="#FFFFFF"><b><font color="#FFFFFF">[Ver Movimientos]</font></b></td>
		<td align="center" bgcolor="#FFFFFF"><b><font color="#FFFFFF">[Realizar Movimientos]</font></b></td>
	</tr>

<%
  ' Pintamos resultados
  Do While NOT Rs.EOF
     tCuenta=Rs.Fields("nombreTipoCuenta").value
     nCuenta=Rs.Fields("numero").value
     sCuenta="0"
     sSQL2="select * from SaldoCuenta where numero=" & nCuenta & ""
     Set Rs2 = Conn.Execute(sSQL2)
     Do While NOT Rs2.EOF
	sCuenta=Rs2.Fields("saldo").value
	Rs2.MoveNext
     Loop
     Response.Write("<tr>")
     Response.Write("<td  align='center' bgcolor='#D7D7D7' style='border-style: outset; border-width: 3px' bordercolor='#000000'>"&tCuenta&"</td>")
     Response.Write("<td  align='center' bgcolor='#D7D7D7' style='border-style: outset; border-width: 3px' bordercolor='#000000'>"&nCuenta&"</td>")
     Response.Write("<td  align='center' bgcolor='#D7D7D7' style='border-style: outset; border-width: 3px' bordercolor='#000000'><b>"&sCuenta&" €</b></td>")
     Response.Write("<td align='center'><a href='Movimientos.asp?ccc="&nCuenta&"&tipo="&tCuenta&"&ver=TODOS&ord=fAsc'>[Ver Movimientos]</a></td>")
     Response.Write("<td><a href='NuevoMov.asp?ccc="&nCuenta&"&tipoCuen="&tCuenta&"&id=conf'>[Realizar Movimiento]</a></td>")
     Response.Write("</tr>")
     Rs.MoveNext
  Loop

  Response.Write("</table>")
%>
<br><br><p align="center"> ¿Quieres ver tus Comisiones pagadas por tipo de movimiento?</p>
<form action="Comisiones.asp" method="GET" name="FormComisiones" onsubmit="return EnviaFormComisiones();">
<table border="0" align="center" width="600">
	<tr>
		<td bgcolor="#666666" align="center"><button type="submit">Ver Comisiones Pagadas</button></td>
		<td bgcolor="#666666"><font color="#FFFFFF">&nbsp; entre la fecha &nbsp;<input type="text" name="fInicio" size="10"></font></td>
		<td bgcolor="#666666"><font color="#FFFFFF">&nbsp; y la fecha &nbsp;<input type="text" name="fFinal" size="10"></font></td>
	</tr>
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
%></body></html>
