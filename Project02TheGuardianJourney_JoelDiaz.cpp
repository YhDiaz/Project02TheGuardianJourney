#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<queue>
#include<limits>
#include <unordered_map>
#include <string>
using namespace std;

//Variables globales
int minVillages = 3; //Minimo de aldeas
int minAdjacentVillages = 2; //Minimo de conexiones entre aldeas
int maxPower = 100; //Poder maximo de un guardian
int minPower = 25; //Poder minimo de un guardian
int minPowerMainMaster = 70; //Poder minimo del maestro principal
string mainVillage = "Tesla"; //Aldea principal

//Clases

//Guardian: Maestros y aprendices
class Guardian
{
	public:
		
		//Constructores
		Guardian() //Constructor por defecto
		{
			
		}
		
		Guardian(string name, int powerLevel, string mainMasterName, string village)
		{
			name_ = name;
			powerLevel_ = powerLevel;
			mainMasterName_ = mainMasterName;
			village_ = village;
		}
		
		//Destructor
		~Guardian()
		{
			
		}
		
		//Metodos
		void addApprentice(Guardian*) //Agregar aprendiz
		{
			
		}
		
		//Setters
		void SetName(string);
		void SetPowerLevel(int);
		void SetMainMasterName(string);
		void SetMainMaster(Guardian*);
		void SetVillage(string);
		
		//Getters
		string GetName() //Nombre
		{
			return name_;
		}
		
		int GetPowerLevel() //Nivel de poder
		{
			return powerLevel_;
		}
		
		string GetMainMasterName() //Nombre del maestro
		{
			return mainMasterName_;
		}
		
		Guardian* GetMainMaster() //Maestro
		{
			return mainMaster_;
		}
		
		string GetVillage() //Aldea
		{
			return village_;
		}
		
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
		Village() //Constructor por defecto
		{
			name_ = "";
			guardians_ = 0;
			adjacentVillages_ = 0;
		}
		
		Village(string name, string adjacentVillageName)
		{
			name_ = name;
			adjacentVillagesNames_.push_back(adjacentVillageName);
			adjacentVillages_ = 1;
			guardians_ = 0;
		}
		
		//Destructor
		~Village()
		{
			
		}
		
		//Metodos
		void AddAdjacentVillageName(string adjacentVillageName)
		{
			for(const auto& it : adjacentVillagesNames_)
			{
				if(it == adjacentVillageName) //La aldea esta en la lista de aldeas adyacentes
				{
					return;
				}
			}
			
			adjacentVillagesNames_.push_back(adjacentVillageName); //La aldea no estaba en la lista de adyacentes
			adjacentVillages_++; //Incremento en el contador de aldeas adyacentes
		}
		
		void IncrementNumOfGuardians()
		{
			guardians_++;
		}
		
		//Setters
		void SetName(string name)
		{
			name_ = name;
		}
		
		void SetPrevious(Village*);
		void SetNext(Village*);
		
		//Getters
		string GetName()
		{
			return name_;
		}
		
		vector<string> GetAdjacentVillagesNames()
		{
			return adjacentVillagesNames_;
		}
		
		int GetNumAdjacentVillages()
		{
			return adjacentVillages_;
		}
		
//Village* GetNext();
	
	//Atributos
	private:
		
		string name_;
		vector<string> adjacentVillagesNames_;
		int guardians_;
		int adjacentVillages_;
};

//Grafo: Aldeas
class Graph
{
	
};



//Nodo guardian (Para la lista de guardianes)
class GuardianNode
{
	public:
		
		//Constructores
		GuardianNode() //Constructor por defecto
		{
			guardian_ = nullptr;
			next_ = nullptr;
		}
		
		GuardianNode(Guardian* guardian)
		{
			guardian_ = guardian;
			next_ = nullptr;
		}
		
		//Destructor
		~GuardianNode()
		{
			
		}
		
		//Metodos
		void AddGuardianNode(GuardianNode** root, string name, int power, string master, string village)
		{
			if(*root == nullptr)
			{
				GuardianNode* aux = *root;
				aux = CreateGuardianNode(name, power, master, village); //Asignacion del root de la lista de guardianes
				*root = aux;
			}
			else
			{
				GuardianNode* current = *root;
				
				while(current->next_ != nullptr)
				{
					current = current->next_;
				}
				
				current->next_ = CreateGuardianNode(name, power, master, village);
			}
		}
		
