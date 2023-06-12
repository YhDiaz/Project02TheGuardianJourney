#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<queue>
#include<limits>
#include <unordered_map>
#include <string>
#include <ctime>
using namespace std;

/*Notas

	Aprendiz preparado
	Verificar que la aldea a la que se quiere mover no sea Tesla
	Si se quiere mover a tesla, debe ser aprendiz preparado
	Agregar busqueda de caminos entre cada aldea y Tesla para verificar que se puede llegar desde el inicio y que no hay subgrafos
	Historiales
	Opcion 3 en gameloop: Alquimia, crear camino entre aldeas sacrificando puntos de poder
		verificar que la cantidad de puntos de poder sea minimo 4 ya que es el maximo de poder que se puede emplear en alquimia

*/

//Variables globales
int minVillages = 3; //Minimo de aldeas
int minAdjacentVillages = 2; //Minimo de conexiones entre aldeas
int maxPower = 100; //Poder maximo de un guardian
int minPower = maxPower / 2; //Poder minimo de un guardian
int minPowerMainMaster = (maxPower / 10) * 9; //Poder minimo del maestro principal
string mainVillage = "Tesla"; //Aldea principal
int minGuardiansPerVillage = 2; //Minimo de guardianes por aldea
int maxPointsInARow = 4; //Puntaje maximo por aldea

//Clases

//Player
class Player
{
	public:
		
		//Constructores
		Player(string name, int power, string home)
		{
			name_ = name;
			power_ = power;
			homeVillage_ = home;
			currentVillage_ = home;
			pointsInARow_ = 0;
			preparedApprentice_ = false;
		}
		
		//Metodos
		void IncrementPower(int increment)
		{
			if(power_ + increment >= maxPower) //No se puede pasar del poder maximo
			{
				power_ = maxPower;
			}
			else
			{
				power_ += increment;
			}			
		}
		
		void PrintData(bool powerComparison, int lastPower)
		{
			cout << "\n\t--------------- DATOS DEL JUGADOR ---------------" << endl;
			cout << "\n\t* Nombre: " << name_;
			
			if(powerComparison)
			{
				cout << "\n\t* Puntos de poder (al llegar): " << lastPower;
			}
			
			cout << "\n\t* Puntos de poder (actual): " << power_ << "\n\t* Aldea de origen: " << homeVillage_ << "\n\t* Aldea actual: " << currentVillage_ << endl;
		}
		
		void IncrementPointsInARow(int points, bool* maxReached)
		{
			if(pointsInARow_ + points >= maxPointsInARow)
			{
				pointsInARow_ = maxPointsInARow;
				*maxReached = true; //Se alcanzo el maximo de puntos seguidos
			}
			else
			{
				pointsInARow_ += points;
			}
		}
		
		void RestartPointsInARow()
		{
			pointsInARow_ = 1;
			power_++;
		}
		
		//Setters
		void SetCurrentVillage(string name)
		{
			currentVillage_ = name;
		}
		
		//Getters
		string GetName()
		{
			return name_;
		}
		
		int GetPower()
		{
			return power_;
		}
		
		string GetHomeVillage()
		{
			return homeVillage_;
		}
		
		string GetCurrentVillage()
		{
			return currentVillage_;
		}
		
		int GetPointsInARow()
		{
			return pointsInARow_;
		}
		
	private:
		
		//Atributos
		string name_;
		int power_;
		int pointsInARow_;
		string homeVillage_;
		string currentVillage_;
		bool preparedApprentice_;
		//AGREGAR HISTORIALES
};

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
			numApprentices_ = 0;
		}
		
		//Destructor
		~Guardian()
		{
			
		}
		
		//Metodos
		void AddApprentice(Guardian* apprentice) //Agregar aprendiz
		{
			apprentices_.push_back(apprentice);
			numApprentices_++;
		}
		
		void PrintData(bool name, bool power, bool master, bool village, bool apprentices)
		{
			cout << "\n\t--------------- DATOS DEL GUARDIAN ---------------" << endl;
			
			if(name)
			{
				cout << "\n\t* Nombre: " << name_;
			}
			
			if(power)
			{
				cout << "\n\t* Nivel de poder: " << powerLevel_;
			}
			
			if(master)
			{
				if(mainMasterName_ != "")
				{
					cout << "\n\t* Maestro: " << mainMasterName_;
				}
				else
				{
					cout << "\n\t* Maestro: No tiene, es el maestro supremo" << mainMasterName_;
				}
			}
			
			if(village)
			{
				cout << "\n\t* Aldea: " << village_;
			}
			
			if(apprentices)
			{
				cout << "\n\t* Aprendices: ";
				
				if(numApprentices_ > 0)
				{
					cout << endl;
					
					for(const auto& apprentice : apprentices_)
					{
						cout << "\n\t\t- " << apprentice->GetName();
					}
				}
				else
				{
					cout << "No tiene aprendices";
				}
			}			
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
		int numApprentices_;
		vector<Guardian*> apprentices_;
};

//Aldea
class Village
{
	public:
		
		//Constructores
		Village() //Constructor por defecto
		{
			name_ = "";
			numGuardians_ = 0;
			numAdjacentVillages_ = 0;
			master_ = nullptr;
			maxPointsInARowReached_= false;
		}
		
		Village(string name, string adjacentVillageName)
		{
			name_ = name;
			adjacentVillagesNames_.push_back(adjacentVillageName);
			numAdjacentVillages_ = 1;
			numGuardians_ = 0;
			master_ = nullptr;
			maxPointsInARowReached_ = false;
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
			numAdjacentVillages_++; //Incremento en el contador de aldeas adyacentes
		}
		
		void PrintGuardiansNames()
		{
			int i = 1;
			
			for(const auto& guardian : guardians_)
			{
				if(guardian == master_)
				{
					cout << "\n\t- Guardian " << i << ": " << guardian->GetName() << " (Maestro de la aldea)";
				}
				else
				{
					cout << "\n\t- Guardian " << i << ": " << guardian->GetName();
				}

				i++;
			}
		}
		
		void AddGuardian(Guardian* guardian)
		{
			if(master_ != nullptr)
			{
				if(guardian->GetPowerLevel() > master_->GetPowerLevel())
				{
					master_ = guardian;
				}
			}
			else
			{
				master_ = guardian;
			}
			
			guardians_.push_back(guardian);
		}
		
		void IncrementNumOfGuardians()
		{
			numGuardians_++;
		}
		
		Guardian* SearchGuardianByIndex(int index)
		{
			int count = 1;
			
			for(const auto& guardian : guardians_)
			{
				if(index == count)
				{
					return guardian;
				}
				
				count++;
			}
		}
		
		string SearchVillageByIndex(int index)
		{
			int count = 1;
			
			for(const auto& village : adjacentVillagesNames_)
			{
				if(index == count)
				{
					return village;
				}
				
				count++;
			}
		}
		
