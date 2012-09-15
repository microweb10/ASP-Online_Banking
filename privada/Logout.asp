<!doctype html public "-//w3c//dtd html 4.0 transitional//en">
<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
   <meta name="Author" content="Julian Nicolas">
   <meta name="Copyright" content="© Julian Nicolas">
   <meta http-equiv="Content-Script-Type" content="JavaScript">
   <title>Pagina de Logout</title>
</head>
<body>

<%
'Quitamos el usuario de la sesion
    session("dni")=Empty
    session("nombre")=Empty
'Redireccionamos a la pagina Principal
    response.Redirect("../abierta/Principal.php")
%>

</body>
</html>