		GuardianNode* CreateGuardianNode(string name, int power, string master, string village)
		{
			Guardian* guardian = new Guardian(name, power, master, village);
			GuardianNode* newGuardianNode = new GuardianNode(guardian);
			
			return newGuardianNode;
		}
		
		void PrintFullList();
		
		void PrintJustName(GuardianNode* root)
		{
			GuardianNode* current = root;
			cout << "\n\n\tLista de guardianes:\n" << endl;
			int count = 1;
			
			while(current != nullptr)
			{
				cout << "\t- Guardian " << count << ": " << current->guardian_->GetName() << endl;
				count++;
				current = current->next_;
			}
		}
		Guardian* SearchGuardian(GuardianNode* root, string name)
		{
			GuardianNode* current = root;
		
			while(current != nullptr)
			{
				if(current->guardian_->GetName() == name)
				{
					return current->guardian_;
				}
				
				current = current->next_;
			}
			
			return nullptr;
		}
		
	private:
		
		//Atributos
		Guardian* guardian_;
		GuardianNode* next_;
};

//Nodo aldea (Para la lista de aldeas)
class VillageNode
{
	public:
		
		//Constructores
		VillageNode() //Constructor por defecto
		{
			village_ = nullptr;
			next_ = nullptr;
		}
		
		VillageNode(Village* village, VillageNode* next) //Aldea, siguiente nodo
		{
			village_ = village;
			next_ = next;
		}
		
		//Destructor
		~VillageNode()
		{
			
		}
		
		//Metodos
		void AddVillageNode(VillageNode** root, string villageName, string adjacentVillageName, int* countVillages) //Agregar un nuevo nodo
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
				
				while(current->next_ != nullptr && firstVillageNodeDoesntExist)
				{
					if(current->village_->GetName() == villageName) //La primera aldea ya existe en la lista
					{
						firstVillageNodeDoesntExist = false;
						current->village_->AddAdjacentVillageName(adjacentVillageName); //Solo se agrega la aldea adyacente de la primera aldea
					}
					
					current = current->next_;
				}
				
				if(firstVillageNodeDoesntExist && current->village_->GetName() != villageName) //Si la primera aldea no existe en la lista, se agrega
				{
					current->next_ = CreateVillageNode(villageName, adjacentVillageName);
					*countVillages += 1; //Incremento del contador de aldeas
				}
				else if(firstVillageNodeDoesntExist) //La aldea es el ultimo elemento de la lista
				{
					current->village_->AddAdjacentVillageName(adjacentVillageName);
					*countVillages += 1;
				}				
				
				current = *root;
				
				while(current->next_ != nullptr && secondVillageNodeDoesntExist)
				{
					if(current->village_->GetName() == adjacentVillageName) //La segunda aldea ya existe en la lista
					{
						secondVillageNodeDoesntExist = false;
						current->village_->AddAdjacentVillageName(villageName); //Solo se agrega la aldea adyacente de la segunda aldea
					}
					
					current = current->next_;
				}
				
				if(secondVillageNodeDoesntExist && current->village_->GetName() != adjacentVillageName) //Si la segunda aldea no existe en la lista, se agrega
				{
					current->next_ = CreateVillageNode(adjacentVillageName, villageName);
					*countVillages += 1;
				}
				else if(secondVillageNodeDoesntExist) //La aldea es el ultimo elemento de la lista
				{
					current->village_->AddAdjacentVillageName(villageName);
				}
			}
		}
		
		void PrintFullList(VillageNode* root) //Imprimir la lista de aldeas
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
		
		void PrintJustNames(VillageNode* root) //Imprimir los nombres de las aldeas
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
		
		bool SearchVillage(VillageNode* root, string villageName) //Buscar aldea
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
		
		//Getters
		Village* GetVillage() //Aldea
		{
			return village_;
		}
		
		VillageNode* GetNext() //Siguiente nodo
		{
			return next_;
		}
		
	private:
		
		//Atributos
		Village* village_;
		VillageNode* next_;
		
		//Metodos
		VillageNode* CreateVillageNode(string villageName, string adjacentVillageName) //Crear un nuevo nodo
		{
			Village* village = new Village(villageName, adjacentVillageName); //Creacion de una nueva aldea
			VillageNode* newVillageNode = new VillageNode(village, nullptr); //Creacion de un nuevo elemento aldea
			
			return newVillageNode;
		}
};

