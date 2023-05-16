#include <iostream> // Librer�a est�ndar de entrada y salida.
#include <mysql.h> // Librer�a MySQl para base de datos.
#include <locale> // Librer�a para uso de car�cteres especiales de espa�ol.
#include <string> // Librer�a para uso de variables tipo string.
#include <vector> // Librer�a para uso de arreglos din�micos.
#include <sstream> // Librer�a para conversi�n de cadenas a enteros.
#include <ctime> // Librer�a para uso de tiempo y fecha del sistema.

using namespace std; // Para uso de cin y cout.

// Declaraci�n de estructura datos.

struct datos{
	string nombre, apellido, DPI;
};

// Prototipos de funciones y m�todos.

MYSQL* conectar_bd();
void ingresar_datos();
void menu(string);
void mostrar_productos(int, int, string);
int numero_aleatorio();
void info_pedido(int);

int main(){
	
	// Leer/Escribir car�cteres especiales de espa�ol.
	
	setlocale(LC_ALL, "spanish");
	
	// Llamando al m�todo ingresar_datos.
	
	ingresar_datos();
	
	return 0;
}

// Definici�n de la funci�n conectar_bd.

MYSQL* conectar_bd(){
	
	// Estableciendo conexi�n con la base de datos.
	
    MYSQL* conectar = mysql_init(0);
    conectar = mysql_real_connect(conectar, "127.0.0.1", "Admin", "r62af79a", "toma_pedidos", 3306, NULL, 0);
    return conectar; 
}

// Definici�n del m�todo ingresar_datos.

void ingresar_datos(){
		
	// Declaraci�n de variables.
	
	datos vardatos;
	
	// Solicitando datos del usuario.
	
	cout<<"\t\t============"<<endl;
	cout<<"\t\tTOMA PEDIDOS"<<endl;
	cout<<"\t\t============\n"<<endl;
	cout<<"\tIngrese sus datos personales\n"<<endl;
	cout<<"\tDPI: "; getline(cin, vardatos.DPI);
	cout<<"\tNombre: ";getline(cin, vardatos.nombre);
	cout<<"\tApellido: ";getline(cin, vardatos.apellido);
	cout<<"\n\n\n\n\t=== Datos Ingresados Correctamente ===\n\n"<<endl;
	system("pause");
	
	// Llamando a la funcion conectar_bd para establecer conexion con la base de datos.
    
	MYSQL* conexion = conectar_bd();
	
	// Preparando la consulta SQL e insertar los datos ingresados por el usuario en la tabla clientes.
	
    string consulta = "INSERT INTO cliente (idcliente, nombre, apellido) VALUES ('" + vardatos.DPI + "', '" + vardatos.nombre + "', '" + vardatos.apellido + "')";
    mysql_query(conexion, consulta.c_str());
    
    // Cerrar la conexi�n a la base de datos
    
    mysql_close(conexion);
    
    // Llamando al m�todo menu.
    
    menu(vardatos.DPI);
}

// Definici�n del m�todo menu.
 
void menu(string DPI){
	
	// Limpiando pantalla.
	
	system("cls");
	
	// Declaraci�n de varaibles.
	
	int op, numero_pedido = numero_aleatorio();
	
	// Mostrando franjas disponibles y solicitando el ingreso de una opci�n.
	
	cout<<"\t\t============"<<endl;
	cout<<"\t\tTOMA PEDIDOS"<<endl;
	cout<<"\t\t============"<<endl;
	cout<<"\t\t  FRANJAS"<<endl;
	cout<<"\t\t============\n"<<endl;
	cout<<"\t1. Franja de desayuno"<<endl;
	cout<<"\t2. Franja de almuerzo"<<endl;
	cout<<"\t3. Franja de cena\n"<<endl;
	cout<<"\t";cin>>op;
	
	// Llamando a la funcion conectar_bd para establecer conexion con la base de datos.
    
	MYSQL* conexion = conectar_bd();
	
	// Preparando la consulta SQL e insertar los datos en la tabla.
	
    string consulta = "INSERT INTO pedido (idpedido, franja_idfranja, cliente_idcliente) VALUES ('" + to_string(numero_pedido) + "', '" + to_string(op) + "', '" + DPI + "')";
    mysql_query(conexion, consulta.c_str());
    
    // Cerrar la conexi�n a la base de datos
    
    mysql_close(conexion);
	
	// Mostrando franjas segun la opci�n ingresada, llamando al m�todo mostrar_productos.
	
	mostrar_productos(op, numero_pedido, DPI);
}

// Definici�n del m�todo mostrar_productos.

void mostrar_productos(int idFranjas, int idPedido, string DPI) {
    
    // Declaraci�n de variables.
    
    int contador = 1, id;

	do{
		
		// Limpiando pantalla.
    
    	system("cls");
		
		// Llamando a la funci�n conectar_bd para establecer conexi�n con la base de datos.
    
    	MYSQL* conexion = conectar_bd();
    
    	// Preparar la consulta SQL.
    
   		string consulta = "SELECT idproducto, nombre, descripcion, precio FROM producto WHERE franja_idfranja = " + to_string(idFranjas);

    	// Ejecutar la consulta SQL.
    
    	mysql_query(conexion, consulta.c_str());

    	// Obtener el resultado de la consulta.
    
    	MYSQL_RES* resultado = mysql_store_result(conexion);
    	
	    // Mostrando datos de los productos.
	    
	    cout<<"\t\t====================="<<endl;    
	    cout<<"\t\tPRODUCTOS DISPONIBLES"<<endl;
	    cout<<"\t\t=====================\n"<<endl;  
	    
	    MYSQL_ROW fila;
	        
	    while ((fila = mysql_fetch_row(resultado)) != NULL) {
	        
	        cout<<"\t"<<contador<<". Nombre: "<<fila[1]<<endl;
	        cout<<"\t   Descripci�n: "<<fila[2]<<endl;
	        cout<<"\t   Precio: " <<fila[3]<<endl;
	        cout<<"\t   ID: " <<fila[0]<<"\n"<<endl;
	        contador = ++contador;
	    }
	    
	    // Solicitando al usuario que ingrese los productos que desea comprar.
	    
	    cout<<"\n\tEliga el ID producto, ingrese 0 para terminar\n"<<endl;
	    cout<<"\t";cin>>id;
	    
	    if(id != 0){
	    	
	    	// Preparando la consulta SQL e insertar los datos en la tabla detalle_pedido.
	
    		string consulta = "INSERT INTO detalle_pedido (producto_idproducto, pedido_idpedido) VALUES ('" + to_string(id) + "', '" + to_string(idPedido) + "')";
			mysql_query(conexion, consulta.c_str());
				
			// Cerrar la conexi�n a la base de datos
    
    		mysql_close(conexion);
	    	
			cout<<"\n\t\t=== Su producto ha sido agregado ===\n"<<endl;
		}
		else
			cout<<"\n\t\t=== Productos agregados con �xito ===\n"<<endl;	
			
	    system("pause");
	    contador = 1;
	    
    }while(id != 0);
    
    info_pedido(idPedido);
    
}

// Definici�n de la funci�n numero_aleatorio.

int numero_aleatorio(){
	
	// Obtener el valor actual del reloj del sistema como semilla.
	
    srand(time(0));
    
    // Retornando un n�mero aleatorio de 5 cifras.
    
    int num;
    
    return num = rand() % 90000 + 10000;
}

void info_pedido(int idPedido){
		system("cls");
		cout<<"\t"<<idPedido<<endl;
}
