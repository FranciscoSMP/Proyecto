#include <iostream> // Librería estándar de entrada y salida.
#include <mysql.h> // Librería MySQl para base de datos.
#include <locale> // Librería para uso de carácteres especiales de español.
#include <string> // Librería para uso de variables tipo string.
#include <vector> // Librería para uso de arreglos dinámicos.
#include <sstream> // Librería para conversión de cadenas a enteros.
#include <ctime> // Librería para uso de tiempo y fecha del sistema.

using namespace std; // Para uso de cin y cout.

// Declaración de estructura datos.

struct datos{
	string nombre, apellido, DPI;
};

// Prototipos de funciones y métodos.

MYSQL* conectar_bd();
int numero_aleatorio();
void ingresar_datos();
void menu(string);
void mostrar_productos(int, int);
void info_pedido(int);

int main(){
	
	// Leer/Escribir carácteres especiales de español.
	
	setlocale(LC_ALL, "spanish");
	
	// Llamando al método ingresar_datos.
	
	ingresar_datos();
	
	return 0;
}

// Definición de la función conectar_bd.

MYSQL* conectar_bd(){
	
	// Estableciendo conexión con la base de datos.
	
    MYSQL* conectar = mysql_init(0);
    conectar = mysql_real_connect(conectar, "127.0.0.1", "Admin", "r62af79a", "toma_pedidos", 3306, NULL, 0);
    return conectar; 
}

// Definición de la función numero_aleatorio.

int numero_aleatorio(){
	
	// Obtener el valor actual del reloj del sistema como semilla.
	
    srand(time(0));
    
    // Retornando un número aleatorio de 5 cifras.
    
    int num;
    
    return num = rand() % 90000 + 10000;
}

// Definición del método ingresar_datos.

void ingresar_datos(){
		
	// Declaración de variables.
	
	datos vardatos;
	
	// Solicitando datos del usuario.
	
	cout<<"\n\t\t\t============"<<endl;
	cout<<"\t\t\tTOMA PEDIDOS"<<endl;
	cout<<"\t\t\t============\n"<<endl;
	cout<<"\t\tIngrese sus datos personales\n"<<endl;
	cout<<"\tNo. de DPI (sin espacios): "; getline(cin, vardatos.DPI);
	cout<<"\tNombres: ";getline(cin, vardatos.nombre);
	cout<<"\tApellidos: ";getline(cin, vardatos.apellido);
	cout<<"\n\n\n\n\t=== Datos Ingresados Correctamente ===\n\n"<<endl;
	system("pause");
	
	// Llamando a la funcion conectar_bd para establecer conexion con la base de datos.
    
	MYSQL* conexion = conectar_bd();
	
	// Preparando la consulta SQL e insertar los datos ingresados por el usuario en la tabla clientes.
	
    string consulta = "INSERT INTO cliente (idcliente, nombre, apellido) VALUES ('" + vardatos.DPI + "', '" + vardatos.nombre + "', '" + vardatos.apellido + "')";
    mysql_query(conexion, consulta.c_str());
    
    // Cerrar la conexión a la base de datos
    
    mysql_close(conexion);
    
    // Llamando al método menu.
    
    menu(vardatos.DPI);
}

// Definición del método menu.
 
void menu(string DPI){
	
	// Limpiando pantalla.
	
	system("cls");
	
	// Declaración de varaibles.
	
	int op, numero_pedido = numero_aleatorio();
	
	// Mostrando franjas disponibles y solicitando el ingreso de una opción.
	
	cout<<"\n\t\t============"<<endl;
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
    
    // Cerrar la conexión a la base de datos
    
    mysql_close(conexion);
	
	// Mostrando franjas segun la opción ingresada, llamando al método mostrar_productos.
	
	mostrar_productos(op, numero_pedido);
}

// Definición del método mostrar_productos.

