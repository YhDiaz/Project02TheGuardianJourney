#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <limits>
#include <unordered_map>
#include <string>
using namespace std;

//Clases

//Guardian: Maestros y aprendices
class Guardian
{
	public:
		
		//Constructores
		Guardian(); //Constructor por defecto
		Guardian(string, int, string, string); //Nombre, nivel de poder, nombre del maestro, aldea
		
		//Destructor
		~Guardian();
		
		//Metodos
		void addApprentice(Guardian*); //Agregar aprendiz
		
		//Setters
		void SetName(string);
		void SetPowerLevel(int);
		void SetMainMasterName(string);
		void SetMainMaster(Guardian*);
		void SetVillage(string);
		
		//Getters
		string GetName();
		int GetPowerLevel();
		string GetMainMasterName();
		Guardian* GetMainMaster();
		string GetVillage();
		
	//Atributos
	private:
		
		string name_;
		int powerLevel_;
		string mainMasterName_;
		Guardian* mainMaster_;
		string village_;
		vector<Guardian> apprentices_;
};

//Aldea
class Village
{
	public:
		
		//Constructores
		Village(); //Constructor por defecto
		Village(string, string); //Nombre, nombre de la aldea adyacente
		
		//Destructor
		~Village();
		
		//Metodos
		void AddAdjacentVillageName(string);
		
		//Setters
		void SetName(string);
		void SetPrevious(Village*);
		void SetNext(Village*);
		
		//Getters
		string GetName();
		vector<string> GetAdjacentVillagesNames();
		Village* GetNext();
	
	//Atributos
	private:
		
		string name_;
		vector<string> adjacentVillagesNames_;
};

//Grafo: Aldeas
class Graph
{
	
};

//Elemento aldea
class VillageNode
{
	public:
		
		//Constructores
		VillageNode(); //Constructor por defecto
		VillageNode(Village*, VillageNode*); //Aldea, siguiente nodo
		
		//Destructor
		~VillageNode();
		
		//Metodos
		VillageNode* AddVillageNode(VillageNode**, string, string, int*); //Agregar un nuevo nodo
		void PrintFullList(VillageNode*); //Imprimir la lista de aldeas
		void PrintJustNames(VillageNode*); //Imprimir los nombres de las aldeas
		bool SearchVillage(VillageNode*, string); //Buscar aldea
		
	private:
		
		//Atributos
		Village* village_;
		VillageNode* next_;
		
		//Metodos
		VillageNode* CreateVillageNode(string, string); //Crear un nuevo nodo
};

//Funciones

bool VillagesFileSameVillageValidation(string, string); //Validacion para saber si la aldea es adyacente consigo misma
void VillagesFileCountValidation(int, bool*); //Validacion de la cantidad de aldeas
void VillagesFileTeslaValidation(int, bool*, VillageNode*); //Validacion de la existencia de Tesla (aldea principal)
void ReadVillagesFileInLoopValidations(bool*, VillageNode**, string, string, int*, int, string); //Validaciones en el loop de la lectura de aldeas
void ReadVillagesFilePostLoopValidations(bool, int, bool*, VillageNode*); //Validacioens post ciclo de lectura de aldeas
void ReadVillagesFile(VillageNode**, bool*); //Leer archivo de aldeas

//VERIFICAR
void ReadGuardiansFile(queue<Guardian> *guardians, bool* successRead); //Leer archivo de guardianes

void FilesInfo(); //Informacion sobre el contenido de los archivos en caso de falla en la lectura

//VERIFICAR
void ReadFiles(queue<Guardian> *guardians, VillageNode** villagesList, bool* successRead); //Leer archivos
//void PrintGuardianQueue(queue<Guardian> guardians); //Imprimir cola de guardianes
//void PrintVillagesQueue(queue<Village> villages); //Imprimir cola de aldeas

int main()
{
	queue<Guardian> guardians;
	VillageNode* villages = nullptr;
	bool successRead = false;
	
	ReadFiles(&guardians, &villages, &successRead);
	
	if(successRead)
	{
		villages->PrintFullList(villages);
		villages->PrintJustNames(villages);
	}
	
	return 0;
}

