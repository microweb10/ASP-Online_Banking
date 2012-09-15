//devuelve false si hay algun campo del formulario incorrecto
function EnviaFormSolicitud(){

	//comprobamos que introduce un nombre
	if(document.FormSolicitud.nombre.value==""){
		alert("Introduce tu nombre");
		return false;
	}
	if(document.FormSolicitud.nombre.value.length>100){
		alert("El Nombre no puede tener mas de 100 carácteres.");
		return false;
	}
	//comprobamos que introduce una direccion
	if(document.FormSolicitud.direccion.value==""){
		alert("Introduce tu direccion");
		return false;
	}
	if(document.FormSolicitud.direccion.value.length>100){
		alert("La Dirección no puede tener mas de 100 carácteres.");
		return false;
	}
	//comprobamos que introduce una poblacion
	if(document.FormSolicitud.poblacion.value==""){
		alert("Introduce tu poblacion");
		return false;
	}
	if(document.FormSolicitud.poblacion.value.length>100){
		alert("La Población no puede tener mas de 100 carácteres.");
		return false;
	}
	//comprobamos que introduce una provincia
	if(document.FormSolicitud.provincia.value==""){
		alert("Introduce tu provincia");
		return false;
	}
	if(document.FormSolicitud.provincia.value.length>50){
		alert("La Provincia no puede tener mas de 50 carácteres.");
		return false;
	}
	//Expresion Regular que debe cumplir el dni
	var bDni=new RegExp("^[0-9]{8}[A-Z]$");

	//Expresion Regular que debe cumplir el codigo postal
	var bCod=new RegExp("^[0-9]{5}$");

	//Expresion Regular que debe cumplir el email
	var bEmail=new RegExp("^[A-Za-z0-9]+\@[a-z\.]+[a-z]{2,3}$");

	//Comprobamos que introduce un dni correcto
	if(document.FormSolicitud.dni.value==""){
		alert("Debes introducir tu DNI");
		return false;
	}
	if(bDni.test(document.FormSolicitud.dni.value)==false){
		alert("El formato del DNI es incorrecto, ej: 12345678A");
		return false;
	}
	//comprobamos que introduce un email correcto
	if(document.FormSolicitud.email.value==""){
		alert("Introduce tu correo electrónico (email)");
		return false;
	}
	if(bEmail.test(document.FormSolicitud.email.value)==false){
		alert("La direccion de email es incorrecta");
		return false;
	}
	if(document.FormSolicitud.email.value.length>50){
		alert("El Email no puede tener mas de 50 carácteres.");
		return false;
	}
	//comprobamos que el codigo postal es correcto
	if(document.FormSolicitud.codpostal.value==""){
		alert("Introduce el codigo postal");
		return false;
	}
	if(bCod.test(document.FormSolicitud.codpostal.value)==false){
	 	alert("El formato del codigo postal es incorrecto, ej: 12345");
		return false;
	}
return true;
}

//devuelve false si todos los campos del formulario estan vacios
function EnviaFormBuscar(){

        if((document.FormBuscar.asunto.value=="")&&(document.FormBuscar.empleado.value=="")
	&&(document.FormBuscar.pregunta.value=="")&&(document.FormBuscar.respuesta.value=="")){
                alert("Debes introducir al menos alguna palabra clave para buscar");
		return false;
	}
	if(document.FormBuscar.asunto.value.length>50){
		alert("El Asunto no puede tener mas de 50 carácteres.");
		return false;
	}
	if(document.FormBuscar.empleado.value.length>100){
		alert("El Nombre no puede tener mas de 50 carácteres.");
		return false;
	}
	if(document.FormBuscar.pregunta.value.length>250){
		alert("La Pregunta no puede tener mas de 250 carácteres.");
		return false;
	}
	if(document.FormBuscar.respuesta.value.length>250){
		alert("La Respuesta no puede tener mas de 250 carácteres.");
		return false;
	}
return true;
}

//devuelve false si el campo de la cantidad no es correcto
function EnviaFormComision(){

	//Expresion Regular que debe cumplir la cantidad
	var bCant=new RegExp("^[0-9]{1,9}$");

        if(document.FormComision.cantidad.value==""){
                alert("No has introducido ninguna cantidad para calcular la comision");
		return false;
	}
	if(bCant.test(document.FormComision.cantidad.value)==false){
		alert("Debe escribir la cantidad sin puntos ni comas,\n y con un maximo de 9 digitos ej: 100000");
		return false;
	}
return true;
}

