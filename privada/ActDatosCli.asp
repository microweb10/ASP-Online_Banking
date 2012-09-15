<!-- #include file="Menu.html" -->
<title>PIDIRECT - Cambio de Datos Personales</title>
<%
'Comprobamos que hayan iniciado sesion
if session("dni")=Empty or session("nombre")=Empty then
	response.Redirect("../abierta/Login.php?id=cli&msg=noid")
else
	dni=session("dni")
	nombre=session("nombre")
	'recibo tambien los parametros
	dir=request.queryString("direccion")
	pob=request.queryString("poblacion")
	cp=request.queryString("codigopostal")
	prov=request.queryString("provincia")
	pw=request.queryString("contrasenya")
end if
%>
<br><center><font size="6" color="#D60000">Cliente <%=nombre%></font> <a href="Logout.asp">Salir</a><center>
<%
'Hay que modificar los datos del cliente

 ' Creamos conexion y dos recorset

  Set Conn = Server.CreateObject("ADODB.Connection")

 ' Abrimos conexion
 Conn.Open "pidirect"

  ' preparamos la consulta
  sSQL = "update clientes set direccion='"&dir&"',poblacion='"&pob&"',codigopostal='"&cp&"',provincia='"&prov&"',contrasenya='"&pw&"' where dni='"&dni&"'"

  'Comprobamos que haya algun resultado
  Conn.execute(sSQL)
	if Err.number <> 0  then
		Response.Write("<br><center>Ha ocurrido algun error en el servidor</center>")
		Response.Write("<br><center><a href='DatosCli.asp'>VOLVER A INTENTAR</a></center>")
	else
		Response.Write("<br><center>Los Datos se han enviado Correctamente</center>")
		Response.Write("<br><center><a href='DatosCli.asp'>VER NUEVOS DATOS</a></center>")
	end if

	Err.clear
	On error Goto 0  'QUE NO SE OLVIDE ESO PARA QUE VUELVA OTRA VEZ
	

  ' Cerramos la conexion
  Conn.Close
  Set Conn = Nothing

%>

</body>
</html>

