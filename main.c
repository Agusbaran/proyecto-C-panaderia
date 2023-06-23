#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
    char nombre_preparacion[20]; /// venta
    int cantidad;
} PreparacionVenta;

typedef struct
{
    char nombre_preparacion[20];  ///demanda
    int cantidad;
} Preparacion;

typedef struct
{
    char nombre_ingrediente[20]; /// ingredientes
    float cantidad;
} IngredienteXReceta;

typedef struct
{
    char nombre_preparacion[20];  /// receta
    IngredienteXReceta ingredientes[20];
    int cantIngredientes;
} Receta;

typedef struct
{
    char nombre_ingrediente[20];
    float cantidad;  //en kg o en litro segun el tipo de ingrediente
    char tipo[20]; /// "liquido" "solido"
    float costo; ///costo por kg o por litro según corresponda
} StockIngrediente;

typedef struct
{
    char nombre_preparacion[20];        /// precios
    float precio_venta; ///precio por unidad
} PrecioPreparacion;

typedef struct
{
    char nombre_preparacion[20];
    int cantidad;
} PedidoPreparacion;

typedef struct
{
    int idVenta;
    PedidoPreparacion items_pedido[20]; ///puedo pedir hasta 20 items
    int cantItems; ///los validos del arreglo de items_pedido
    float valor_total; ///valor total a pagar
} Venta;


void demandaYrecetas();
void MuestraStock ();
int buscarEnIngrediente(char nombre[]);
void elaboracion();
void cargarPrecio();
void modificarPrecio();
void subMenuPrecios();
int buscarEnPrecio(char nombre[]);
void ventas();
int buscarEnPreparados(char nombre[]);
void reducirPreparado(char nombre[], int cantidad);
void MuestraRemanente ();
void mostrarPrecios();


int main()
{
    int a;
    char rep = 's';

    printf("Menu de opciones del sistema:\n");
    printf("\n");
    printf("1: mostrar stock de ingredientes\n");
    printf("2: mostrar demanda del dia y recetas\n");
    printf("3: ejecutar elaboracion\n");
    printf("4: mostrar submenu de precios\n");
    printf("5: mostrar remanente\n");
    printf("6: mostrar pre venta\n");
    printf("7: cargar ventas\n");
    printf("\n");

    do
    {
        printf("que accion desea ejecutar? ");
        scanf("%d", &a);
        printf("\n");
        printf("\n");

        if(a > 0 && a < 8)
        {
            switch(a)
            {
            case 1:
                MuestraStock();
                break;

            case 2:
                demandaYrecetas();
                break;

            case 3:
                elaboracion();
                break;

            case 4:
                subMenuPrecios();
                break;

            case 5:
                MuestraRemanente ();
                break;

            case 6:
                muestraPreVenta();
                break;

            case 7:
                ventas();
                break;
            }



            printf("desea ejecutar otra accion? ");
            fflush(stdin);
            scanf("%c", &rep);
        }
        else
        {
            printf("opcopn no valida, por favor ingrese nuevamente\n");
        }
    }
    while(rep == 's' || rep == 'S');



    return 0;
}

void muestraPreVenta()
{
    FILE *f = fopen("stockventa.bin", "rb");
    PreparacionVenta v;

    if(f != NULL)
    {
        printf("stock para la venta\n");
        printf("\n");

        while(fread(&v, sizeof(PreparacionVenta), 1, f) > 0)
        {
            printf("%s\n", v.nombre_preparacion);
            printf("%d unidades\n", v.cantidad);
            printf("\n");
        }
    }
    else
    {
        printf("no se pudo abrir el archivo de stock ventas\n");
    }

    printf("====================\n");

    fclose(f);

}