		void MaxPointsInARowReached()
		{
			maxPointsInARowReached_ = true;
		}
		
		//Setters
		void SetName(string name)
		{
			name_ = name;
		}
		
		//Getters
		string GetName()
		{
			return name_;
		}
		
		vector<string> GetAdjacentVillagesNames()
		{
			return adjacentVillagesNames_;
		}
		
		vector<Guardian*> GetGuardians()
		{
			return guardians_;
		}
		
		int GetNumGuardians()
		{
			return numGuardians_;
		}
		
		int GetNumAdjacentVillages()
		{
			return numAdjacentVillages_;
		}
		
		Guardian* GetMainMaster()
		{			
			return master_;
		}
		
		bool GetPointsInARowReached()
		{
			return maxPointsInARowReached_;
		}
	
	//Atributos
	private:
		
		string name_;
		vector<string> adjacentVillagesNames_;
		vector<Guardian*> guardians_;
		Guardian* master_;
		int numGuardians_;
		int numAdjacentVillages_;
		bool maxPointsInARowReached_;
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
		
		//Getters
		Guardian* GetLastElement(GuardianNode* root)
		{
			GuardianNode* current = root;
			
			while(current->next_ != nullptr)
			{
				current = current->next_;
			}
			
			return current->guardian_;
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
		
		/*void ReplaceVillage(Village* replacement, VillageNode** villages)
		{
			VillageNode* current = *villages;
			
			while(current != nullptr)
			{
				
			}
		}*/
		
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
		
		Village* SearchVillage(VillageNode* root, string villageName) //Buscar aldea
		{
			VillageNode* current = root;
		
			while(current != nullptr)
			{
				if(current->village_->GetName() == villageName)
				{
					return current->village_; //Se encontro la aldea
				}
				
				current = current->next_;
			}
			
			return nullptr; //Salir del bucle indica que no se encontro la aldea
		}
		
		Village* SearchVillageByNum(VillageNode* root, int num)
		{
			if(num > 0)
			{
				VillageNode* current = root;
				int count = 0;
				
				while(current != nullptr)
				{
					count++;
					
					if(count == num) //Aldea encontrada
					{
						return current->village_;
					}
					
					current = current->next_;
				}
			}	
			
			return nullptr; //La aldea no fue encontrada o el numero no estaba dentro del rango		
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

//Grafo: Aldeas
class Graph
{
	public:
		
		//Constructores
		Graph()
		{
			size_ = 0;
		}
		
		//Destructor
		~Graph()
		{
			
		}
	
		//Metodos
		void CreateMaps(VillageNode* root) //Crear mapas
		{
			VillageNode* current = root;
	
			while(current != nullptr) //Mientras existan aldeas en la lista
			{
				string name = current->GetVillage()->GetName(); //Nombre de la aldea
				
				mapVillageName_Index_[name] = size_;
				mapIndex_VillageName_[size_] = name;
				
				size_++;
				current = current->GetNext();
			}
		}
		
		void CreateMatrix() //Crear matriz de adyacencia
		{
			graph_ = vector<vector<int>>(size_, vector<int>(size_, 0));
		}
		
		void CreateConnections(VillageNode* villages) //Crear conexiones
		{
			for(int i = 0; i < size_; i++)
			{
				string name = mapIndex_VillageName_[i]; //Nombre de la aldea "i" en el mapa
				Village* currentVillage = villages->SearchVillage(villages, name); //Busqueda de la aldea en la lista, a traves del nombre
				vector<string> adjacentVillages = currentVillage->GetAdjacentVillagesNames(); //Obtencion del vector de aldeas adyacentes de la aldea "i"
				cout << "\n\tAdyacentes de " << name << ":\n" << endl;
				
				for(int j = 0; j < currentVillage->GetNumAdjacentVillages(); j++)
				{
					string adjacentName = adjacentVillages[j]; //Nombre de la aldea adyacente "j" en el vector de aldeas adyacentes
					int k = mapVillageName_Index_[adjacentName]; //Indice de la aldea "j" en el mapa
					AddEdge(i, k); //Se crea la conexion entre la aldea "i" y la aldea "j" en el grafo
					cout << "\t\t- " << adjacentName << endl;
					
				}
			}
		}
		
		void AddEdge(int village, int adjacent) //Agregar arista
		{
			graph_[village][adjacent] = 1;
			graph_[adjacent][village] = 1;
		}
		
		void PrintGraph() //Mostrar el grafo (matriz)
		{
			cout << "\n* Matriz de adyacencia:\n" << endl;
				
			for(const auto& row : graph_)
			{
				cout << "\t";
				
				for(int val : row)
				{
					cout << val << " ";						
				}
					
				cout << endl;
			}
		}
		
		bool AreAdjacent(int village, int adjacent) //Aldeas adyacentes
		{
			if(graph_[village][adjacent] == 1)
			{
				return true; //Las aldeas estan conectadas
			}
			
			return false; //Las aldeas no estan conectadas
		}
	
	private:
		
		//Atributos
		vector<vector<int>> graph_;
		int size_; //Cantidad de aldeas
		unordered_map<string, int> mapVillageName_Index_; //Mapa: Nombre de aldeas --- Indice en la matriz de adyacencia
		unordered_map<int, string> mapIndex_VillageName_; //Mapa: Indice en la matriz de adyacencia --- Nombre de aldeas
};

//Funciones

bool VillagesFileSameVillageValidation(string, string); //Validacion para saber si la aldea es adyacente consigo misma
void VillagesFileCountValidation(int, bool*); //Validacion de la cantidad de aldeas
void VillagesFileMainVillageValidation(int, bool*, VillageNode*); //Validacion de la existencia de la aldea principal
void VillagesFileConnectionsValidation(VillageNode*, bool*); //Validacion de la cantidad minima de conexiones
void ReadVillagesFileInLoopValidations(bool*, VillageNode**, string, string, int*, int, string); //Validaciones en el loop de la lectura de aldeas
void ReadVillagesFilePostLoopValidations(bool, int, bool*, VillageNode*); //Validaciones post ciclo de lectura de aldeas
void ReadVillagesFile(VillageNode**, bool*); //Leer archivo de aldeas
void MainMasterNoMasterValidation(bool*, string, int, string, string); //Validacion 1 (Maestro principal): No debe tener maestro
void MainMasterPowerRangeValidation(bool*, int, int, string, string); //Validacion 2 (Maestro principal): Poder dentro del rango
void MainMasterInMainVillageValidation(bool*, string, int, string); //Validacion 3 (Maestro principal): Debe estar en la aldea principal
void ReadGuardiansFileMainMasterValidations(int, string, string, int, string, string, bool*, GuardianNode**, VillageNode**); //Validaciones del maestro principal
void GuardiansNameExistsValidation(bool*, Guardian*, string, int, string); //Validacion 1 (Guardianes): El nombre ya existe
void GuardiansMastersExistenceValidation(bool*, Guardian*, string, string, int, string); //Validacion 2 (Guardianes): El maestro no existe
void GuardiansMasterPowerValidation(bool*, Guardian*, string, string, int, string); //Validacion 3 (Guardianes): El maestro tiene el puntaje minimo de poder
void GuardiansPowerComparisonValidation(bool*, Guardian*, int, int, string, string); //Validacion 4 (Guardianes): Los puntos del aprendiz no pueden superar los del maestro
void GuardiansMainVillageValidation(bool*, string, int, string, string); //Validacion 5 (Guardianes): La aldea no puede ser la aldea principal
void GuardiansVillagesExistenceValidation(bool*, Village*, string, string, int, string); //Validacion 6 (Guardianes): Existencia de la aldea
void GuardiansMinPowerValidation(bool*, int, int, string, string); //Validacion 7 (Guardianes): Minimo de puntos de poder
void ReadGuardiansFileGuardiansValidations(bool*, Guardian*, string, int, string, Guardian*, string, int, string, Village*); //Validaciones del resto de guardianes
void ReadGuardiansFileInLoopValidations(int, string, string, int, string, string, bool*, GuardianNode**, VillageNode**); //Validaciones durante el ciclo de lectura del archivo de guardianes
void GuardiansMasterAndApprenticeValidation(bool*, VillageNode*); //Validacion: Minimo 1 maestro y su aprendiz por aldea
void ReadGuardiansFilePostLoopValidations(VillageNode**, bool*); //Validaciones post ciclo de lectura del archivo de guardianes
void ReadGuardiansFile(GuardianNode**, VillageNode*, bool*); //Leer archivo de guardianes
void FilesInfo(); //Informacion sobre el contenido de los archivos en caso de falla en la lectura
void ReadFiles(GuardianNode**, VillageNode**, bool*); //Leer archivos
void MainMenuSelection(int*, bool*); //Menu de seleccion
void MainMenuPostSelection(int, bool*, bool*); //Menu post seleccion

//FALTA AGREGAR ESTA FUNCIONALIDAD Y SUS RELACIONADOS
	void SelectPlayer(GuardianNode**, VillageNode**, Player**); //Seleccionar guardian como jugador
//HASTA AQUI

void CreatePlayerAssignData(Player**, Guardian*); //Asignacion de datos al crear un jugador
void CreatePlayerNameWarning(bool*, Player**, Guardian*, bool*); //Advertencia al crear un jugador: Hay un guardian con el mismo nombre
void CreatePlayerNameVerification(string*, Player**, GuardianNode**); //Creacion del jugador: Entrada y verificacion del nombre
void CreatePlayerVillageVerification(VillageNode**, string*); //Creacion del jugador: Entrada y verificacion de la aldea
void CreatePlayer(GuardianNode**, VillageNode**, Player**); //Crear jugador
void MainMenu(GuardianNode**, VillageNode**, Player**, bool*); //Menu de inicio
void InitializeGame(VillageNode**); //Inicializacion del juego: Creacion del grafo de aldeas
void GameMenu(int*); //Menu del juego
void TrainingModeGetEasiestGuardian(vector<Guardian*>, Guardian**); //Obtener el guardian mas facil de enfrentar en el modo de entrenamiento
int TrainingModeGetWinProbability(Guardian*, Player*); //Obtener la probabilidad de victoria en un enfrentamiento
string TrainingModeGetDifficulty(int); //Obtener la dificultad de un enfrentamiento
int TrainingModeOpponentSelection(); //Menu de accion al momento de seleccionar un oponente
void TrainingModePrintGuardians(Village*, Player*, Guardian*, int*, Guardian**); //Imprimir los guardianes a los que se puede enfrentar el player
void TrainingModeDiceRoll(vector<int>*, int); //Lanzamiento de dados para saber el resultado del enfrentamiento
void TrainingModeUpdatePlayer(Player**, int, Village*); //Actualizacion de los datos del jugador
void TrainingModeShowdownResult(vector<int>, Guardian*, Player**, bool); //Resultado del enfrentamiento
void TrainingModeGuardiansShowdown(Guardian*, Player**, bool); //Enfrentamiento de guardianes
void TrainingMode(Village*, Player**); //Modo de entrenamiento
void TravelToVillage(Village*, VillageNode*, Player**); //Viajar a otra aldea
void GameLoop(GuardianNode**, VillageNode**, Player**); //Loop del juego

int main()
{
	GuardianNode* guardians = nullptr;
	VillageNode* villages = nullptr;
	Player* player = nullptr;
	bool successRead = true, endGame = false;
	
	ReadFiles(&guardians, &villages, &successRead); //Lectura de archivos
	
	if(successRead) //Lectura de archivos correcta
	{
		//guardians->PrintJustName(guardians);
		//villages->PrintFullList(villages);
		//villages->PrintJustNames(villages);
		
		MainMenu(&guardians, &villages, &player, &endGame); //Menu principal del juego
		
		if(!endGame) //El usuario no eligio cerrar el juego
		{
			GameLoop(&guardians, &villages, &player); //Loop del juego
		}	
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

//TRABAJANDO AQUI

//Validacion 1 (Maestro principal): No debe tener maestro
void MainMasterNoMasterValidation(bool* successRead, string master, int countLines, string filename, string name)
{
	if(master != "")
	{
		cout << "\n\t* El maestro principal no debe tener maestro (Linea " << countLines << " en el archivo " << filename << ", " << master << " no puede ser maestro de " << name << " ya que es el maestro principal) *" <<  endl;
		*successRead = false;
	}
}

//Validacion 2 (Maestro principal): Poder dentro del rango
void MainMasterPowerRangeValidation(bool* successRead, int power, int countLines, string filename, string name)
{
	if(*successRead) //El poder no pasa del maximo
	{
		if(power > maxPower)
		{
			cout << "\n\t* El poder de un guardian no puede ser mayor a " << maxPower << " (Linea " << countLines << " en el archivo " << filename << ", " << name << " tiene " << power << " puntos de poder) *" << endl;
			*successRead = false;
		}
	}
	
	if(*successRead) //El poder no pasa del minimo
	{
		if(power < minPowerMainMaster)
		{
			cout << "\n\t* El poder del maestro principal no puede ser menor a " << minPowerMainMaster << " (Linea " << countLines << " en el archivo " << filename << ", " << name << " tiene " << power << " puntos de poder) *" << endl;
			*successRead = false;
		}
	}
}

//Validacion 3 (Maestro principal): Debe estar en la aldea principal
void MainMasterInMainVillageValidation(bool* successRead, string village, int countLines, string filename)
{
	if(*successRead) //La aldea principal es la ciudad del maestro principal
	{
		if(village != mainVillage)
		{
			cout << "\n\t* El maestro principal debe estar en " << mainVillage << " (Linea " << countLines << " en el archivo " << filename << ", la aldea del maestro principal es " << village << ") *" <<  endl;
			*successRead = false;
		}
	}
}

//Validaciones del maestro principal
void ReadGuardiansFileMainMasterValidations(int countLines, string filename, string name, int power, string master, string village, bool* successRead, GuardianNode** guardians, VillageNode** villages)
{
	MainMasterNoMasterValidation(successRead, master, countLines, filename, name); //Validacion 1: El maestro principal no debe tener maestro
	MainMasterPowerRangeValidation(successRead, power, countLines, filename, name); //Validacion 2: Rango de poder
	MainMasterInMainVillageValidation(successRead, village, countLines, filename); //Validacion 3: Esta en la aldea principal
}

//Validacion 1 (Guardianes): El nombre ya existe
void GuardiansNameExistsValidation(bool* successRead, Guardian* nameExist, string name, int countLines, string filename)
{
	if(nameExist != nullptr)
	{
		cout << "\n\t* El nombre \"" << name << "\" ya esta en uso, modifiquelo e intente nuevamente (Linea " << countLines << " en el archivo " << filename << ", el nombre del guardian ya esta en uso) *" << endl; 
		*successRead = false;
	}
}

//Validacion 2 (Guardianes): El maestro no existe
void GuardiansMastersExistenceValidation(bool* successRead, Guardian* myMaster, string name, string master, int countLines, string filename)
{
	if(*successRead && myMaster == nullptr)
	{
		cout << "\n\t* El maestro de " << name << " (" << master << ") no existe (Linea " << countLines << " en el archivo " << filename << ") *" << endl;
		*successRead = false;
	}
}

//Validacion 3 (Guardianes): El maestro tiene el puntaje minimo de poder
void GuardiansMasterPowerValidation(bool* successRead, Guardian* myMaster, string name, string master, int countLines, string filename)
{
	if(*successRead && myMaster->GetPowerLevel() == minPower)
	{
		cout << "\n\t* El maestro de " << name << " (" << master << ") tiene la cantidad minima de puntos de poder (" << minPower << "). Verifique e intente de nuevo (Linea " << countLines << " en el archivo " << filename << ") *" << endl;
		*successRead = false;
	}
}

//Validacion 4 (Guardianes): Los puntos del aprendiz no pueden superar los del maestro
void GuardiansPowerComparisonValidation(bool* successRead, Guardian* myMaster, int power, int countLines, string filename, string name)
{
	if(*successRead && myMaster->GetPowerLevel() <= power)
	{
		cout << "\n\t* El nivel de poder de un aprendiz no puede ser mayor o igual que el de su maestro (Linea " << countLines << " en el archivo " << filename << ", el poder de " << name << " (" << power << ") es mayor o igual que el de su maestro " << myMaster->GetName() << "(" << myMaster->GetPowerLevel() << ")) *" << endl;
		*successRead = false;
	}
}

//Validacion 5 (Guardianes): La aldea no puede ser la aldea principal
void GuardiansMainVillageValidation(bool* successRead, string village, int countLines, string filename, string name)
{
	if(*successRead && village == mainVillage)
	{
		cout << "\n\t* Un guardian que no sea el maestro principal no puede estar en la aldea principal (Linea " << countLines << " en el archivo " << filename << ", " << name << " esta en la aldea principal (" << mainVillage << ")) *" << endl;
		*successRead = false;
	}
}

//Validacion 6 (Guardianes): Existencia de la aldea
void GuardiansVillagesExistenceValidation(bool* successRead, Village* myVillage, string name, string village, int countLines, string filename)
{
	if(*successRead && myVillage == nullptr)
	{
		cout << "\n\t* La aldea de " << name << " (" << village << ") no existe (Linea " << countLines << " en el archivo " << filename << ", verifique e intente nuevamente) *" << endl;
		*successRead = false;
	}
}

//Validacion 7 (Guardianes): Minimo de puntos de poder
void GuardiansMinPowerValidation(bool* successRead, int power, int countLines, string filename, string name)
{
	if(*successRead && power < minPower)
	{
		cout << "\n\t* El minimo de puntos de poder es " << minPower << " (Linea " << countLines << " en el arhivo " << filename << ", " << name << " tiene " << power << " puntos de poder) *" << endl;
		*successRead = false;
	}
}

//Validaciones del resto de guardianes
void ReadGuardiansFileGuardiansValidations(bool* successRead, Guardian* nameExist, string name, int countLines, string filename, Guardian* myMaster, string master, int power, string village, Village* myVillage)
{
	GuardiansNameExistsValidation(successRead, nameExist, name, countLines, filename); //Validacion 1: El nombre ya existe
	GuardiansMastersExistenceValidation(successRead, myMaster, name, master, countLines, filename); //Validacion 2: Existencia del maestro
	GuardiansMasterPowerValidation(successRead, myMaster, name, master, countLines, filename); //Validacion 3: Poder del maestro
	GuardiansPowerComparisonValidation(successRead, myMaster, power, countLines, filename, name); //Validacion 4: Comparacion de poder entre un aprendiz y su maestro
	GuardiansMainVillageValidation(successRead, village, countLines, filename, name); //Validacion 5: El guardian no puede estar en la aldea principal
	GuardiansVillagesExistenceValidation(successRead, myVillage, name, village, countLines, filename); //Validacion 6: Existencia de la aldea
	GuardiansMinPowerValidation(successRead, power, countLines, filename, name); //Validacion 7: Minimo de puntos de poder
}

//Validaciones durante el ciclo de lectura del archivo de guardianes
void ReadGuardiansFileInLoopValidations(int countLines, string filename, string name, int power, string master, string village, bool* successRead, GuardianNode** guardians, VillageNode** villages)
{
	GuardianNode* tempGuardians = *guardians;
	VillageNode* tempVillages = *villages;
	
	if(countLines == 2) //Validaciones maestro principal
	{
		ReadGuardiansFileMainMasterValidations(countLines, filename, name, power, master, village, successRead, guardians, villages);
		
		if(*successRead) //Si todas las validaciones resultaron bien, se agrega el guardian
		{
			tempGuardians->AddGuardianNode(guardians, name, power, master, village);
		}
	}
	else //Validaciones del resto de guardianes
	{
		Guardian* myMaster = tempGuardians->SearchGuardian(*guardians, master);
		Village* myVillage = tempVillages->SearchVillage(*villages, village);
		Guardian* nameExist = tempGuardians->SearchGuardian(*guardians, name);
		
		ReadGuardiansFileGuardiansValidations(successRead, nameExist, name, countLines, filename, myMaster, master, power, village, myVillage);
		
		if(*successRead) //Todas las validaciones resultaron bien
		{
			tempGuardians->AddGuardianNode(guardians, name, power, master, village); //Se agrega el guardian a la lista
			
			//VillageNode*->Village*
			(*villages)->SearchVillage(*villages, village)->IncrementNumOfGuardians(); //Incremento del numero de guardianes de la aldea
			
			//VillageNode*->SearchVillage RETURN: (Village*)->AddGuardian(GuardianNode*->GetLastElement RETURN: Guardian*)
			(*villages)->SearchVillage(*villages, village)->AddGuardian((*guardians)->GetLastElement(*guardians)); //Se agrega el guardian a la lista de guardianes de la aldea
		}
	}
}

//Validacion: Minimo 1 maestro y su aprendiz por aldea
void GuardiansMasterAndApprenticeValidation(bool* successRead, VillageNode* current)
{
	//Validacion 2: Minimo 1 maestro y su aprendiz por aldea
	Guardian* villageMainMaster = current->GetVillage()->GetMainMaster();
	string masterName = villageMainMaster->GetName();
	bool masterHasAnApprentice = false;
	
	for(const auto& guardian : current->GetVillage()->GetGuardians()) //Busqueda en la lista de guardianes de la aldea
	{
		if(guardian != villageMainMaster) //Se cicla por todos los guardianes de la aldea, excepto el maestro
		{
			if(guardian->GetMainMasterName() == masterName)
			{
				masterHasAnApprentice = true;
			}
		}
	}
	
	if(!masterHasAnApprentice)
	{
		cout << "\n\t* Todas las aldeas deben tener como minimo 1 maestro y su aprendiz (El/La maestro/maestra " << masterName << " de " << current->GetVillage()->GetName() << " no tiene aprendices, modifique e intente de nuevo) *";
		*successRead = false;
	}
}

//Validaciones post ciclo de lectura del archivo de guardianes
void ReadGuardiansFilePostLoopValidations(VillageNode** villages, bool* successRead)
{
	VillageNode* current = *villages;
	
	while(current != nullptr && *successRead)
	{
		if(current->GetVillage()->GetName() != mainVillage) //La aldea no es la principal (La aldea principal es la unica que tiene 1 guardian)
		{
			if(current->GetVillage()->GetNumGuardians() >= minGuardiansPerVillage) //Validacion 1: Minimo de guardianes por aldea
			{
				GuardiansMasterAndApprenticeValidation(successRead, current); //Validacion 2: Debe haber minimo 1 maestro y su aprendiz en la aldea
			}
			else
			{
				cout << "\n\t\t" << current->GetVillage()->GetNumGuardians() << endl;
				cout << "\n\t* Una aldea debe tener como minimo " << minGuardiansPerVillage << " guardianes (Verifique la cantidad de guardianes de " << current->GetVillage()->GetName() << " e intente nuevamente) *" << endl;
				*successRead = false;
			}
		}
		
		current = current->GetNext();
	}
}

//Leer archivo de guardianes
void ReadGuardiansFile(GuardianNode** guardians, VillageNode** villages, bool* successRead)
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
		
		while(getline(file, line) && *successRead)
		{
			countLines++;
			stringstream ss(line);
			string name, power, master, village;
			
			getline(ss, name, ',');
			getline(ss, power, ',');
			getline(ss, master, ',');
			getline(ss, village, ',');
			
			ReadGuardiansFileInLoopValidations(countLines, filename, name, stoi(power), master, village, successRead, guardians, villages);
		}
		
		ReadGuardiansFilePostLoopValidations(villages, successRead);
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
		ReadGuardiansFile(guardians, villagesList, successRead); //Archivo de guardianes
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

//Menu de seleccion
void MainMenuSelection(int* option, bool* selection)
{
	cout << "\n\t\t-------------------- MENU DE SELECCION --------------------\n\n\t\t* Presiona <0> para cerrar el juego\n\t\t* Presiona <1> para seleccionar un guardian\n\t\t* Presiona <2> para crear un guardian\n\n\t\t* Ingresa tu opcion: ";
	cin >> *option;
	
	while(*option < 0 || *option > 2)
	{
		cout << "\n\t\t\t* (Error: Opcion invalida) Por favor, selecciona una opcion definida: ";
		cin >> *option;
	}
	
	*selection = false;
}

//Menu post seleccion
void MainMenuPostSelection(int option, bool* selection, bool* stopLoop)
{
	int optionPostSelection = 0;
	
	cout << "\n\t\t* Opcion seleccionada: " << option << " *\n\n\t\t* Presiona <0> para cancelar y seleccionar una opcion diferente\n\t\t* Presiona <1> para confirmar la opcion seleccionada\n\t\t* Presiona <2> para desplegar mas informacion sobre la opcion seleccionada\n\t\t  (Recomendacion antes de confirmar)\n\n\t\t* Ingresa tu opcion: ";
	cin >> optionPostSelection;
	
	while(optionPostSelection < 0 || optionPostSelection > 2)
	{
		cout << "\n\t\t\t* (Error: Opcion invalida) Por favor, selecciona una opcion definida: ";
		cin >> optionPostSelection;
	}
	
	if(optionPostSelection == 0)
	{
		*selection = true;
	}
	else if(optionPostSelection == 1)
	{
		*stopLoop = true;
	}
	else
	{
		if(option == 1)
		{
			cout << "\n\t\t------------------------- INFORMACION: SELECCION DE UN GUARDIAN -------------------------" << endl;
			cout << "\n\t\tAl seleccionar un guardian debes considerar lo siguiente:\n\n\t\t1. No podras cambiarle el nombre\n\t\t2. No podras elegir la aldea en la cual inicia\n\t\t3. Los puntos de poder se actualizaran al de aprendiz (en caso de que sea un maestro)\n\t\t4. No puedes seleccionar al maestro principal" << endl;
		}
		else if(option == 2)
		{
			cout << "\n\t\t------------------------- INFORMACION: CREACION DE UN GUARDIAN -------------------------" << endl;
			cout << "\n\t\tAl crear un guardian debes considerar lo siguiente:\n\n\t\t1. Podras definir el nombre\n\t\t2. Podras elegir la aldea en la cual inicia (excepto " << mainVillage << ")\n\t\t3. Comenzaras con los puntos de poder seran de un aprendiz\n\t\t4. Si usas el nombre de un guardian que ya existe, se te asignara dicho guardian\n\t\ty aplicaran las reglas de Seleccion de guardian (Se conserva el nombre, la aldea\n\t\tde origen y se reinician los puntos de poder)" << endl;
		}
	}
}

//AGREGAR LAS SIGUIENTES FUNCIONES A LA PARTE DE ARRIBA

//Seleccionar guardian como jugador
void SelectPlayer(GuardianNode** guardians, VillageNode** villages, Player** player)
{
	
}

//HASTA AQUI

//Asignacion de datos al crear un jugador
void CreatePlayerAssignData(Player** player, Guardian* guardian)
{
	string name = guardian->GetName();
	string village = guardian->GetVillage();
	
	*player = new Player(name, minPower, village);
}

//Advertencia al crear un jugador: Hay un guardian con el mismo nombre
void CreatePlayerNameWarning(bool* askName, Player** player, Guardian* guardianSameName, bool* stopLoop)
{
	int option = 0;
	
	do
	{
		cout << "\n\tAdvertencia: Ya existe un guardian con ese nombre, por lo que no podras\n\tseleccionar la aldea de inicio. Indica la accion que deseas realizar;\n\n\t\t* Presiona <0> para cancelar y escribir otro nombre\n\t\t* Presiona <1> para continuar sin cambiar el nombre\n\t\t* Presiona <2> para ver mas informacion del guardian con el mismo nombre\n\n\t\t* Ingresa tu opcion: ";
		cin >> option;
		
		while(option < 0 || option > 2)
		{
			cout << "\n\t\t\t* (Error: Opcion invalida) Por favor, selecciona una opcion definida: ";
			cin >> option;
		}
		
		if(option == 0)
		{
			*askName = true;
		}
		else if(option == 1)
		{
			CreatePlayerAssignData(player, guardianSameName);
			*stopLoop = true;
		}
		else
		{
			guardianSameName->PrintData(true, true, true, true, false);
			cout << endl;
		}
	}
	while(option != 0 && option != 1);
}

//Creacion del jugador: Entrada y verificacion del nombre
void CreatePlayerNameVerification(string* name, Player** player, GuardianNode** guardians)
{
	bool askName = true, stopLoop = false;
	
	do
	{
		if(askName)
		{
			cout << "\n\t* Ingresa el nombre del guardian: ";
			askName = false;
		}
		
		cin >> *name;
		
		Guardian* guardianSameName = (*guardians)->SearchGuardian(*guardians, *name);
		
		if(guardianSameName != nullptr) //Ya existe un guardian con el mismo nombre
		{
			if(guardianSameName->GetVillage() == mainVillage)
			{
				cout << "\n\t\t* (Error: No puedes elegir a " << guardianSameName->GetName() << " ya que es el maestro supremo)\n\t\tPor favor, ingresa otro nombre: ";
			}
			else
			{
				CreatePlayerNameWarning(&askName, player, guardianSameName, &stopLoop);
			}
		}
		else
		{
			stopLoop = true;
		}
	}
	while(!stopLoop);
}

//Creacion del jugador: Entrada y verificacion de la aldea
void CreatePlayerVillageVerification(VillageNode** villages, string* village)
{
	int numVillage = 0;
	(*villages)->PrintJustNames(*villages);
	cout << endl;
	
	cout << "\n\t* Ingresa el NUMERO de la aldea en la que deseas iniciar: ";
	cin >> numVillage;
	
	Village* villageSelected = (*villages)->SearchVillageByNum(*villages, numVillage);
	
	while(villageSelected == nullptr) //La aldea no existe o el numero ingresado no estaba dentro del rango
	{
		cout << "\n\t\t* (Error: La aldea no existe o el numero estaba fuera de rango) Por favor, selecciona una aldea de la lista: ";
		cin >> numVillage;
		villageSelected = (*villages)->SearchVillageByNum(*villages, numVillage);
	}
	
	while(villageSelected->GetName() == mainVillage) //La aldea seleccionada no puede ser la aldea principal
	{
		cout << "\n\t\t* (Error: La aldea de origen no puede ser " << mainVillage << " ya que es la aldea principal)\n\t\tPor favor, selecciona otra aldea de origen: ";
		cin >> numVillage;
		villageSelected = (*villages)->SearchVillageByNum(*villages, numVillage);
		
		while(villageSelected == nullptr) //La aldea no existe o el numero ingresado no estaba dentro del rango
		{
			cout << "\n\t\t* (Error: La aldea no existe o el numero estaba fuera de rango) Por favor, selecciona una aldea de la lista: ";
			cin >> numVillage;
			villageSelected = (*villages)->SearchVillageByNum(*villages, numVillage);
		}
	}
	
	*village = villageSelected->GetName();
}

//Crear jugador
void CreatePlayer(GuardianNode** guardians, VillageNode** villages, Player** player)
{
	cout << "\n\t------------------------- CREACION DE GUARDIAN -------------------------" << endl;
	string name, village;
	bool stopLoop = false;
	
	
	CreatePlayerNameVerification(&name, player, guardians); //Entrada y verificacion del nombre
	
	if(*player == nullptr) //El player aun no se crea
	{
		CreatePlayerVillageVerification(villages, &village); //Entrada y verificacion de la aldea
		*player = new Player(name, minPower, village);
	}
	
	cout << "\n\t\tPlayer creado\n\n\t\tNombre: " << (*player)->GetName() << "\n\t\tNivel de poder: " << (*player)->GetPower() << "\n\t\tAldea de origen: " << (*player)->GetHomeVillage() << endl;
}

//Menu de inicio
void MainMenu(GuardianNode** guardians, VillageNode** villages, Player** player, bool* endGame)
{
	cout << "\n\t---------------------------------------- THE GUARDIAN JOURNEY: EN BUSCA DE LA GLORIA ----------------------------------------\n";
	cout << "\n\tBienvenido a The Guardian Journey: En busca de la gloria, un juego en el que podras poner a prueba tus habilidades,\n\tenfrentandote a diferentes guardianes con el objetivo de llegar a " << mainVillage << ", la ciudad principal, en donde podras enfrentarte\n\tal maestro supremo y coronarte como el Maestro de los Guardianes. Para ello, deberas recorrer todas las aldeas y ganar\n\tpuntos de poder en enfrentamientos contra los maestros y sus aprendices. Espero que estes listo para comenzar este\n\temocionante viaje, pero antes, debes crear el personaje con el que alcanzaras la gloria. Para ello, selecciona una de\n\tlas siguientes opciones:" << endl;
	
	int option = 0;
	bool stopLoop = false, selection = true;

	do
	{
		if(selection)
		{
			MainMenuSelection(&option, &selection); //Seleccion de opcion
		}
		
		MainMenuPostSelection(option, &selection, &stopLoop); //Post seleccion de opcion: Cancelar, confirmar o ver mas informacion
	}
	while(!stopLoop);
	
	if(option == 1) //Seleccion de guardian
	{
		SelectPlayer(guardians, villages, player);
	}
	else if(option == 2) //Creacion de guardian
	{
		CreatePlayer(guardians, villages, player);
	}
	else
	{
		*endGame = true;
		cout << "\n\t\tCerrando el juego...\n";
	}
}

//Inicializacion del juego: Creacion del grafo de aldeas
void InitializeGame(VillageNode** villages)
{	
	//Grafo del juego: Creacion de mapas, creacion de la matriz de adyacencia y conexiones entre las aldeas
	Graph* villagesGraph = new Graph();
	villagesGraph->CreateMaps(*villages);
	villagesGraph->CreateMatrix();
	villagesGraph->CreateConnections(*villages);
	villagesGraph->PrintGraph();
	
	cout << "\n\tComienza el viaje..." << endl;	
}

//Menu del juego
void GameMenu(int* option)
{
	cout << "\n\t------------------------------ MENU DE JUEGO ------------------------------" << endl;
	cout << "\n\tSelecciona la accion que quieres realizar;\n\n\t\t* Presiona <0> para salir del juego\n\t\t* Presiona <1> para entrenar con algun guardian de la aldea\n\t\t* Presiona <2> para ir a otra ciudad\n\n\t\t* Ingresa tu opcion: ";

	cin >> *option;
	
	while(*option < 0 || *option > 2)
	{
		cout << "\n\t\t\t* (Error: Opcion invalida) Por favor, selecciona una opcion definida: ";
		cin >> *option;
	}
}

//Obtener el guardian mas facil de enfrentar en el modo de entrenamiento
void TrainingModeGetEasiestGuardian(vector<Guardian*> guardians, Guardian** easiest)
{
	for(const auto& guardian : guardians)
	{
		if(*easiest == nullptr)
		{
			*easiest = guardian;
		}
		else
		{
			if(guardian->GetPowerLevel() <= (*easiest)->GetPowerLevel())
			{
				*easiest = guardian;
			}
		}
	}
}

//Obtener la probabilidad de victoria en un enfrentamiento
int TrainingModeGetWinProbability(Guardian* guardian, Player* player)
{
	int difference = guardian->GetPowerLevel() - player->GetPower();
	//int winProbability = 0;
	
	if(difference <= 0)
	{
		return 100;
	}
	else
	{
		return 100 - (difference / 2); 
	}
}

//Obtener la dificultad de un enfrentamiento
string TrainingModeGetDifficulty(int winProbability)
{
	if(winProbability >= 90)
	{
		return "Facil";
	}
	else if(winProbability >= 80)
	{
		return "Media";
	}
	else if(winProbability >= 70)
	{
		return "Dificil";
	}
	else
	{
		return "Imposible";
	}
}

//Menu de accion al momento de seleccionar un oponente
int TrainingModeOpponentSelection()
{
	int option = 0;
	
	cout << "\n\t------------------------- MENU DE ACCIONES -------------------------" << endl;
	cout << "\n\t\t* Presiona <1> para aceptar la sugerencia de combate\n\t\t* Presiona <2> para seleccionar el guardian que quieras\n\n\t\t* Ingresa tu opcion: ";
	cin >> option;
	
	while(option != 1 && option != 2)
	{
		cout << "\n\t\t\t* (Error: Opcion invalida) Por favor, selecciona una opcion definida: ";
		cin >> option;
	}
	
	return option;
}

//Imprimir los guardianes a los que se puede enfrentar el player
void TrainingModePrintGuardians(Village* village, Player* player, Guardian* master, int* selection, Guardian** opponent)
{
	vector<Guardian*> guardians = village->GetGuardians();
	int count = 1, suggestedOpponent = 0;
	Guardian* easiest = nullptr;
	Guardian* tempOpponent = nullptr;
	
	TrainingModeGetEasiestGuardian(guardians, &easiest); //Se obtiene el guardian mas facil de enfrentar
	cout << "\n\tGuardianes:\n" << endl;
	
	for(const auto& guardian : guardians)
	{
		int winProbability = TrainingModeGetWinProbability(guardian, player); //Probabilidad de victoria
		string difficulty = TrainingModeGetDifficulty(winProbability);
		
		if(guardian == master)
		{
			cout << "\t- Guardian " << count << ": " << guardian->GetName() << " --- Probabilidad de vencerlo: " << winProbability << "% --- Dificultad: " << difficulty << " (Maestro de la aldea)" << endl;
		}
		else if(guardian == easiest)
		{
			cout << "\t- Guardian " << count << ": " << guardian->GetName() << " --- Probabilidad de vencerlo: " << winProbability << "% --- Dificultad: " << difficulty << " (Sugerencia para enfrentamiento)" << endl;
			suggestedOpponent = count;
			tempOpponent = guardian;
		}
		else
		{
			cout << "\t- Guardian " << count << ": " << guardian->GetName() << " --- Probabilidad de vencerlo: " << winProbability << "% --- Dificultad: " << difficulty<< endl;
		}
		
		count++;
	}
	
	if(TrainingModeOpponentSelection() == 1) //El jugador acepto la sugerencia de combate
	{
		*opponent = tempOpponent;
		*selection = suggestedOpponent;
	}
}

//Lanzamiento de dados para saber el resultado del enfrentamiento
void TrainingModeDiceRoll(vector<int>* winningNumbers, int nums)
{
	//Lanzamiento de dados (10 numeros)
	srand(time(NULL));
	int dice = 0;
	
	for(int i = 0; i < nums; i++)
	{
		dice = rand() % 10 + 1;
		bool add = true;
		
		if(i != 0)
		{
			do
			{
				add = true;
				
				for(const auto& num : *winningNumbers) //Se comprueba que el numero no este en la lista de numeros ganadores
				{
					if(num == dice) //El numero ya esta en la lista
					{
						add = false;
					}
				}
				
				if(!add)
				{
					dice = rand() % 10 + 1; //Se lanza nuevamente el dado en caso de que el numero ya estuviera
				}
			}
			while(!add);
		}
		
		(*winningNumbers).push_back(dice);
	}
}

//Actualizacion de los datos del jugador
void TrainingModeUpdatePlayer(Player** player, int points, Village* current)
{
	bool maxReached = false;
	(*player)->IncrementPower(points); //Incremento de poder
	(*player)->IncrementPointsInARow(points, &maxReached); //Incremento del puntaje obtenido de manera seguida
	
	int lastPower = (*player)->GetPower() - (*player)->GetPointsInARow(); //Poder con el que el jugador llego a la aldea actual
	
	(*player)->PrintData(true, lastPower);
	
	if(maxReached) //Se alcanzo el maximo puntaje de la aldea
	{
		current->MaxPointsInARowReached();
	}
}

//Resultado del enfrentamiento
void TrainingModeShowdownResult(vector<int> winningNumbers, Guardian* opponent, Player** player, bool master, Village* village)
{
	int dice = rand() % 10 + 1, victoryPoints = 1;
	bool playerWin = false;
	
	if(master)
	{
		victoryPoints = 2;
	}
	
	for(const auto& num : winningNumbers)
	{
		if(num == dice)
		{
			playerWin = true;
		}
	}
	
	if(playerWin)
	{
		cout << "\n\t\t* Felicidades!! Has derrotado a " << opponent->GetName() << " por lo que has ganado " << victoryPoints << " puntos *" << endl;
		TrainingModeUpdatePlayer(player, victoryPoints, village);
	}
	else
	{
		cout << "\n\t\t* Lo siento :( Has sido derrotado por " << opponent->GetName() << ", pero no te rindas, lo importante es seguir mejorando :) *" << endl;
	}
}

//Enfrentamiento de guardianes
void TrainingModeGuardiansShowdown(Guardian* opponent, Player** player, bool master, Village* village)
{
	int winProbability = TrainingModeGetWinProbability(opponent, *player);
	string difficulty = TrainingModeGetDifficulty(winProbability);
	int easyNums = 8, mediumNums = 6, hardNums = 4, impossibleNums = 2; //Cantidad de numeros ganadores por dificultad
	vector<int> winningNumbers; //Numeros ganadores
	
	//Dependiendo del nivel de dificultad se escogen numeros ganadores
	if(difficulty == "Facil")
	{
		TrainingModeDiceRoll(&winningNumbers, easyNums);
	}
	else if(difficulty == "Media")
	{
		TrainingModeDiceRoll(&winningNumbers, mediumNums);
	}
	else if(difficulty == "Dificil")
	{
		TrainingModeDiceRoll(&winningNumbers, hardNums);
	}
	else
	{
		TrainingModeDiceRoll(&winningNumbers, impossibleNums);
	}
	
	TrainingModeShowdownResult(winningNumbers, opponent, player, master, village);
}

//Modo de entrenamiento
void TrainingMode(Village* village, Player** player)
{
	cout << "\n\t----------------------------------- ENTRENAMIENTO -----------------------------------" << endl;
	cout << "\n\tEntrenar aumenta tus puntos de poder de la siguiente manera:\n\n\t\t- Obtienes +1 punto si derrotas un aprendiz\n\t\t- Obtienes +2 puntos si derrotas al maestro de la aldea\n\n\t* Consideraciones:\n\n\t\t1. Dependiendo de los puntos de poder de cada guardian el enfrentamiento\n\t\ttendra una dificultad y una probabilidad de ganar\n\t\t2. El maximo de puntos seguidos que puedes alcanzar en una aldea son 4\n\n\tCon eso en mente, puedes continuar;" << endl;
	string name = village->GetName();
	
	if(village->GetPointsInARowReached())
	{
		cout << "\n\t* Ya has alcanzado el puntaje maximo en " << name << ", por lo que tendras que\n\tcontinuar tu viaje *" << endl;
	}
	else
	{
		int numGuardians = village->GetNumGuardians(), guardianSelection = -1;
		Guardian* opponent = nullptr;
		bool master = false;
		
		cout << "\n\tAldea actual: " << name;
		TrainingModePrintGuardians(village, *player, village->GetMainMaster(), &guardianSelection, &opponent); //Mostrar los guardianes
		
		if(guardianSelection == -1) //El jugador no acepto la sugerencia de combate
		{
			cout << "\n\t* Ingresa el NUMERO del guardian con el que deseas entrenar: ";
			cin >> guardianSelection;
			
			while(guardianSelection <= 0 || guardianSelection > numGuardians)
			{
				cout << "\n\t\t* (Error: Opcion fuera de rango) Por favor, selecciona un guardian de la lista: ";
				cin >> guardianSelection;
			}
			
			opponent = village->SearchGuardianByIndex(guardianSelection);
		}
		
		if(opponent == village->GetMainMaster()) //El oponente es el maestro
		{
			master = true;
		}
		
		cout << "\n\t* Has elegido a " << opponent->GetName() << " como tu oponente *\n\t* Entrando en la arena de entrenamiento *\n\t* Enfrentamiento en curso... *\n\t* Obteniendo resultados del enfrentamiento... *" << endl;
		TrainingModeGuardiansShowdown(opponent, player, master, village); //Enfrentamiento entre el jugador y el guardian seleccionado
	}	
}

//Viajar a otra aldea
void TravelToVillage(Village* currentVillage, VillageNode* villages, Player** player)
{
	cout << "\n\t----------------------------------- VIAJE A OTRA ALDEA -----------------------------------" << endl;
	cout << "\n\tViajar a otra aldea te permite enfrentarte a otros guardianes y completar el viaje entre\n\taldeas, requisito necesario para llegar a " << mainVillage << " y enfrentarte al maestro supremo y\n\tobtener el titulo de Maestro de los guardianes. Ademas, cada vez que visitas una aldea\n\tobtienes +1 punto de poder por lo que es una buena opcion si no puedes derrotar a algun\n\tmaestro mas fuerte.\n\n\t* Consideracion: Solo puedes moverte entre aldeas que estan conectadas\n\n\tAhora puedes continuar con la seleccion de la aldea a la que quieres viajar;" << endl;

	vector<string> adjacentVillages = currentVillage->GetAdjacentVillagesNames();
	cout << "\n\tAldea actual: " << currentVillage->GetName();
	cout << "\n\tAldeas a las que puedes viajar:\n" << endl;
	int count = 1, numVillages = currentVillage->GetNumAdjacentVillages();
	
	for(const auto& village : adjacentVillages)
	{
		cout << "\t- Aldea " << count << ": " << village << endl;
		count++;
	}
	
	int option = 0;
	
	cout << "\n\t* Ingresa el NUMERO de la aldea a la que quieres viajar: ";
	cin >> option;
	
	while(option <= 0 || option > numVillages)
	{
		cout << "\n\t\t* (Error: Aldea fuera de rango) Por favor, selecciona una aldea que este dentro del rango: ";
		cin >> option;
	}
	
	string newVillageName = currentVillage->SearchVillageByIndex(option);
	Village* newVillage = villages->SearchVillage(villages, newVillageName);
	cout << "\n\t* Viajando a " << newVillageName << " *\n\t* Has llegado a " << newVillageName << " y has obtenido 1 punto!! *" << endl;
	(*player)->SetCurrentVillage(newVillageName); //Cambia la aldea actual del player
	(*player)->RestartPointsInARow(); //Reinicio del puntaje obtenido de manera seguida
}

//Loop del juego
void GameLoop(GuardianNode** guardians, VillageNode** villages, Player** player)
{
	InitializeGame(villages); //Inicializacion del juego (Grafo)
	bool stopGame = false;
	int option = 0;
	
	do
	{
		GameMenu(&option); //Menu del juego, entrada y validacion
		
		Village* currentVillage = (*villages)->SearchVillage(*villages, (*player)->GetCurrentVillage()); //Se obtiene la aldea actual en la que esta el jugador
		
		if(option == 1) //Entrenamiento
		{
			TrainingMode(currentVillage, player);
		}
		else if(option == 2) //Ir a otra aldea
		{
			TravelToVillage(currentVillage, *villages, player);
		}
		else
		{
			stopGame = true;
			cout << "\n\t\tCerrando el juego..." << endl;
		}
	}
	while(!stopGame);
}

