<!-- #include file="Menu.html" -->
<title>PIDIRECT - Realizar Movimiento de Cuenta</title>
<%
'Comprobamos que hayan iniciado sesion
if session("dni")=Empty or session("nombre")=Empty then
	response.Redirect("../abierta/Login.php?id=cli&msg=noid")
else
	dni=session("dni")
	nombre=session("nombre")
	'recibo tambien los parametros
	cuenta=request.queryString("ccc")
	id=request.queryString("id")
	tipoCuen=request.queryString("tipoCuen")
end if

' Creamos conexion y dos recorset

Set Conn = Server.CreateObject("ADODB.Connection")
Set Rs   = Server.CreateObject("ADODB.RecordSet")

' Abrimos conexion
Conn.Open "pidirect"


' Vemos si ya esta configurado el movimiento o hay que configurarlo
if (id="ok") then
  ' Tenemos que insertar el movimiento en la base de datos
  ' recibo los demas parametros
  conc=request.queryString("concepto")
  tipoMov=request.queryString("tipoMov")
  cant=request.queryString("cantidad")
  cccDest=request.queryString("cccDest")

  if(tipoMov="1") then  'Si el movimiento es una transferencia
    ' Preparo las sentencias
    sSQL= "insert into movimientos  (cuenta,concepto,tipo,cantidad) values"
    sSQL= sSQL & " ("&cuenta&",'"&conc&"',"&tipoMov&",-"&cant&")"
    sSQL2="insert into movimientos (cuenta,concepto,tipo,cantidad) values"
    sSQL2=sSQL2&" ("&cccDest&",'Recibida de "&cuenta&"',2,"&cant&")"
  else
    ' Hay que mirar a ver si el movimiento es positivo o negativo
    sSQLaux="select * from tiposMovimientos where id="&tipoMov&""
    Set Rs = Conn.Execute(sSQLaux)

    Do While NOT Rs.EOF
      neg=Rs.Fields("ApunteNegativo").value
      if (neg="S") then
        ' Preparo la sentencia
        sSQL= "insert into movimientos  (cuenta,concepto,tipo,cantidad) values"
        sSQL= sSQL & " ("&cuenta&",'"&conc&"',"&tipoMov&",-"&cant&")"
      else
        ' Preparo la sentencia
        sSQL= "insert into movimientos  (cuenta,concepto,tipo,cantidad) values"
        sSQL= sSQL & " ("&cuenta&",'"&conc&"',"&tipoMov&","&cant&")"
      end if
      Rs.MoveNext
    Loop
    ' Cerramos el RecordSet (no lo destruimos, lo haremos despues)
    Rs.Close
  end if

  if(tipoMov="1") then
    Conn.execute(sSQL)
    Conn.execute(sSQL2)
  else
    Conn.execute(sSQL)
  end if
  on error resume next ' SI OCURRE ALGUN ERROR CONTINUA
  if Err.number <> 0  then
    response.Write("Ha ocurrido el siguiente ERROR: "&Err.description&"")
  else
    response.Redirect("Movimientos.asp?ccc="&cuenta&"&tipo="&tipoCuen&"&ver=TODOS&ord=fAsc")
  end if

  Err.clear
  On error Goto 0  'QUE NO SE OLVIDE ESO PARA QUE VUELVA OTRA VEZ
  
else
%>
<br><center><font size="6" color="#D60000">Cliente <%=nombre%></font> <a href="Logout.asp">Salir</a><center>
<table border="0" width="50%" align="center">
	<tr>
		<td align="center" colspan="2"><img src="../imagenes/nuevoMov.jpg"></td>
	</tr>
	<tr>
		<td align="center" colspan="2" bgcolor="#000080"><font color="#FFFFFF" size="5"><b><%=tipoCuen%></b> CCC <b><%=cuenta%></b></font></td>
	</tr>
	<tr>
	        <td align="center"><form action="NuevoMov.asp" method="GET" name="FormNuevoMov" onsubmit="return EnviaFormNuevoMov(<%=cuenta%>);">
                	<input type="hidden" name="id" value="ok">
			<input type="hidden" name="ccc" value="<%=cuenta%>">
			<input type="hidden" name="tipoCuen" value="<%=tipoCuen%>">
			<b>TIPO DE MOVIMIENTO</b></td>
		<td><select name="tipoMov" size="1">
<%
  ' necesitamos mostrar en el select los tipos de movimientos para la cuenta
  
  ' preparamos la consulta
  sSQL = "select * from MovPermitCuenta where cuenta="&cuenta&""

  ' La ejecutamos en el recorset creado
  Set Rs = Conn.Execute(sSQL)
  
  Do While NOT Rs.EOF
    mov=Rs.Fields("movimiento").value
    id=Rs.Fields("id").value
    Response.Write("<option value="&id&">"&mov&"</option>")
    Rs.MoveNext
  Loop
  
  ' Cerramos el RecordSet (no lo destruimos, lo utilizaremos despues)
   Rs.Close
%>
                </select></td>
	</tr>
	<tr>
		<td align="center"><b>CUENTA DESTINO (*)</b></td>
		<td><select name="cccDest" size="1">
	
<%
' necesitamos mostrar en el select las cuentas del cliente
' no mostraremos la cuenta con la que quiere hacer el movimiento

  ' preparamos la consulta
  sSQL = "select * from CuentasCli where dni='"&dni&"'"

  ' La ejecutamos en el recorset creado
  Set Rs = Conn.Execute(sSQL)

  Response.Write("<option value='0'> </option>")
  Do While NOT Rs.EOF
    rs_cuen=Rs.Fields("numero").value
    rs_tipo=Rs.Fields("nombreTipoCuenta").value
    Response.Write("<option value="&rs_cuen&">"&rs_tipo&" Nº "&rs_cuen&"</option>")
    Rs.MoveNext
  Loop

%>
		</select></td>
	</tr>
	<tr>
		<td align="center"><b>CONCEPTO</b></td>
		<td><input type="text" name="concepto" size="35"></td>
	</tr>
	<tr>
		<td align="center"><b>CANTIDAD EN EUROS</b></td>
		<td><input type="text" name="cantidad" size="10"></td>
	</tr>
	<tr><td align="center" colspan="2"><button type="submit" style="font-size:18">Realizar Movimiento</button></tr>
</form></table>
<br><center>(*) Ésta Casilla Sólo Tiene Valor Si El Movimiento Es TRANSFERENCIA ENVIADA</center>

<%
   ' Cerramos el RecordSet (no lo destruimos, lo haremos despues)
   Rs.Close
end if

  ' Finalmente destruimos el RecordSet
   Set Rs = Nothing

  ' Cerramos tambien la conexion
  Conn.Close
  Set Conn = Nothing
%>

</body>
</html>


