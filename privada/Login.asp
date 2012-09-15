<!doctype html public "-//w3c//dtd html 4.0 transitional//en">
<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
   <meta name="Author" content="Julian Nicolas">
   <meta name="Copyright" content="© Julian Nicolas">
   <meta http-equiv="Content-Script-Type" content="JavaScript">
   <title>Pagina de Login</title>
</head>
<body>
<%
'Recibimos la informacion del formulario de autentificación
sID=request.queryString("id")
sDni=request.form("dni")
sClave=request.form("password")

'COMPROBAMOS SI SE TRATA DEL USUARIO ADMINISTRADOR
if (sDni="PidirectAdm" and sClave="1234") then
 	session("id_usuario")="admin" 'Guardamos los datos del usuario en variables de sesión
	session("nombre_usuario")="Administrador"
 	response.Redirect("PrincipalAdm.asp") 'Redireccionamos a la página privada del administrador
else
   'COMPROBAMOS SI SE TRATA DE UN CLIENTE
   if (sID="cli") then
	'Sustituimos los caracteres peligrosos (comillas) que podrían provocar agujeros de seguridad en las
	'sentencias SQL
	sDni=Replace(sDni,"'","`")
	sDni=Replace(sDni,"""","`")
 	sClave=Replace(sClave,"'","`")
	sClave=Replace(sClave,"""","`")
	'Creamos la conexion con la BD
	set conexion = Server.CreateObject("ADODB.Connection")
	'Abrimos la conexión
	conexion.Open "pidirect"
	'Definimos la sentencia para buscar el nombre de usuario y la contraseña indicados
	sSQL = "select * from clientes where "
	sSQL = sSQL & " dni = '" & sDni & "' and contrasenya = '" & sClave & "' "
	'Enviamos la sentencia a la base de datos
	set rsUsuarios = conexion.execute(sSQL)
	if not rsUsuarios.eof then
	    session("dni")=sDni 'Guardamos los datos del usuario en variables de sesión
	    session("nombre")=rsUsuarios("nombre")
	    response.Redirect("PrincipalCli.asp") 'Redireccionamos a la página incial del cliente
	else
            response.Redirect("../abierta/Login.php?id=cli&msg=nocli") 'Redireccionamos a la pagina de login
	end if
        'Cerramos el RecordSet y la conexión con la BD y liberamos recursos
        rsUsuarios.Close()
        set rsUsuarios = Nothing
	conexion.Close()
	set conexion = Nothing
   end if
   'COMPROBAMOS SI SE TRATA DE UN EMPLEADO
   if (sID="emp") then
	'Sustituimos los caracteres peligrosos (comillas) que podrían provocar agujeros de seguridad en las
	'sentencias SQL
	sDni=Replace(sDni,"'","`")
	sDni=Replace(sDni,"""","`")
 	sClave=Replace(sClave,"'","`")
	sClave=Replace(sClave,"""","`")
	'Creamos la conexion con la BD
	set conexion = Server.CreateObject("ADODB.Connection")
	'Abrimos la conexión
	conexion.Open "pidirect"
	'Definimos la sentencia para buscar el nombre de usuario y la contraseña indicados
	sSQL = "select * from empleados where "
	sSQL = sSQL & " dni = '" & sDni & "' and contrasenya = '" & sClave & "' "
	'Enviamos la sentencia a la base de datos
	set rsUsuarios = conexion.execute(sSQL)
	if not rsUsuarios.eof then
	    session("dni")=sDni 'Guardamos los datos del usuario en variables de sesión
	    session("nombre")=rsUsuarios("nombre")
	    response.Redirect("PrincipalEmp.asp") 'Redireccionamos a la página incial del empleado
	else
            response.Redirect("../abierta/Login.php?id=emp&msg=noemp") 'Redireccionamos a la pagina de login
	end if
        'Cerramos el RecordSet y la conexión con la BD y liberamos recursos
        rsUsuarios.Close()
        set rsUsuarios = Nothing
	conexion.Close()
	set conexion = Nothing
   end if
end if
%>

</body>
</html>