//Definiciones
	
	//Guardian
	
	//Constructores
	Guardian::Guardian()
	{
		
	}
	
	Guardian::Guardian(string name, int powerLevel, string mainMasterName, string village)
	{
		name_ = name;
		powerLevel_ = powerLevel;
		mainMasterName_ = mainMasterName;
		village_ = village;
	}
	
	//Destructor
	Guardian::~Guardian()
	{
		
	}
	
	//Metodos
	
	//Setters
	
	//Getters
	string Guardian::GetName()
	{
		return name_;
	}
	
	int Guardian::GetPowerLevel()
	{
		return powerLevel_;
	}
	
	string Guardian::GetMainMasterName()
	{
		return mainMasterName_;
	}
	
	Guardian* Guardian::GetMainMaster()
	{
		return mainMaster_;
	}
	
	string Guardian::GetVillage()
	{
		return village_;
	}
	
	//Village
	
	//Constructores
	Village::Village()
	{
		
	}
	
	Village::Village(string name, string adjacentVillageName)
	{
		name_ = name;
		adjacentVillagesNames_.push_back(adjacentVillageName);
	}
	
	//Destructor
	Village::~Village()
	{
		
	}
	
	//Metodos
	void Village::AddAdjacentVillageName(string adjacentVillageName)
	{
		bool villageDoesntExist = true; //La aldea no esta en la lista de aldeas adyacentes
		
		for(const auto& it : adjacentVillagesNames_)
		{
			if(it == adjacentVillageName) //La aldea esta en la lista de aldeas adyacentes
			{
				villageDoesntExist = false;
			}
		}
		
		if(villageDoesntExist) //Si la aldea no esta en la lista de aldeas adyacentes, se agrega
		{
			adjacentVillagesNames_.push_back(adjacentVillageName);
		}		
	}
	
	//Setters
	void Village::SetName(string name)
	{
		name_ = name;
	}
	
	//Getters
	string Village::GetName()
	{
		return name_;
	}
	
	vector<string> Village::GetAdjacentVillagesNames()
	{
		return adjacentVillagesNames_;
	}
	
	//VillageNode
	
	//Constructores
	VillageNode::VillageNode()
	{
		
	}
	
	VillageNode::VillageNode(Village* village, VillageNode* next)
	{
		village_ = village;
		next_ = next;
	}
	
	//Destructor
	VillageNode::~VillageNode()
	{
		
	}
	
	//Metodos	
	VillageNode* VillageNode::AddVillageNode(VillageNode** root, string villageName, string adjacentVillageName, int* countVillages)
	{
		bool rootReturn = false;
		
		if(*root == nullptr)
		{
			VillageNode* aux = *root;
			
			aux = CreateVillageNode(villageName, adjacentVillageName); //Asignacion del root de la lista de aldeas
			aux->next_ = CreateVillageNode(adjacentVillageName, villageName); //La segunda aldea en la lista es la adyacente al root
			*root = aux;			
			*countVillages += 1; //Incremento del contador de aldeas
		}
		else
		{
			VillageNode* current = *root;
			
			//Variables que indican que las aldeas no existen en la lista (true) o que si existen (false)
			bool firstVillageNodeDoesntExist = true, secondVillageNodeDoesntExist = true;
			
			while(current->next_ != nullptr && (firstVillageNodeDoesntExist || secondVillageNodeDoesntExist))
			{
				if(current->village_->GetName() == villageName) //La primera aldea ya existe en la lista
				{
					firstVillageNodeDoesntExist = false;
					current->village_->AddAdjacentVillageName(adjacentVillageName); //Solo se agrega la aldea adyacente de la primera aldea
				}
				
				if(current->village_->GetName() == adjacentVillageName) //La segunda aldea ya existe en la lista
				{
					secondVillageNodeDoesntExist = false;
					current->village_->AddAdjacentVillageName(villageName); //Solo se agrega la aldea adyacente de la segunda aldea
				}
				
				current = current->next_;
			}
			
			if(firstVillageNodeDoesntExist && current->village_->GetName() != villageName) //Si la primera aldea no existe en la lista, se agrega
			{
				current->next_ = CreateVillageNode(villageName, adjacentVillageName);
				current = current->next_; //El puntero de la lista pasa al nodo creado, ya que es el ultimo
				*countVillages += 1; //Incremento del contador de aldeas
			}
			
			if(secondVillageNodeDoesntExist && current->village_->GetName() != adjacentVillageName) //Si la segunda aldea no existe en la lista, se agrega
			{
				current->next_ = CreateVillageNode(adjacentVillageName, villageName);
				*countVillages += 1;
			}
		}
		
		if(rootReturn)
		{
			return *root;
		}
		
		return nullptr;
	}
	
	VillageNode* VillageNode::CreateVillageNode(string villageName, string adjacentVillageName)
	{
		Village* village = new Village(villageName, adjacentVillageName); //Creacion de una nueva aldea
		VillageNode* newVillageNode = new VillageNode(village, nullptr); //Creacion de un nuevo elemento aldea
		
		return newVillageNode;
	}
	
	void VillageNode::PrintFullList(VillageNode* root)
	{
		VillageNode* current = root;
		cout << "\n\t------------------------- LISTA DE ALDEAS -------------------------\n" << endl;
		
		int count = 1;
		
		while(current != nullptr)
		{
			cout << "\t--------------- ALDEA " << count << " ---------------\n\n\t* Nombre: " << current->village_->GetName();
			cout << "\n\t* Aldeas adyacentes:\n";
			
			for(const auto& village : current->village_->GetAdjacentVillagesNames())
			{
				cout << "\t\t- " << village << endl;
			}			
			
			cout << endl;
			
			count++;
			current = current->next_;
		}
	}
	
	void VillageNode::PrintJustNames(VillageNode* root)
	{
		VillageNode* current = root;
		cout << "\n\t--------------- LISTA DE ALDEAS ---------------\n" << endl;
		
		int count = 1;
		
		while(current != nullptr)
		{
			cout << "\t- Aldea " << count << ": " << current->village_->GetName() << endl;
			count++;
			current = current->next_;
		}
	}
	
	bool VillageNode::SearchVillage(VillageNode* root, string villageName)
	{
		VillageNode* current = root;
		
		while(current != nullptr)
		{
			if(current->village_->GetName() == villageName)
			{
				return true; //Se encontro la aldea
			}
			
			current = current->next_;
		}
		
		return false; //Salir del bucle indica que no se encontro la aldea
	}