//Funciones

bool VillagesFileSameVillageValidation(string, string); //Validacion para saber si la aldea es adyacente consigo misma
void VillagesFileCountValidation(int, bool*); //Validacion de la cantidad de aldeas
void VillagesFileMainVillageValidation(int, bool*, VillageNode*); //Validacion de la existencia de la aldea principal
void VillagesFileConnectionsValidation(VillageNode*, bool*); //Validacion de la cantidad minima de conexiones
void ReadVillagesFileInLoopValidations(bool*, VillageNode**, string, string, int*, int, string); //Validaciones en el loop de la lectura de aldeas
void ReadVillagesFilePostLoopValidations(bool, int, bool*, VillageNode*); //Validaciones post ciclo de lectura de aldeas
void ReadVillagesFile(VillageNode**, bool*); //Leer archivo de aldeas

//VERIFICAR
//void ReadGuardiansFileInLoopValidations();
//void ReadGuardiansFilePostLoopValidations();

void ReadGuardiansFile(GuardianNode**, VillageNode*, bool*); //Leer archivo de guardianes
void FilesInfo(); //Informacion sobre el contenido de los archivos en caso de falla en la lectura
void ReadFiles(GuardianNode**, VillageNode**, bool*); //Leer archivos
//void PrintGuardianQueue(queue<Guardian> guardians); //Imprimir cola de guardianes
//void PrintVillagesQueue(queue<Village> villages); //Imprimir cola de aldeas

int main()
{
	GuardianNode* guardians = nullptr;
	VillageNode* villages = nullptr;
	bool successRead = true;
	
	ReadFiles(&guardians, &villages, &successRead);
	
//successRead = true;
	
	if(successRead)
	{
		guardians->PrintJustName(guardians);
		//villages->PrintFullList(villages);
		//villages->PrintJustNames(villages);
	}
	
	return 0;
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
		
		*successRead = false; //Cantidad de aldeas incorrecta
	}
}

//Validacion de la existencia de la aldea principal
void VillagesFileMainVillageValidation(int count, bool* successRead, VillageNode* villagesList)
{
	bool mainVillageFound = villagesList->SearchVillage(villagesList, mainVillage); //Se busca la aldea principal en la lista de aldeas
	*successRead = mainVillageFound; //El exito de la validacion depende de haber encontrado Tesla en la lista
	
	if(count >= minVillages && !mainVillageFound)
	{
		cout << "\n\t* La cantidad de aldeas es correcta, pero la aldea principal (" << mainVillage << ") no existe *" << endl;
	}
}

//Validaciones del minimo de conexiones de cada aldea
void VillagesFileConnectionsValidation(VillageNode* root, bool* success)
{
	VillageNode* current = root;
	
	while(current != nullptr && *success)
	{
		Village* village = current->GetVillage();
		
		if(village != nullptr && village->GetNumAdjacentVillages() < minAdjacentVillages)
		{
			cout << "\n\t* Las aldeas deben tener " << minAdjacentVillages << " conexiones como minimo (Verifique que " << village->GetName() << " cumpla con el requisito) *" << endl;
			*success = false;
			return;
		}
		
		current = current->GetNext();
	}
}

//Validaciones mientras se ejecuta el ciclo de lectura del archivo de aldeas
void ReadVillagesFileInLoopValidations(bool* validation, VillageNode** villagesList, string name, string adjacentVillage, int* countVillages, int countLines, string filename)
{
	*validation = VillagesFileSameVillageValidation(name, adjacentVillage); //La aldea y su adyacente no son iguales
	VillageNode* tempList = *villagesList;

	if(*validation) //Si se supero la validacion, se agregan las aldeas a la lista
	{		
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
		VillagesFileCountValidation(countVillages, successRead); //Validacion de la cantidad de aldeas
		VillagesFileMainVillageValidation(countVillages, successRead, villagesList); //Validacion de la existencia de la aldea principal
		VillagesFileConnectionsValidation(villagesList, successRead); //Validacion del minimo de conexiones
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
		
		ReadVillagesFilePostLoopValidations(validation, countVillages, successRead, *villagesList); //Validaciones post ciclo
		
		file.close();
	}
	else
	{
		cout << "\n\tError al intentar abrir el archivo: " << filename << endl;
		*successRead = false; //El archivo no se leyo con exito
	}
}

