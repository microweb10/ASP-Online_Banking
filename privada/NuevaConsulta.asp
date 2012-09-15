<!-- #include file="Menu.html" -->
<title>PIDIRECT - Consultas</title>
<%
'Comprobamos que hayan iniciado sesion
if session("dni")=Empty or session("nombre")=Empty then
	response.Redirect("../abierta/Login.php?id=cli&msg=noid")
else
	dni=session("dni")
	nombre=session("nombre")
	'recibo tambien los parametros
	asunto=request.queryString("asunto")
	preg=request.queryString("pregunta")

	'Realizamos la insercion el la Base de Datos
	Set Conn = Server.CreateObject("ADODB.Connection")
	Conn.Open "pidirect"

	' Preparo la sentencia
	sSQL= " insert into consultas  (dniCli,asunto,txtpregunta) values"
	sSQL= sSQL & " ('" & dni & "','" & asunto & "','" & preg & "')"

	on error resume next ' SI OCURRE ALGUN ERROR CONTINUA
	Conn.execute(sSQL)
	if Err.number <> 0  then
		response.Redirect("ConsultasCli.asp?id=nook&error="& Err.description &"")
	else
		response.Redirect("ConsultasCli.asp?id=ok")
	end if

	Err.clear
	On error Goto 0  'QUE NO SE OLVIDE ESO PARA QUE VUELVA OTRA VEZ
	
end if
%>

</body>
</html>