//Funciones

//Validacion para saber si la aldea es adyacente consigo misma
bool VillagesFileSameVillageValidation(string name, string adjacentVillageName)
{
	if(name == adjacentVillageName)
	{
		return false;
	}
	
	return true;
}

//Validacion de la cantidad de aldeas del archivo de aldeas
void VillagesFileCountValidation(int count, bool* successRead)
{
	int minVillages = 2; //Minimo 2 aldeas: Tesla y otra
	
	if(count < minVillages) //La cantidad de aldeas es menor a la cantidad minima
	{
		if(count == 0) //Archivo vacio
		{
			cout << "\n\t* El archivo esta vacio (Minimo de aldeas requeridas: " << minVillages << ") *" << endl;
		}
		else //Archivo con una sola aldea
		{
			cout << "\n\t* El archivo solo tiene una aldea (Minimo de aldeas requeridas: " << minVillages << ") *" << endl;
		}
		
		*successRead = false; //Lectura de archivos fallida
	}
	else
	{
		*successRead = true; //Cantidad de aldeas correcta
	}
}

//Validacion de la existencia de Tesla (aldea principal)
void VillagesFileTeslaValidation(int count, bool* successRead, VillageNode* villagesList)
{
	bool teslaFound = villagesList->SearchVillage(villagesList, "Tesla"); //Se busca Tesla en la lista de aldeas
	*successRead = teslaFound; //El exito de la validacion depende de haber encontrado Tesla en la lista
	
	if(count >= 2 && !teslaFound)
	{
		cout << "\n\t* La cantidad de aldeas es correcta pero Tesla no existe *" << endl;
	}
}