/*void ReadGuardiansFileInLoopValidations(string name, int power, string master, string village)
{
	
}

void ReadGuardiansFilePostLoopValidations()
{
	
}*/

//Leer archivo de guardianes
void ReadGuardiansFile(GuardianNode** guardians, VillageNode* villages, bool* successRead)
{
	string filename = "Guardians.txt";
	ifstream file(filename);
	
	if(file.is_open())
	{
		string line;
		getline(file, line); //Omitir la primera linea correspondiente a los encabezados
		int countLines = 1;
		bool validation = true;
		GuardianNode* tempGuardians = *guardians;
		
		while(getline(file, line) && validation)
		{
			countLines++;
			stringstream ss(line);
			string name, power, master, village;
			
			getline(ss, name, ',');
			getline(ss, power, ',');
			getline(ss, master, ',');
			getline(ss, village, ',');
			
			//ReadGuardiansFileInLoopValidations(name, stoi(power), master, village);
			//AGREGAR FOR LOOP DE VALIDACIONES
			
			//Validaciones maestro principal
			if(countLines == 2) //Validacion 1: No tiene maestro
			{
				if(master != "")
				{
					cout << "\n\t* El maestro principal no debe tener maestro (Linea " << countLines << " en el archivo " << filename << ", " << master << " no puede ser maestro de " << name << " ya que es el maestro principal) *" <<  endl;
					*successRead = false;
					validation = false;
				}
				
				if(*successRead) //Validacion 2: El poder no pasa del maximo
				{
					if(stoi(power) > maxPower)
					{
						cout << "\n\t* El poder de un guardian no puede ser mayor a " << maxPower << " (Linea " << countLines << " en el archivo " << filename << ", " << name << " tiene " << power << " puntos de poder) *" << endl;
						*successRead = false;
						validation = false;
					}
				}
				
				if(*successRead) //Validacion 3: El poder no pasa del minimo
				{
					if(stoi(power) < minPowerMainMaster)
					{
						cout << "\n\t* El poder del maestro principal no puede ser menor a " << minPowerMainMaster << " (Linea " << countLines << " en el archivo " << filename << ", " << name << " tiene " << power << " puntos de poder) *" << endl;
						*successRead = false;
						validation = false;
					}
				}
				
				if(*successRead) //Validacion 4: La aldea principal es la ciudad del maestro principal
				{
					if(village != mainVillage)
					{
						cout << "\n\t* El maestro principal debe estar en " << mainVillage << " (Linea " << countLines << " en el archivo " << filename << ", la aldea del maestro principal es " << village << ") *" <<  endl;
						*successRead = false;
						validation = false;
					}
				}
				
				if(*successRead)
				{
					tempGuardians->AddGuardianNode(guardians, name, stoi(power), master, village);
				}
			}
			//Validaciones del resto de guardianes
			else
			{
				Guardian* myMaster = tempGuardians->SearchGuardian(*guardians, master);
				bool villageFound = villages->SearchVillage(villages, village);
				Guardian* nameExist = tempGuardians->SearchGuardian(*guardians, name);
				
				if(nameExist != nullptr) //Validacion 1: El nombre ya existe
				{
					cout << "\n\t* El nombre \"" << name << "\" ya esta en uso, modifiquelo e intente nuevamente (Linea " << countLines << " en el archivo " << filename << ", el nombre del guardian ya esta en uso) *" << endl; 
					validation = false;
					*successRead = false;
				}
				
				if(*successRead && myMaster == nullptr) //Validacion 2: El maestro existe
				{
					cout << "\n\t* El maestro de " << name << " (" << master << ") no existe (Linea " << countLines << " en el archivo " << filename << ") *" << endl;
					validation= false;
					*successRead = false;
				}
				
				if(*successRead && myMaster->GetPowerLevel() == minPower) //Validacion 3: El maestro tiene el puntaje minimo de poder
				{
					cout << "\n\t* El maestro de " << name << " (" << master << ") tiene la cantidad minima de puntos de poder (" << minPower << "). Verifique e intente de nuevo (Linea " << countLines << " en el archivo " << filename << ") *" << endl;
					validation= false;
					*successRead = false;
				}
				
				if(*successRead && myMaster->GetPowerLevel() <= stoi(power)) //Validacion 4: Los puntos del aprendiz no pueden superar los del maestro
				{
					cout << "\n\t* El nivel de poder de un aprendiz no puede ser mayor o igual que el de su maestro (Linea " << countLines << " en el archivo " << filename << ", el poder de " << name << " (" << power << ") es mayor o igual que el de su maestro " << myMaster->GetName() << "(" << myMaster->GetPowerLevel() << ")) *" << endl;
					validation= false;
					*successRead = false;
				}
			
				if(*successRead) //Validacion 5: La aldea no puede ser la aldea principal
				{
					if(village == mainVillage)
					{
						cout << "\n\t* Un guardian que no sea el maestro principal no puede estar en la aldea principal (Linea " << countLines << " en el archivo " << filename << ", " << name << " esta en la aldea principal (" << mainVillage << ")) *" << endl;
						validation = false;
						*successRead = false;
					}
				}
				
				if(*successRead) //Validacion 6: La aldea debe existir
				{
					if(!villageFound)
					{
						cout << "\n\t* La aldea de " << name << " (" << village << ") no existe (Linea " << countLines << " en el archivo " << filename << ", verifique e intente nuevamente) *" << endl;
						validation = false;
						*successRead = false;
					}
				}
				
				if(*successRead) //Validacion 7: Puntos minimo de poder
				{
					if(stoi(power) < minPower)
					{
						cout << "\n\t* El minimo de puntos de poder es " << minPower << " (Linea " << countLines << " en el arhivo " << filename << ", " << name << " tiene " << power << " puntos de poder) *" << endl;
						validation = false;
						*successRead = false;
					}
				}
				
				if(*successRead)
				{
					tempGuardians->AddGuardianNode(guardians, name, stoi(power), master, village);
				}
			}
			
			//tempGuardians->PrintJustName(*guardians);
		}
		
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
	cout << "\t- Archivo de aldeas:\n\n\t\t1. La cantidad minima de aldeas es " << minVillages << "\n\t\t2. Una aldea no puede ser adyacente consigo misma\n\t\t3. " << mainVillage << " debe estar en el archivo, ya que es la ciudad principal\n\t\t4. Cada aldea debe tener 2 conexiones como minimo" << endl;
	cout << "\n\t- Archivo de guardianes:\n\n\t\t1. La aldea de los guardianes debe existir, es decir, debe estar en el archivo de aldeas\n\t\t2. Una aldea debe tener, como minimo, 1 maestro y 1 aprendiz\n\t\t3. El maestro principal debe estar en la segunda linea del archivo (justo despues de los encabezados)\n\t\t4. El maestro principal no debe tener maestro y debe estar en " << mainVillage << endl;
	cout << "\n\t* Observacion: Es probable que se haya mostrado un mensaje mas arriba sobre el error" << endl;
}

//Leer archivos
void ReadFiles(GuardianNode** guardians, VillageNode** villagesList, bool* successRead)
{
	cout << "\tCargando archivos..." << endl;
	ReadVillagesFile(villagesList, successRead); //Archivo de aldeas
	
	if(*successRead) //Si no se leyo bien el primer archivo, no es necesario leer el segundo
	{
//cout << "\n\tDEBUG Lectura segundo archivo";
		ReadGuardiansFile(guardians, *villagesList, successRead); //Archivo de guardianes
	}
	
	if(*successRead)
	{
		cout << "\n\t* Archivos cargados exitosamente *" << endl;
	}
	else
	{
		cout << "\n\t* Algo salio mal. Por favor, corrobora que los archivos contengan lo siguiente y ejecuta nuevamente el programa *" << endl;
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
