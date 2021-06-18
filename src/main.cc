#include <global.h>
#include <checkArgs.hpp>

double getRandomFloatInRange(double min, double max);
void displaySimulation(int rows, int columns, char* matrix, int show);
void beginSimulation(int rows, int columns, double density, int show, int seq, int32_t nt);
char* createSimulation(int rows, int columns, double density);
char* runSimulation(int rows, int columns, char* prevMatrix);
void applyBorders(int rows, int columns, char* matrix);
int countNeighborhood(int rows, int columns, int x, int y, char* matrix);
char* stepSimulation(int rows, int columns, char* prevMatrix);



/*
Main
name: main
desc: This function will take, parse, and validate the inputs from the user through the
      commandline before starting the game.
params: (1) int argc - The number of symbols in the code
        (2) char* argv[] - The string arguments passed to the program.
returns: An exit code
*/
int main(int argc, char* argv[])
{

  int32_t nt  ;
  int32_t seq   = 0;
  int32_t show  = 0;
  int columns;
  int rows;
  double density;
  
  std::string mystr;

  for (size_t i=0; i < argc; i++){
    mystr=argv[i];
    if(argc < 6)
    {
     printf("Usage : ./program --ncol --nfil --nt --plive --seq --show>\n");
     return -1;
    }
    if (mystr == "--ncol" ) {
			columns = atoi(argv[i+1]);
      if(columns <= 0)
      {
        printf("Las columnas deben ser mayor a 0\n");
        return -1;
      }
		}
    
    if (mystr == "--nfil") {
			rows = atoi(argv[i+1]);
      if(rows < 1){
        printf("Las filas deben ser mayor a 0\n");
        return -1;
      }
		}
    
    if (mystr == "--nt") {
			nt = atoi(argv[i+1]);
      if(nt <= 0){
        printf("Los threads deben ser mayor a 0\n ");
        return -1;

      }
		}
    if (mystr == "--plive") {
			density = atof(argv[i+1]);
      if(density <= 0 || density >= 1.0)
      {
       printf("La probabilidad que una célula este viva en la primero generación esta entre 0 y 1\n");
       return -1;
      }
		}
    if (mystr == "--seq") {
			seq = atoi(argv[i+1]);
      if(seq < 0 || seq > 1){
        printf("Debe ser 1 (secuencial) o 0(Paralelo/secuencial)");
        return -1;
      }
		}
    if (mystr == "--show") {
			show = atoi(argv[i+1]);
      if(show < 0 || seq > 1){
        printf(" 0 para no mostrar tablero y 1 para mostrar tablero");
        return -1;
      }
		}
  }
  columns+=2;
  rows+=2;
  srand(time(NULL)); //seed random
  //start the fun
  if(seq == 1 ){
    printf("secuencial");
    beginSimulation(rows, columns, density, show, seq, nt);
  }else{
    beginSimulation(rows, columns, density, show, seq, nt);
   // beginSimulationP(rows, columns, density,show);
  }
  return 0; 
}

/*
Get Random Float In Range
name: getRandomFloatInRange
desc: This function will generate a random floating point number between
      the min and max
params: (1) int min - The minimum size of the floating point number
        (2) int max - The maximum size of the floating point number
returns: An integer representing the number of tries allowed to the user
*/
double getRandomFloatInRange(double min, double max)
{
  double frand = (((double)rand()/RAND_MAX)*(max-min))+min;
  return frand;
}

/*
Display Simulation
name: displaySimulation
desc: This function will generate a random floating point number between
      the min and max
params: (1) int row - The # of rows
        (2) int columns - The # of columns
		(3) char* matrix - the matrix to display to the user
returns: Nothing
*/
void displaySimulation(int rows, int columns, char* matrix, int show)
{
  static int stepNumber = 0;
  stepNumber++;
  if(show == 1){
    printf("\n\n\n\n\n\n"); //whitespace for formatting
    for(int y = 1; y < rows-1; y++)
    {
      for(int x = 1; x < columns-1; x++)
      {
        printf("%c ",*(matrix+(y*columns+x)));
      }
      printf("\n");
    }
  }
  printf("Step # : %d\n", stepNumber);
}