//Validaciones mientras se ejecuta el ciclo de lectura del archivo de aldeas
void ReadVillagesFileInLoopValidations(bool* validation, VillageNode** villagesList, string name, string adjacentVillage, int* countVillages, int countLines, string filename)
{
	*validation = VillagesFileSameVillageValidation(name, adjacentVillage); //La aldea y su adyacente no son iguales
	VillageNode* tempList = *villagesList;

	if(*validation) //Si se supero la validacion, se agregan las aldeas a la lista
	{
		if(*villagesList == nullptr)
		{
			*villagesList = tempList->AddVillageNode(villagesList, name, adjacentVillage, countVillages);
		}
		
		tempList->AddVillageNode(villagesList, name, adjacentVillage, countVillages);
	}
	else
	{
		cout << "\n\t* Una aldea no puede ser adyacente de si misma (" << name << ", Linea " << countLines << " en el archivo " << filename << ") *" << endl;
	}
}

//Validaciones post ciclo de lectura del archivo de aldeas
void ReadVillagesFilePostLoopValidations(bool validation, int countVillages, bool* successRead, VillageNode* villagesList)
{
	if(validation) //Si se superaron las validaciones de la lectura y guardado de datos, se continuan las validaciones
	{
		VillagesFileCountValidation(countVillages, successRead); //Validacion de la cantidad de aldeas (Minimo 2)
		VillagesFileTeslaValidation(countVillages, successRead, villagesList); //Validacion de la existencia de Tesla (aldea principal)
	}
}

//Leer archivo de aldeas
void ReadVillagesFile(VillageNode** villagesList, bool* successRead)
{
	string filename = "Villages.txt";
	ifstream file(filename);
	
	if(file.is_open())
	{
		string line;
		getline(file, line); //Omitir la primera linea correspondiente a los encabezados
		int countVillages = 0, countLines = 1;
		bool validation = true;
		
		while(getline(file, line) && validation)
		{
			countLines++; //Incremento del contador de lineas
			stringstream ss(line);
			string name, adjacentVillageName;
			
			getline(ss, name, ',');
			getline(ss, adjacentVillageName, ',');
			
			ReadVillagesFileInLoopValidations(&validation, villagesList, name, adjacentVillageName, &countVillages, countLines, filename); //Validaciones y agregacion de aldeas a la lista
		}
		
		//VillageNode* tempVillagesList = *villagesList;
		//tempVillagesList->PrintList(*villagesList);
		
		ReadVillagesFilePostLoopValidations(validation, countVillages, successRead, *villagesList); //Validaciones post ciclo
		
		file.close();
	}
	else
	{
		cout << "\n\tError al intentar abrir el archivo: " << filename << endl;
		*successRead = false; //El archivo no se leyo con exito
	}
}

//Leer archivo de guardianes
void ReadGuardiansFile(queue<Guardian> *guardians, bool* successRead)
{
	string filename = "Guardians.txt";
	ifstream file(filename);
	
	if(file.is_open())
	{
		string line;
		getline(file, line); //Omitir la primera linea correspondiente a los encabezados
		
		while(getline(file, line))
		{
			stringstream ss(line);
			string name, powerLevel, mainMasterName, village;
			
			getline(ss, name, ',');
			getline(ss, powerLevel, ',');
			getline(ss, mainMasterName, ',');
			getline(ss, village, ',');
			
			Guardian* guardian = new Guardian(name, stoi(powerLevel), mainMasterName, village); //Creacion de un nuevo guardian
			guardians->push(*guardian); //Se agrega el guardian a la cola
		}
		
		*successRead = true; //El archivo se leyo correctamente
		file.close();
	}
	else
	{
		cout << "\n\tError al intentar abrir el archivo: " << filename << endl;
		*successRead = false; //El archivo no se leyo correctamente
	}
}

