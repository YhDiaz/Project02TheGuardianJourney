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
		Village* previous_;
		Village* next_;
};

//Grafo: Aldeas
class Graph
{
	
};

//Estructuras

//Elemento aldea
struct VillageNode
{
	public:
		
		//Constructores
		VillageNode();
		VillageNode(string, VillageNode*);
		
		//Destructor
		~VillageNode();
		
		//Metodos
		VillageNode* AddVillageNode(VillageNode**, string);
		void PrintList(VillageNode*);
		
	private:
		
		//Atributos
		string name_;
		VillageNode* next_;
		
		//Metodos
		VillageNode* CreateVillageNode(string);
};

//Funciones

void VillagesFileCountValidation(int count, bool* successRead); //Validacion de la cantidad de aldeas
//
void ReadVillagesFile(queue<Village> *villages, bool* successRead); //Leer archivo de aldeas
void ReadGuardiansFile(queue<Guardian> *guardians, bool* successRead); //Leer archivo de guardianes
void ReadFiles(queue<Guardian> *guardians, queue<Village> *villages, bool* successRead); //Leer archivos
void PrintGuardianQueue(queue<Guardian> guardians); //Imprimir cola de guardianes
void PrintVillagesQueue(queue<Village> villages); //Imprimir cola de aldeas

int main()
{
	queue<Guardian> guardians;
	queue<Village> villages;
	bool successRead = false;
	
	ReadFiles(&guardians, &villages, &successRead);
	
	if(successRead)
	{
		PrintGuardianQueue(guardians);
		PrintVillagesQueue(villages);
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
		previous_ = NULL;
		next_ = NULL;
	}
	
	//Destructor
	Village::~Village()
	{
		
	}
	
	//Metodos
	void Village::AddAdjacentVillageName(string adjacentVillageName)
	{
		adjacentVillagesNames_.push_back(adjacentVillageName);
	}
	
	//Setters
	void Village::SetName(string name)
	{
		name_ = name;
	}
	
	void Village::SetPrevious(Village* previous)
	{
		previous_ = previous;
	}
	
	void Village::SetNext(Village* next)
	{
		next_ = next;
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
	
	Village* Village::GetNext()
	{
		return next_;
	}
	
	//VillageNode
	
	//Constructores
	VillageNode::VillageNode()
	{
		
	}
	
	VillageNode::VillageNode(string name, VillageNode* next)
	{
		name_ = name;
		next_ = next;
	}
	
	//Destructor
	VillageNode::~VillageNode()
	{
		
	}
	
	//Metodos
	VillageNode* VillageNode::AddVillageNode(VillageNode** root, string villageName)
	{
		cout << "Entrando addNode";
		if(root == nullptr)
		{
			cout << "E";
			*root = CreateVillageNode(villageName);
			
			cout << "Pasa root";
			return *root;
		}
		else
		{
			VillageNode* current = *root;
			
			while(current->next_ != nullptr)
			{
				current = current->next_;
			}
			
			current->next_ = CreateVillageNode(villageName);
		}
		
		return nullptr;
	}
	
	VillageNode* VillageNode::CreateVillageNode(string name)
	{
		cout << "Entra creacion";
		VillageNode* newVillageNode = new VillageNode(name, nullptr);
		cout << "Pasa creacion";
		
		return newVillageNode;
	}
	
	void VillageNode::PrintList(VillageNode* root)
	{
		VillageNode* current = root;
		cout << "\n\tLista de aldeas:\n" << endl;
		
		int count = 1;
		
		while(current != nullptr)
		{
			cout << "\t\t* Aldea " << count << ": " << root->name_ << endl;
			count++;
		}
	}

//Funciones

bool VilllagesFileSameVillageValidation(string name, string adjacentVillageName)
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
	
	cout << "\n\tDEBUG count: " << count;
	
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
		*successRead = true;
	}
}

//Leer archivo de aldeas
void ReadVillagesFile(queue<Village> *villages, bool* successRead)
{
	string filename = "Villages.txt";
	ifstream file(filename);
	
	if(file.is_open())
	{
		string line;
		getline(file, line); //Omitir la primera linea correspondiente a los encabezados
		int countVillages = 0, countLines = 1;
		bool validation = false;
		
		VillageNode* villagesList = nullptr;
		
		getline(file, line); //Obtener primera linea
		
		do
		{
			countLines++; //Incremento del contador de lineas
			stringstream ss(line);
			string name, adjacentVillageName;
			
			getline(ss, name, ',');
			getline(ss, adjacentVillageName, ',');
			
			validation = VilllagesFileSameVillageValidation(name, adjacentVillageName);
			
			if(validation) //Si se supero la validacion, se agregan las aldeas a la lista
			{
				if(villagesList == nullptr)
				{
					cout << "Entra null";
					villagesList = villagesList->AddVillageNode(&villagesList, name);
				}
				else
				{
					villagesList->AddVillageNode(&villagesList, name);
				}
				
				villagesList->AddVillageNode(&villagesList, adjacentVillageName);
			}
			else
			{
				cout << "\n\t* Una aldea no puede ser adyacente de si misma (" << name << ", Linea " << countLines << " en el archivo " << filename << ") *" << endl;
			}
			
			countVillages++; //Incremento del contador de aldeas
		}
		while(getline(file, line) && validation);
		
		//villagesList->PrintList(villagesList);
		
		if(validation) //Si se superaron las validaciones de la lectura y guardado de datos, se continuan las validaciones
		{
			VillagesFileCountValidation(countVillages, successRead); //Validacion de la cantidad de aldeas (Minimo 2)
			//VillagesFileTeslaValidation(successRead);
		}
		
		if(*successRead)
		{
			cout << "\n\tDEBUG Validacion contador true";
		}
		
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

//Leer archivos
void ReadFiles(queue<Guardian> *guardians, queue<Village> *villages, bool* successRead)
{
	cout << "\tCargando archivos..." << endl;
	ReadVillagesFile(villages, successRead); //Archivo de aldeas
	
	if(*successRead) //Si no se leyo bien el primer archivo, no es necesario leer el segundo
	{
		cout << "\n\tDEBUG Lectura segundo archivo";
		ReadGuardiansFile(guardians, successRead); //Archivo de guardianes
	}
	
	if(*successRead)
	{
		cout << "\n\t* Archivos cargados exitosamente *";
	}
	else
	{
		cout << "\n\t* Algo salio mal. Por favor comprueba el nombre de los archivos y ejecuta nuevamente el programa *";
	}
}

//Imprimir cola de guardianes
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
}

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
}
