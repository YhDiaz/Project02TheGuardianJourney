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

//Funciones

void ReadGuardiansFile(queue<Guardian> *guardians, bool* successRead); //Leer archivo de guardianes
void ReadVillagesFile(queue<Village> *villages, bool* successRead); //Leer archivo de aldeas
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

//Funciones

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

//Leer archivo de aldeas
void ReadVillagesFile(queue<Village> *villages, bool* successRead)
{
	string filename = "Villages.txt";
	ifstream file(filename);
	
	if(file.is_open())
	{
		string line;
		getline(file, line); // Skip the header line
		
		while(getline(file, line))
		{
			stringstream ss(line);
			string name, adjacentVillageName;
			
			getline(ss, name, ',');
			getline(ss, adjacentVillageName, ',');
			
			Village* village = new Village(name, adjacentVillageName); //Creacion de una nueva aldea	
			villages->push(*village); //Se agrega la aldea a la cola
			
			/*
			Village* current;
			
			if(!villages->empty())
			{
				current = &villages->front();
			}
			
			bool found = false;
			
			while(current != NULL && !found)
			{
				if(current->GetName() == name)
				{
					found = true;
					current->AddAdjacentVillageName(adjacentVillageName);
				}
				else
				{
					current = current->GetNext();
				}
			}
			
			if(!found)
			{
				Village* village = new Village(name, adjacentVillageName); //Creacion de una nueva aldea
				
				/*if(!villages->empty()) //Si hay al menos un elemento en la cola, se asignan previous y next
				{
					villages->back().SetNext(village);
					village->SetPrevious(&(villages->back()));
				}
				
				villages->push(*village); //Se agrega la aldea a la cola
			}
			*/		
		}
		
		file.close();
	}
	else
	{
		cout << "\n\tError al intentar abrir el archivo: " << filename << endl;
		*successRead = false; //El archivo no se leyo con exito
	}
}

//Leer archivos
void ReadFiles(queue<Guardian> *guardians, queue<Village> *villages, bool* successRead)
{
	cout << "\tCargando archivos..." << endl;
	ReadGuardiansFile(guardians, successRead); //Archivo de guardianes
	
	if(*successRead) //Si no se leyo bien el primer archivo, no es necesario leer el segundo
	{
		ReadVillagesFile(villages, successRead); //Archivo de aldeas
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