/*
Create Simulation
name: createSimulation
desc: This function will generate and populate a matrix with cells.
params: (1) int rows - The # of rows
        (2) int columns - The # of columns
		(3) int boundary - A boolean representing whether or not to use a boundary
		(4) double density - A double representing the % of cells to be alive
returns: A pointer to a string of characters representing the cell matrix
*/
char* createSimulation(int rows, int columns, double density)
{
  char* matrix = (char*)calloc(rows*columns,sizeof(char));
  
  if(matrix == NULL)
	  return NULL;
  
  for(int y = 1; y < rows-1; y++)
  {
    for(int x = 1; x < columns-1; x++)
    {
      if(getRandomFloatInRange(0.0, 1.0) <= density)
      {
        *(matrix+(y*columns+x)) = '#'; //manual indexing
      }
      else
      {
        *(matrix+(y*columns+x)) = '.'; //ditto
      }
    }
  }
  
  if(0 == 0) applyBorders(rows, columns, matrix);
	
  return matrix;
}

/*
Begin Simulation
name: beginSimulation
desc: This function will begin the simulation, entering a permanent loop and "stepping"
      the simulation every second.
params: (1) int rows - The # of rows
        (2) int columns - The # of columns
		(3) double density - A double representing the % of cells to be alive
		(4) int boundary - A boolean representing whether or not to use a boundary
returns: A pointer to a string of characters representing the cell matrix
*/
void beginSimulation(int rows, int columns, double density, int show, int seq, int32_t nt)
{
  auto start = std::chrono::high_resolution_clock::now();
  int totalTimeFill_secuencial = 0;
  char* sim = createSimulation(rows, columns, density);
  char* par = sim;
  
  if(sim == NULL)
  {
	  printf("Unable to allocate memory!\n");
	  return;
  }
  displaySimulation(rows, columns, sim , show);
    auto end     = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    totalTimeFill_secuencial = elapsed.count();
    std::cout << "Tiempo secuencial : "  << totalTimeFill_secuencial<< "[ms]"<< std::endl;
  
  if( seq == 1){
    
    for(int i = 0 ; i <4 ; i++)
    {
      auto start = std::chrono::high_resolution_clock::now();
      int totalTimeFill_secuencial = 0;
      sim = stepSimulation(rows, columns, sim);
    
      if(sim == NULL)
        {
        printf("Unable to allocate memory!\n");
        return;
        }
    
      displaySimulation(rows, columns, sim, show);
      usleep(1000000); //1 second in microseconds
      auto end     = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    totalTimeFill_secuencial = elapsed.count();
    std::cout << "Tiempo secuencial : "  << totalTimeFill_secuencial<< "[ms]" << std::endl;
    }
    /////////////////////////////////////////////////////////////////////
  }else{
    
    for(int i = 0 ; i <10 ; i++)
    {
      auto start = std::chrono::high_resolution_clock::now();
      int totalTimeFill_secuencial = 0;
      sim = stepSimulation(rows, columns, sim);
    
      if(sim == NULL)
        {
        printf("Unable to allocate memory!\n");
        return;
        }
    
      displaySimulation(rows, columns, sim, show);
      usleep(1000000); //1 second in microseconds
      auto end     = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    totalTimeFill_secuencial = elapsed.count();
    std::cout << "Tiempo secuencial : "  << totalTimeFill_secuencial<< "[ms]" << std::endl;
    }


    //paralelo
    
    auto start = std::chrono::high_resolution_clock::now();
    int totalTimeFill_paralelo = 0;
    displaySimulation(rows, columns, par , show);
    auto end     = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    totalTimeFill_paralelo = elapsed.count();
    std::cout << "Tiempo paralelo : "  << totalTimeFill_paralelo<< "[ms]"<< std::endl;
    
    #pragma omp parallel for num_threads(nt)
    for(int i = 0 ; i <10 ; i++)
    {
      auto start = std::chrono::high_resolution_clock::now();
      int totalTimeFill_paralelo = 0;
      par = stepSimulation(rows, columns, par);
    
      displaySimulation(rows, columns, par, show);
      usleep(1000000); //1 second in microseconds
      auto end     = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    totalTimeFill_paralelo = elapsed.count();
    std::cout << "Tiempo paralelo : "  << totalTimeFill_paralelo<< "[ms]" << std::endl;
    }
  }
  
}