void mostrar_productos(int idFranjas, int idPedido) {
    
    // Declaración de variables.
    
    int contador = 1, id, lpedido = 0;

	do{
		
		// Limpiando pantalla.
    
    	system("cls");
		
		// Llamando a la función conectar_bd para establecer conexión con la base de datos.
    
    	MYSQL* conexion = conectar_bd();
    
    	// Preparar la consulta SQL.
    
   		string consulta = "SELECT idproducto, nombre, descripcion, precio, existencia FROM producto WHERE franja_idfranja = " + to_string(idFranjas);

    	// Ejecutar la consulta SQL.
    
    	mysql_query(conexion, consulta.c_str());

    	// Obtener el resultado de la consulta.
    
    	MYSQL_RES* resultado = mysql_store_result(conexion);
    	
	    // Mostrando datos de los productos.
	    
	    cout<<"\n\t\t====================="<<endl;    
	    cout<<"\t\tPRODUCTOS DISPONIBLES"<<endl;
	    cout<<"\t\t=====================\n"<<endl;  
	    
	    MYSQL_ROW fila;
	        
	    while ((fila = mysql_fetch_row(resultado)) != NULL) {
	        
	        cout<<"\t"<<contador<<". Nombre: "<<fila[1]<<endl;
	        cout<<"\t   Descripción: "<<fila[2]<<endl;
	        cout<<"\t   Precio: " <<fila[3]<<endl;
	        cout<<"\t   En Existencia: " <<fila[4]<<" unidades"<<endl;
	        cout<<"\t   ID: " <<fila[0]<<"\n"<<endl;
	        contador = ++contador;
	    }
	    
	    // Solicitando al usuario que ingrese los productos que desea comprar.
	    
	    cout<<"\n\tEliga el ID producto, ingrese 0 para terminar\n"<<endl;
	    cout<<"\t";cin>>id;

	    if(id != 0){
	    	
	    	lpedido = ++lpedido;
	    	
	    	// Preparando la consulta SQL e insertar los datos en la tabla detalle_pedido.
			
    		string consulta = "INSERT INTO detalle_pedido (producto_idproducto, pedido_idpedido, linea_pedido) VALUES ('" + to_string(id) + "', '" + to_string(idPedido) + "', '" + to_string(lpedido) + "')";
			mysql_query(conexion, consulta.c_str());
			
			// Preparando la consulta SQL para actualizar la existencia de los productos.
			
			string eliminar = "UPDATE producto SET existencia = existencia - 1 WHERE idproducto = '" + to_string(id) + "' AND franja_idfranja = '" + to_string(idFranjas) + "'";
    		mysql_query(conexion, eliminar.c_str());
				
			// Cerrar la conexión a la base de datos
    
    		mysql_close(conexion);
	    	
			cout<<"\n\t\t=== Su producto ha sido agregado ===\n"<<endl;
		}
		else
			cout<<"\n\t\t=== Productos agregados con éxito ===\n"<<endl;	
			
	    system("pause");
	    contador = 1;
	    
    }while(id != 0);
    
    // Llamando al método info_pedidos().
    
    info_pedido(idPedido);
    
}

void info_pedido(int idPedido){
	
	// Limpiando pantalla.
	
	system("cls");
	
	//Declaración de variables.
	
	string franja;	
	datos vardatos;
	
	// Conectarse a la base de datos.
	
    MYSQL* conexion = conectar_bd();
    
    // Consulta para obtener los datos del cliente.
    
    string consultaCliente = "SELECT c.idcliente, c.nombre, c.apellido, f.nombre AS franja "
							 "FROM cliente c "
							 "INNER JOIN pedido p ON c.idcliente = p.cliente_idcliente "
							 "INNER JOIN franja f ON p.franja_idfranja = f.idfranja "
							 "WHERE p.idpedido = " + to_string(idPedido);
    mysql_query(conexion, consultaCliente.c_str());
    
    MYSQL_RES* resultadoCliente = mysql_store_result(conexion);
    
    if (MYSQL_ROW filaCliente = mysql_fetch_row(resultadoCliente)) {
        vardatos.nombre = filaCliente[1];
        vardatos.apellido = filaCliente[2];
        vardatos.DPI = filaCliente[0];
        franja = filaCliente[3];
    }
    
    // Consulta para obtener los productos elegidos.
    
    string consulta_productos = "SELECT p.nombre, p.descripcion, p.precio, dp.linea_pedido "
                                "FROM producto p "
                                "INNER JOIN detalle_pedido dp ON p.idproducto = dp.producto_idproducto "
                                "INNER JOIN pedido pe ON dp.pedido_idpedido = pe.idpedido "
                                "WHERE dp.pedido_idpedido = '" + to_string(idPedido) + "'";

    mysql_query(conexion, consulta_productos.c_str());
    MYSQL_RES* resultado_productos = mysql_store_result(conexion);
    MYSQL_ROW fila_producto;
    
    // Mostrar información de los productos elegidos.
    
    cout<<"\n\t      =================="<<endl;    
	cout<<"\t      DETALLE DEL PEDIDO"<<endl;
	cout<<"\t      ==================\n"<<endl;    
    cout<<"\t=== Información del Cliente ===\n"<<endl;
    cout<<"\tNombre: "<<vardatos.nombre<<endl;
    cout<<"\tApellido: "<<vardatos.apellido<<endl;
    cout<<"\tNo. de DPI: "<<vardatos.DPI<<"\n\n"<<endl;
    cout<<"\t  === Productos Comprados ==="<<endl;
    cout<<"\t       Franja: "<<franja<<"\n"<<endl;
    while ((fila_producto = mysql_fetch_row(resultado_productos)) != nullptr) {
        string nombre = fila_producto[0];
        string descripcion = fila_producto[1];
        string precio = fila_producto[2];
        string linea_pedido = fila_producto[3];
   		cout<<"       "<<linea_pedido<<". Nombre: "<<nombre<<endl;
    	cout<<"\t  Descripción: "<<descripcion<<endl;
    	cout<<"\t  Precio: "<<precio<<endl;
		cout<<endl;
    }
    
    cout<<"\n\n\t=== Su número de orden es: "<<idPedido<<" ==="<<endl;

    // Liberar recursos y cerrar la conexión a la base de datos.
    
    mysql_free_result(resultadoCliente);
	mysql_free_result(resultado_productos);
    mysql_close(conexion);
}