void demandaYrecetas()
{
    /// DEMANDA

    FILE *bin = fopen("demanda.bin", "rb");
    Preparacion a;

    if(bin != NULL)
    {
        printf("DEMANDA DEL DIA\n");
        printf("\n");

        while(fread(&a, sizeof(Preparacion), 1, bin) > 0)
        {
            printf("%s\n", a.nombre_preparacion);
            printf("%d unidades\n", a.cantidad);
            printf("\n");
        }
    }
    else
    {
        printf("no se pudo abrir el archivo de demanda\n");
    }

    printf("====================\n");

    fclose(bin);

    ///RECETAS

    FILE *arch = fopen("recetas.bin", "rb");
    Receta b;
    int valid;

    if(arch != NULL)
    {
        printf("\n");
        printf("\n");
        printf("RECETAS\n");
        printf("\n");


        while(fread(&b, sizeof(Receta), 1, arch) > 0)
        {
            valid = b.cantIngredientes;

            printf("%s\n", b.nombre_preparacion);
            printf("\n");
            printf("%d ingredientes\n", b.cantIngredientes);
            printf("\n");

            for(int i = 0; i < valid; i++)
            {
                printf("%s\n", b.ingredientes[i].nombre_ingrediente);
                printf("%f kg/L\n", b.ingredientes[i].cantidad);
            }

            printf("\n--------------------\n");
        }

    }
    else
    {
        printf("no se pudo abrir el archivo de rcetas");
    }

    printf("====================\n");

    fclose(arch);
}

void elaboracion()
{
    FILE *DEM = fopen("demanda.bin", "rb");
    FILE *REC = fopen("recetas.bin", "rb");
    FILE *VEN = fopen("stockventa.bin", "wb");
    FILE *ING = fopen("stockingredientes.bin", "r + b");
    Preparacion dem;
    Receta rec;
    PreparacionVenta ven;
    StockIngrediente ing;
    int valid, pos, k;
    ///char nombre[20];

    if(DEM != NULL)
    {
        if(REC != NULL)
        {
            if(VEN != NULL)
            {
                while(fread(&dem, sizeof(Preparacion), 1, DEM) > 0)
                {

                    fread(&rec, sizeof(Receta), 1, REC);

                    valid = dem.cantidad;

                    for(int i = 0; i < valid; i++)/// demanda
                    {
                        ven.cantidad = 0;
                        pos = rec.cantIngredientes; /// pos = validos ingrdientes receta

                        for(int j = 0; j < pos; j++)/// rec.ingredientes
                        {
                            //strcpy(nombre, rec.ingredientes[j].nombre_ingrediente)
                            //buscarEnIngrediente(nombre[20]);

                            k = 0;

                            while(k == 0)
                            {
                                if(strcmpi(dem.nombre_preparacion, rec.nombre_preparacion) == 0)
                                {
                                    while(rec.ingredientes[j].cantidad <= ing.cantidad)
                                    {
                                        ing.cantidad = ing.cantidad - rec.ingredientes[j].cantidad;
                                        ven.cantidad++;
                                    }

                                    k = 1;
                                }
                                else
                                {
                                    fread(&rec, sizeof(Receta), 1, REC);
                                }

                            }



                            fseek(ING, -1, SEEK_CUR);
                            fwrite(&ing, sizeof(StockIngrediente), 1, ING); /// modifico el stock ingredientes
                            fseek(ING, 1, SEEK_CUR);
                        }


                        strcpy(ven.nombre_preparacion, dem.nombre_preparacion);/// cargo el stock d ventas
                        fwrite(&ven, sizeof(PreparacionVenta), 1, VEN);
                        rewind(ING);
                        rewind(REC);
                    }
                }




            }
            else
            {
                printf("no se pudo abrir el archivo de ingredientes");
            }
        }
        else
        {
            printf("no se pudo abrir el archivo de recetas");
        }
    }
    else
    {
        printf("no se pudo abrir el archivo de demandas");
    }

    fclose(DEM);
    fclose(REC);
    fclose(VEN);
    fclose(ING);

    printf("elaboracion ejecutada\n");
}

int buscarEnIngrediente(char nombre[])
{
    int pos = -1;
    int i=0;
    StockIngrediente a;
    FILE* fp;
    fp = fopen("stockingredientes.bin","rb");
    if (fp != NULL)
    {
        while ((fread(&a,sizeof(StockIngrediente),1,fp) > 0) && (pos == -1))
        {
            if (strcmp(nombre,a.nombre_ingrediente)== 0)
            {
                pos = i;

            }
            i++;
        }
    }

    fclose(fp);
    return pos;
}