//Informacion que deben tener los archivos
void FilesInfo()
{
	cout << "\n\t---------------------------------------- INFORMACION REQUERIDA Y RESTRICCIONES EN LOS ARCHIVOS ----------------------------------------\n" << endl;
	cout << "\t- Archivo de aldeas:\n\n\t\t1. La cantidad minima de aldeas es 2\n\t\t2. Una aldea no puede ser adyacente consigo misma\n\t\t3. Tesla debe estar en el archivo, ya que es la ciudad principal" << endl;
	cout << "\n\t- Archivo de guardianes:\n\n\t\t1. La aldea de los guardianes debe existir, es decir, debe estar en el archivo de aldeas\n\t\t2. Una aldea debe tener, como minimo, 1 maestro y 1 aprendiz" << endl;
	cout << "\n\tObservacion: Es probable que se haya mostrado un mensaje mas arriba sobre el error" << endl;
}

//Leer archivos
void ReadFiles(queue<Guardian> *guardians, VillageNode** villagesList, bool* successRead)
{
	cout << "\tCargando archivos..." << endl;
	ReadVillagesFile(villagesList, successRead); //Archivo de aldeas
	
	if(*successRead) //Si no se leyo bien el primer archivo, no es necesario leer el segundo
	{
		cout << "\n\tDEBUG Lectura segundo archivo";
		ReadGuardiansFile(guardians, successRead); //Archivo de guardianes
	}
	
	*successRead = true;
	
	if(*successRead)
	{
		cout << "\n\t* Archivos cargados exitosamente *" << endl;
	}
	else
	{
		cout << "\n\t* Algo salio mal. Por favor, corrobora que el archivo que los archivos contengan lo siguiente y ejecuta nuevamente el programa *" << endl;
		FilesInfo();
	}
}

/*Imprimir cola de guardianes
void PrintGuardianQueue(queue<Guardian> guardians)
{
	queue<Guardian> temp = guardians; //Copia de la cola de guardianes
	
	cout << "\n\tLista de guardianes:\n" << endl;
	int count = 1;
	
	while(!temp.empty()) //Mientras la copia tenga elementos, se muestran
	{
	  	Guardian guardian = temp.front(); //Se obtiene el elemento que esta al frente en la cola	
	  	cout << "\n\tGuardian " << count << "\n\n\t\t- Nombre: " << guardian.GetName() << "\n\t\t- Nivel de poder: " << guardian.GetPowerLevel();
    	
    	if(guardian.GetMainMasterName() == "") //El maestro mas poderoso no tiene maestro
    	{
    		cout << "\n\t\t- Maestro: No tiene, es el maestro principal";
		}
		else
		{
			cout << "\n\t\t- Maestro: " << guardian.GetMainMasterName();
		}
    	
    	cout << "\n\t\t- Aldea: " << guardian.GetVillage() << endl;
    	
    	count++; //Incremento del contador
    	temp.pop(); //Dequeue del primer elemento de la cola
	}
}*/

/*Imprimir cola de aldeas
void PrintVillagesQueue(queue<Village> villages)
{
	queue<Village> temp = villages; //Copia de la cola de aldeas
	
	cout << "\n\tLista de aldeas:" << endl;
	int count = 1;
	
	while(!temp.empty()) //Mientras la copia tenga elementos, se muestran
	{
  		Village village = temp.front(); //Se obtiene el elemento que esta al frente en la cola
  		
  		cout << "\n\tAldea " << count << "\n\n\t\t- Nombre: " << village.GetName() << "\n\n\t\t- Aldeas adyacentes:" << endl;
  		
  		for(const auto& value : village.GetAdjacentVillagesNames())
  		{
  			cout << "\n\t\t\t* " << value;
		}
    	
    	count++; //Incremento del contador
    	temp.pop(); //Dequeue del primer elemento de la cola
	}
}*/