//devuelve false si hay algun campo del formulario incorrecto
function EnviaFormLogin(){

	//comprobamos que introduce un dni
	if(document.FormLogin.dni.value==""){
		alert("No has introducido tu DNI");
		return false;
	}
	//comprobamos que introduce un password
	if(document.FormLogin.password.value==""){
		alert("No has introducido tu password");
		return false;
	}
	if(document.FormLogin.password.value.length>50){
		alert("La Pregunta no puede tener mas de 50 carácteres.");
		return false;
	}

	//Expresion Regular que debe cumplir el dni
	var bDni=new RegExp("^[0-9]{8}$");

	//Comprobamos que introduce un dni correcto
	if((bDni.test(document.FormLogin.dni.value)==false)&&(document.FormLogin.dni.value!="PidirectAdm")){
		alert("Debe escribir sólo el numero de su DNI, ej: 12345678");
		return false;
	}
return true;
}

//Función que se encarga de comprobar que la fecha seleccionada es correcta
function fComprobar(vardia,varmes,varanyo){

 var todoOK= true;
 //Comprobamos que la fecha es correcta. Para ello, creamos un objeto Date con
 //los datos indicados en el formulario
 var dia= parseInt(vardia);
 var mes= parseInt(varmes);
 var anyo= parseInt(varanyo);

 if(mes>12){
   todoOK = false;
 }
 else{
   if(mes==2){
      if(anyo%4==0){ //febrero si es bisiesto no puede tener mas de 29 dias
	 if(dia>29) todoOK= false;
      }
      else{//febrero si no es bisiesto no puede tener mas de 28 dias
         if(dia>28) todoOK= false;
      }
   }else{
      if((mes==4)||(mes==6)||(mes==9)||(mes==11)){ //esos meses no puede tener mas de 30 dias
         if(dia>30) todoOK= false;
      }
      else{ //el resto no puede tener mas de 31 dias
         if(dia>31) todoOK= false;
      }
   }
 }
return todoOK;
}

//devuelve false si hay alguna fecha del formulario incorrecta
function EnviaFormComisiones(){

	//comprobamos que introduce las dos fechas
	if((document.FormComisiones.fInicio.value=="")||(document.FormComisiones.fFinal.value=="")){
		alert("Debes introducir fecha inicial y final para poder buscar");
		return false;
	}
	
	//Expresion Regular que deben cumplir las fechas
	var bFecha=new RegExp("^[0-9]{2}/[0-9]{2}/[0-9]{4}$");

	//Comprobamos el formato de fechas
	if((bFecha.test(document.FormComisiones.fInicio.value)==false)||(bFecha.test(document.FormComisiones.fFinal.value)==false)){
		alert("Alguna fecha tiene un formato incorrecto, ej: 01/01/2001");
		return false;
	}
	
	//Comprobamos que las fechas son correctas
	var fecha1 = document.FormComisiones.fInicio.value;
	var fecha2 = document.FormComisiones.fFinal.value;
	var dia1 = fecha1.substring(0,2);
	var mes1 = fecha1.substring(3,5);
	var anyo1 = fecha1.substring(6,10);
	var dia2 = fecha2.substring(0,2);
	var mes2 = fecha2.substring(3,5);
	var anyo2 = fecha2.substring(6,10);
	
	if((fComprobar(dia1,mes1,anyo1)==false)||(fComprobar(dia2,mes2,anyo2)==false)){
		alert("Alguna Fecha no es correcta. Por Ejemplo el 30/02/2006 no es correcta");
		return false;
	}
	else{
		document.FormComisiones.fInicio.value=mes1+"/"+dia1+"/"+anyo1;
		document.FormComisiones.fFinal.value=mes2+"/"+dia2+"/"+anyo2;
	}
return true;
}

//devuelve false si hay algun campo del formulario incorrecto
function EnviaFormActDatosCli(){

        //Expresion Regular que debe cumplir la direccion
	var bDir=new RegExp("^[0-9A-Za-záéíóúÁÉÍÓÚñÑçÇ/, ]{2,100}$");
	//Expresion Regular que debe cumplir la poblacion
	var bPob=new RegExp("^[A-Za-záéíóúÁÉÍÓÚñÑçÇ ]{3,100}$");
	//Expresion Regular que debe cumplir la provincia
	var bPro=new RegExp("^[A-Za-záéíóúÁÉÍÓÚñÑçÇ ]{3,50}$");
	//Expresion Regular que debe cumplir el codigo postal
	var bCP=new RegExp("^[0-9]{5}$");
	//Expresion Regular que debe cumplir la contrasenya
	var bPW=new RegExp("^[0-9a-zA-Z]{2,50}$");

        //Comprobamos el formato de la direccion
	if((document.FormActDatosCli.direccion.value=="")||(bDir.test(document.FormActDatosCli.direccion.value)==false)){
		alert("La Direccion introducida no puede estar vacia ni tener caracteres especiales (max 100 carácteres)");
		return false;
	}
	//Comprobamos el formato de la poblacion
	if((document.FormActDatosCli.poblacion.value=="")||(bPob.test(document.FormActDatosCli.poblacion.value)==false)){
		alert("La Poblacion introducida no puede estar vacia ni tener caracteres especiales (max 100 carácteres)");
		return false;
	}
        //Comprobamos el formato del codigo postal
	if((document.FormActDatosCli.codigopostal.value=="")||(bCP.test(document.FormActDatosCli.codigopostal.value)==false)){
		alert("El Codigo Postal introducido es erroneo. Ej: 12345");
		return false;
	}
	//Comprobamos el formato de la provincia
	if((document.FormActDatosCli.provincia.value=="")||(bPro.test(document.FormActDatosCli.provincia.value)==false)){
		alert("La Provincia introducida no puede estar vacia ni tener caracteres especiales (max 50 carácteres)");
		return false;
	}
	//Comprobamos el formato de la contrasenya
	if((document.FormActDatosCli.contrasenya.value=="")||(bPW.test(document.FormActDatosCli.contrasenya.value)==false)){
		alert("La Contrasenya debe tener de 2 a 50 caracteres\n y no puede tener caracteres especiales");
		return false;
	}
return true;
}