void MuestraStock ()
{
    FILE* fp;
    StockIngrediente a; //cambio nombre a la estructura
    fp = fopen("stockingredientes.bin", "rb");
    int i = 1;

    printf("STOCK DE INGREDIENTES\n");
    printf("\n");

    if(fp != NULL)
    {
        while (!feof(fp)) //mientras no estoy posicionado en el final del archivo hacer...
        {
            fread(&a, sizeof(StockIngrediente),1,fp); // mover de a 1 ingrediente teniendo en cuenta todas sus partes.
            if (!feof(fp)) ////si no estoy posicionado en el final del archivo hacer...
            {
                printf("\n Ingrediente Numero: %i.\n", i++);
                printf("\n Nombre: %s.\n", a.nombre_ingrediente);  //primer ingrediente toma nombre, cant, tipo y costo.
                printf("\n Cantidad: %.2f.\n", a.cantidad);        // a. = quiero guardar la informacion en...
                printf("\n Tipo %s.\n", a.tipo);
                printf("\n Costo $%.2f.\n", a.costo);
                printf("\n");
            } //una vez cargado el 1er ingrediente, vuelve a cargar el 2do siempre y cuando no este al final del archivo.
        }
        fclose(fp);
    }
}

///sub-menu case 1
///PRIMERO CARGO PREPARACION, LUEGO EL PRECIO
void cargarPrecio()
{
    char nombre[20];
    PrecioPreparacion a;
    FILE *pr;
    pr = fopen("precios.bin", "ab");

    printf("que precio desea cargar?");
    fflush(stdin);
    gets(nombre);


    if (pr != NULL)
    {
        strcpy(a.nombre_preparacion,nombre);
        printf("Ingrese el precio del preparado: ");
        scanf("%f",&a.precio_venta);
        fwrite(&a,sizeof(PrecioPreparacion),1,pr);


    }

    fclose(pr);

}

///sub-menu case 2
void modificarPrecio()
{

    FILE *pr;
    int i;
    char nombre[20];

    PrecioPreparacion venta;

    printf("Ingrese la preparacion que desea modificarle el precio");
    fflush(stdin);
    gets(nombre);

    i=buscarEnPrecio(nombre); ///busco el preparado a modificar el precio, devuelve posicion del preparado en archivo venta

    if (i != -1)
    {
        pr = fopen("precios.bin", "w+b");
        fseek(pr,sizeof(PrecioPreparacion)*i,SEEK_SET); ///multiplico y llevo el cursor en la posicion correcta
        if(pr != NULL)
        {
            fread(&venta,sizeof(PrecioPreparacion),1,pr);

            printf("Modifique el precio del preparado: ");
            scanf("%f",&venta.precio_venta);
            fwrite(&venta,sizeof(PrecioPreparacion),1,pr);
        }



    }
    fclose(pr);

}

int buscarEnPrecio(char nombre[])
{
    int pos = -1;
    int i=0;
    PrecioPreparacion venta;
    FILE* fp;
    fp = fopen("precios.bin","rb");
    if (fp != NULL)
    {
        while ((fread(&venta,sizeof(PrecioPreparacion),1,fp) > 0) && (pos == -1))
        {
            if (strcmp(nombre,venta.nombre_preparacion)== 0)
            {
                pos = i;

            }
            i++;
        }
    }

    fclose(fp);
    return pos;
}

void mostrarPrecios()
{
    FILE* fp;
    fp=fopen("precios.bin", "rb");
    PrecioPreparacion a;

    if(fp != NULL)
    {
        printf("PRECIOS\n");
        printf("\n");

        while(fread(&a, sizeof(PrecioPreparacion), 1, fp) > 0)
        {
            printf("%s\n", a.nombre_preparacion);
            printf("%f\n", a.precio_venta);
            printf("\n");
        }
    }
    else
    {
        printf("no se pudo abrir el archivo de precios\n");
    }

    printf("====================\n");

    fclose(fp);
}

void subMenuPrecios()
{

    ///sub-menu para cargar o modificar precios

    int op;
    int flag =0;

    char nombre[20];

    do
    {
        printf("Menu de opciones:\n");
        printf("\n");
        printf("1: Cargar precio\n");
        printf("\n");
        printf("2: Modificar precio\n");
        printf("\n");
        printf("3: Lista precios\n");
        printf("\n");
        printf("4: Salir\n");
        printf("\n");
        printf("Marque la accion que desea realizar: ");
        scanf("%d", &op);
        printf("\n");
        printf("\n");


        switch(op)
        {
        case 1:


            cargarPrecio();
            break;

        case 2:
            modificarPrecio();
            break;

        case 3:
            mostrarPrecios();
            break;


        case 4:
            flag = 1;
            break;

        default:
            printf("Opcion no valida");
            break;


        }



    }
    while(flag == 0);


}

