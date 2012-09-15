<!-- #include file="Menu.html" -->
<title>PIDIRECT - Movimientos de Cuenta</title>
<script src="../javascript/OrdenaTabla.js"></script>
<%
'Comprobamos que hayan iniciado sesion
if session("dni")=Empty or session("nombre")=Empty then
	response.Redirect("../abierta/Login.php?id=cli&msg=noid")
else
	dni=session("dni")
	nombre=session("nombre")
	'recibo tambien los parametros
	cuenta=request.queryString("ccc")
	tipo=request.queryString("tipo")
	ver=request.queryString("ver")
	ord=request.queryString("ord")
end if
%>
<br><center><font size="6" color="#D60000">Cliente <%=nombre%></font> <a href="Logout.asp">Salir</a><center>
<table border="0" width="90%" align="center">
<tbody id="TablaMov">
	<tr>
		<td align="center" colspan="5"><img src="../imagenes/movimientos.jpg"></td>
	</tr>
	<tr>
	        <td align="center" colspan="5"><form action="Movimientos.asp" method="GET">
		<input type="hidden" name="ccc" value="<%=cuenta%>">
		<input type="hidden" name="tipo" value="<%=tipo%>">
		<input type="hidden" name="ord" value="<%=ord%>">
		VER TIPOS DE MOVIMIENTO <select name="ver" onchange="form.submit();" size="1">
<%
  ' necesitamos mostrar en el select los distintos tipos de movimientos
  ' Creamos conexion y dos recorset

  Set Conn = Server.CreateObject("ADODB.Connection")
  Set Rs   = Server.CreateObject("ADODB.RecordSet")

  ' Abrimos conexion
  Conn.Open "pidirect"
  
  ' preparamos la consulta
  sSQL = "select * from tiposMovimientos"

  ' La ejecutamos en el recorset creado
  Set Rs = Conn.Execute(sSQL)
  
  if (ver="TODOS") then
    Response.Write("<option value='TODOS' selected>TODOS</option>")
  else
    Response.Write("<option value='TODOS'>TODOS</option>")
  end if
  
  Do While NOT Rs.EOF
    mov=Rs.Fields("nombreTipoMov").value
    if (mov=ver) then
      Response.Write("<option value='"&mov&"' selected>"&mov&"</option>")
    else
      Response.Write("<option value='"&mov&"'>"&mov&"</option>")
    end if
    Rs.MoveNext
  Loop
  
  ' Cerramos el RecordSet (no lo destruimos, lo utilizaremos despues)
   Rs.Close
%>
		</select>
		</form></td>
	</tr>
	<tr>
		<td align="center" colspan="5" bgcolor="#000080"><font color="#FFFFFF" size="5"><b><%=tipo%></b> CCC <b><%=cuenta%></b></font></td>
	</tr>
	<form action="Movimientos.asp" method="GET" name="form1"><input type="hidden" name="ccc" value="<%=cuenta%>"><input type="hidden" name="tipo" value="<%=tipo%>"><input type="hidden" name="ord" value="fAsc"><input type="hidden" name="ver" value="<%=ver%>"></form>
	<form action="Movimientos.asp" method="GET" name="form2"><input type="hidden" name="ccc" value="<%=cuenta%>"><input type="hidden" name="tipo" value="<%=tipo%>"><input type="hidden" name="ord" value="fDesc"><input type="hidden" name="ver" value="<%=ver%>"></form>
	<form action="Movimientos.asp" method="GET" name="form3"><input type="hidden" name="ccc" value="<%=cuenta%>"><input type="hidden" name="tipo" value="<%=tipo%>"><input type="hidden" name="ord" value="cAsc"><input type="hidden" name="ver" value="<%=ver%>"></form>
	<form action="Movimientos.asp" method="GET" name="form4"><input type="hidden" name="ccc" value="<%=cuenta%>"><input type="hidden" name="tipo" value="<%=tipo%>"><input type="hidden" name="ord" value="cDesc"><input type="hidden" name="ver" value="<%=ver%>"></form>
	<tr>
		<td align="center" bgcolor="#000000"><img src="../imagenes/asc.jpg" onclick="form1.submit();"><font color="#FFFFFF"><b> FECHA </b></font><img src="../imagenes/desc.jpg" onclick="form2.submit();"></td>
		<td align="center" bgcolor="#000000"><font color="#FFFFFF"><b>MOVIMIENTO</b></font></td>
		<td align="center" bgcolor="#000000"><font color="#FFFFFF"><b>CONCEPTO</b></font></td>
		<td align="center" bgcolor="#000000"><img src="../imagenes/asc.jpg" onclick="form3.submit();"><font color="#FFFFFF"><b> CANTIDAD </b></font><img src="../imagenes/desc.jpg" onclick="form4.submit();"></td>
		<td align="center" bgcolor="#000000"><font color="#FFFFFF"><b>% COMISIÓN</b></font></td>
	</tr>
	
<%
'Hay que mostrar los movimientos de la cuenta

  ' preparamos la consulta
  if (ord="fAsc") then sSQL = "select * from MovimientosCuentafAsc where cuenta="&cuenta&"" end if
  if (ord="fDesc") then sSQL = "select * from MovimientosCuentafDesc where cuenta="&cuenta&"" end if
  if (ord="cAsc") then sSQL = "select * from MovimientosCuentacAsc where cuenta="&cuenta&"" end if
  if (ord="cDesc") then sSQL = "select * from MovimientosCuentacDesc where cuenta="&cuenta&"" end if
  if NOT (ver="TODOS") then
    sSQL = sSQL&" and movimiento='"&ver&"'"
  end if


  ' La ejecutamos en el recorset creado
  Set Rs = Conn.Execute(sSQL)

  'Comprobamos que haya algun resultado
  if Rs.EOF then
     Response.Write("<tr><td align='center' bgcolor='#C0C0C0' colspan='5'>No hay Movimientos para mostrar</td></tr>")
  else
     ' Pintamos resultados
     Do While NOT Rs.EOF
       fecha=Rs.Fields("fecha").value
       mov=Rs.Fields("movimiento").value
       con=Rs.Fields("concepto").value
       cant=Rs.Fields("cantidad").value
       com=Rs.Fields("Comision").value
       Response.Write("<tr>")
       Response.Write("<td align='center' bgcolor='#C0C0C0'>"&fecha&"</td>")
       Response.Write("<td align='center' bgcolor='#C0C0C0'>"&mov&"</td>")
       Response.Write("<td align='center' bgcolor='#C0C0C0'>"&con&"</td>")
       Response.Write("<td align='center' bgcolor='#C0C0C0'>"&cant&" €</td>")
       Response.Write("<td align='center' bgcolor='#C0C0C0'>"&com&" %</td>")
       Response.Write("</tr>")
       Rs.MoveNext
     Loop
  end if
     Response.Write("</tbody></table>")
     Response.Write("<a href='NuevoMov.asp?ccc="&cuenta&"&tipoCuen="&tipo&"&id=conf'>[Realizar Movimiento]</a>")
     
  ' Finalmente cerramos el RecordSet y lo destruimos
   Rs.Close
   Set Rs = Nothing

  ' Cerramos tambien la conexion
  Conn.Close
  Set Conn = Nothing

%>

</body>
</html>