//devuelve false si no hay asunto o pregunta
function EnviaFormNuevaConsulta(){

	//comprobamos que introduce el asunto y la pregunta
	if((document.FormNuevaConsulta.asunto.value=="")||(document.FormNuevaConsulta.pregunta.value=="")){
		alert("Debes introducir un Asunto y una Pregunta");
		return false;
	}
	if(document.FormNuevaConsulta.asunto.value.length>50){
		alert("El Asunto no puede tener mas de 50 carácteres.");
		return false;
	}
	if(document.FormNuevaConsulta.pregunta.value.length>250){
		alert("La Pregunta no puede tener mas de 250 carácteres.");
		return false;
	}
return true;
}

//devuelve false si hay algun campo del formulario incorrecto
function EnviaFormNuevoMov(ccc){

	//comprobamos que ha introducido una cuenta destino si se quiere hacer una transferencia
	if((document.FormNuevoMov.tipoMov.value=="1")&&(document.FormNuevoMov.cccDest.value=="0")){
                alert("Debes introducir el numero de cuenta a donde ira la Transferencia");
		return false;
	}
	//Comprobamos que esa cuenta destino no sea la misma que de donde esta haciendo la tranferencia
	if((document.FormNuevoMov.tipoMov.value=="1")&&(document.FormNuevoMov.cccDest.value==ccc)){
		alert("No tiene sentido hacer una transferencia a la misma cuenta");
		return false;
	}
        //Expresion Regular que debe cumplir la cantidad
	var bCant=new RegExp("^[0-9]{1,6}$");
	var bCantDec=new RegExp("^[0-9]{1,6}.[0-9]{1,2}$");

	//comprobamos que introduce el concepto
	if(document.FormNuevoMov.concepto.value==""){
		alert("Debes introducir un Concepto");
		return false;
	}
	if(document.FormNuevoMov.concepto.value.length>200){
		alert("El Concepto no puede tener mas de 200 carácteres.");
		return false;
	}
	//comprobamos que introduce una cantidad correcta
        if(document.FormNuevoMov.cantidad.value.indexOf(',')!=-1){
		alert("No escribas el signo coma, la separacion debe escribirse con un punto.\nEj: 12000  Ej: 1520.54");
		return false;
	}
	if(parseInt(document.FormNuevoMov.cantidad.value)==0){
		alert("No tiene sentido que la cantidad sea cero");
		return false;
	}
	if((document.FormNuevoMov.cantidad.value=="")||((bCant.test(document.FormNuevoMov.cantidad.value)==false)&&(bCantDec.test(document.FormNuevoMov.cantidad.value)==false))){
		alert("Debes introducir una Cantidad Positiva Correcta.\nEj: 12000  Ej: 1520.54");
		return false;
	}
return true;
}

//devuelve false si no hay respuesta
function EnviaFormRespCons(){

	//comprobamos que introduce la respuesta
	if(document.FormRespCons.respuesta.value==""){
		alert("No has introducido la Respuesta.");
		return false;
	}
	if(document.FormRespCons.respuesta.value.length>250){
		alert("La Respuesta no puede tener mas de 250 carácteres.");
		return false;
	}
return true;
}

//devuelve false si no hay respuesta
function EnviaFormModCons(){

	//comprobamos que introduce la respuesta
	if(document.FormModCons.respuesta.value==""){
		alert("No has introducido la Respuesta.");
		return false;
	}
	if(document.FormModCons.respuesta.value.length>250){
		alert("La Respuesta no puede tener mas de 50 carácteres.");
		return false;
	}
return true;
}