void ventas()
{
    FILE *v;
    Venta a;
    v = fopen("ventas.bin","ab");
    int valido=20; /// puede pedir hasta 20 items
    char continuar;
    int i = 0;
    if ( v != NULL)
    {

        do
        {
            printf("Ingrese id de venta: ");
            scanf("%i",&a.idVenta);
            printf("Ingrese cantidad de items : ");
            scanf("%i",&a.cantItems);
            printf("Ingrese valor total: ");
            scanf("%f",&a.valor_total);
            printf("Ingrese item de su pedido: ");
            scanf("%s", &a.items_pedido[i].nombre_preparacion);
            printf("Desea cargar otro item?");
            fflush(stdin);
            scanf("%c", &continuar);
            fwrite(&a,sizeof(Venta),1,v);

            i++;

        }
        while((continuar =='s' || continuar == 'S') && i < valido);


    }
    fclose(v);

}

int buscarEnPreparados(char nombre[])
{
    int pos = -1;
    int i=0;
    PreparacionVenta a;
    FILE* fp;
    fp = fopen("demanda.bin","rb");
    if (fp != NULL)
    {
        while ((fread(&a,sizeof(PreparacionVenta),1,fp) > 0) && (pos == -1))
        {
            if (strcmp(nombre,a.nombre_preparacion)== 0)
            {
                pos = i;

            }
            i++;
        }
    }

    fclose(fp);
    return pos;
}

void reducirPreparado(char nombre[], int cantidad)   ///recibo nombre mas cantidad a reducir
{
    int pos;
    PreparacionVenta a;

    FILE* pr;
    pos = buscarEnPreparados(nombre);
    pr = fopen("demanda.bin", "w+b");
    if ( pr != NULL)
    {
        fseek(pr,sizeof(PreparacionVenta)*pos, SEEK_SET); /// busca pos
        fread(&a, sizeof(PreparacionVenta),1,pr);
        a.cantidad-= cantidad;/// a.cantidad = a.cantidad-cantidad /// reducir en cant a la cantidad que hay
        fwrite(&a,sizeof(PreparacionVenta),1,pr);


    }
    fclose(pr);
}

void MuestraRemanente ()
{
    FILE *fp = fopen("stockingredientes.bin", "rb");
    StockIngrediente a;
    int i=1;

    if(fp != NULL)
    {
        printf("Remanente de ingredientes\n");
        printf("\n");

        while(fread(&a, sizeof(StockIngrediente), 1, fp) > 0 &&  a.cantidad>0)
        {
            printf("\n Ingrediente Numero: %i.\n", i++);
            printf("\n Nombre: %s.\n", a.nombre_ingrediente);
            printf("\n Cantidad: %.2f.\n", a.cantidad);

            printf("\n");
        }
    }


    printf("====================\n");

    fclose(fp);
}


float ingresototall()
{
    int validos;
    float ingresoTotal=0;
    Venta ve[20];
    FILE *fp;
    fp = fopen("ventas.bin", "rb");
    if (fp != NULL)
    {
        fseek(fp,0,SEEK_END);
        validos = ftell(fp)/sizeof(Venta);
        fseek(fp,0,SEEK_SET);

        for (int i=0; i<validos; i++)
        {
            fread(&ve[i],sizeof(Venta),1,fp);
            ingresoTotal = ingresoTotal + ve[i].valor_total;
        }
        /* for (int j=0; j<validos; j++)
         {
             if (ve[j].bajaLogica == 1)
             {
                 ingresoTotal = ingresoTotal + ve[j].valor_total;
             }*/fclose (fp);
    }
    //fclose (fp);
    return ingresoTotal;
}

float ganancias (float ingresoTotal, float costoTotal)
{

    float ganancia=0;

    ganancia = ingresoTotal - costoTotal;

    return ganancia;
}