/*
Apply Borders
name: applyBorders
desc: This function will begin apply cell boarders, copying the visible row into
      an invisible one to be used for calculations. These rows will not be treated
	  as though they are alive themselves.
params: (1) int rows - The # of rows
        (2) int columns - The # of columns
		(3 - VOLATILE) char* matrix - the matrix to apply borders to.
returns: Nothing, but will modify the provided array
author note: I'm really sorry, I didn't see your heap-matrix thing until it was way too late.
             so everything is indexed doing the math manually.
*/
void applyBorders(int rows, int columns, char* matrix)
{
	//top/bottom
	for(int x = 1; x < columns-1; x++)
	{
		*(matrix+x) = *(matrix+x+columns*(rows-2));
		*(matrix+x+columns*(rows-1))=*(matrix+x+columns);
	}	
	
	//sides
	for(int y = 1; y < rows-1;y++)
	{
		*(matrix+y*columns) = *(matrix+y*columns+columns-2);
		*(matrix+y*columns+columns-1) = *(matrix+y*columns+1);
	}
  
	*(matrix+0) = *(matrix+columns*(rows-2)+columns-2);
	//top right corner
	*(matrix+columns-1) = *(matrix+columns*(rows-2)+1);
	//bottom left corner
	*(matrix+columns*(rows-1)) = *(matrix+columns*2-2);
	//bottom right corner
	*(matrix+columns*(rows-1)+columns-1) = *(matrix+columns+1);
	
}

int countNeighborhood(int rows, int columns, int x, int y, char* matrix)
{
	int count = 0;
	
	int pivot = ((y)*columns+x);
	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			char c = (*(matrix+pivot+(i*columns)+j));
				
			if(c == '#') count++;
		}
	}
  
	return count;
  
}

char* stepSimulation(int rows, int columns, char* prevMatrix)
{
  char* steppedMatrix = (char*)malloc(rows*columns);
  if(steppedMatrix == NULL)
	  return NULL;
  int live;
  int count=0;
  for(int y = 1; y < rows-1; y++)
  {
    for(int x = 1; x < columns-1; x++)
    {
	   live = countNeighborhood(rows,columns,x,y,prevMatrix);
	    char cell = *(prevMatrix+(y*columns+x));
	    if(cell == '#') {
        live--;  //avoid counting self
        count++;
      }
	    *(steppedMatrix+(y*columns+x)) = cell; //just making sure
	  
      if(live < 2)
	    {
	  	  *(steppedMatrix+(y*columns+x)) = '.'; //manual indexing again
	    }
	    else if(live > 3)
  	  {
	  	  *(steppedMatrix+(y*columns+x)) = '.';
        
	    }
	    else if((live == 2 || live == 3) && cell == '#')
	    {
		    *(steppedMatrix+(y*columns+x)) = '#';
        
	    }
	    else if(live == 3 && cell == '.')
	    {
	  	  *(steppedMatrix+(y*columns+x)) = '#';
        
	    }
      
    }
    
  }
  std::cout << "live:" << count<< std::endl;
  //std::cout << "live:" << live<< std::endl;
  free(prevMatrix); //free up previous matrix
  if(0 == 0) applyBorders(rows, columns, steppedMatrix); //apply border
  return steppedMatrix;
 
}
