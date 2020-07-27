#include <global.h>
#include <checkArgs.hpp>

uint64_t* array_numbers = nullptr;

int main(int argc, char** argv){

	uint64_t totalElementos;
	uint32_t numThreads;
	uint32_t min;
	uint32_t max;

	auto argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);

	totalElementos = argumentos->getArgs().tamProblema;
	numThreads     = argumentos->getArgs().numThreads;
	min	       = argumentos->getArgs().minRandom;
	max            = argumentos->getArgs().maxRandom;

	std::cout << "  " << std::endl;
        std::cout << "Elementos: " << totalElementos << std::endl;
        std::cout << "Threads  : " << numThreads     << std::endl;
        std::cout << "Límite inferior: " << min << std::endl;
        std::cout << "Límite superior: " << max << std::endl;

	std::srand(std::time(0));
        std::random_device device;
        std::mt19937 rng(device());
        std::uniform_int_distribution<uint32_t> unif(min, max);


	// ======LLENADO DE LA ESTRUCTURA CON NUMEROS==Secuencial
	array_numbers = new uint64_t[totalElementos];

	auto start = std::chrono::high_resolution_clock::now();
	for(uint64_t i = 0; i < totalElementos; ++i){
                array_numbers[i] = unif(rng);
        }

	auto end     = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeFill_secuencial = elapsed.count();

	delete[] array_numbers;

	// ======LLENADO DE LA ESTRUCTURA CON NUMEROS==openMP
	array_numbers = new uint64_t[totalElementos];

	start = std::chrono::high_resolution_clock::now();
	#pragma omp parallel for  num_threads(numThreads)
        for(size_t i = 0; i < totalElementos; ++i){
                array_numbers[i] = unif(rng);
        }
	end     = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeFill_openMP = elapsed.count();

        uint32_t acumSecuencial = 0;
	uint32_t acumParalel = 0;

	//=====SUMA===Secuencial	
	start = std::chrono::high_resolution_clock::now();
	for (size_t i=0; i< totalElementos; i++){
		acumSecuencial+= array_numbers[i];
	}

	end     = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        auto totalTimeSum_secuencial = elapsed.count();

	//std::cout << " " << std::endl;
        //std::cout << "Sumas parciales: ";

	//=====SUMA DE NUMEROS===OpenMP
        start = std::chrono::high_resolution_clock::now();
	#pragma omp parallel for reduction(+:acumParalel) num_threads(numThreads)
        for(size_t i = 0; i < totalElementos; ++i){
                acumParalel += array_numbers[i];
        }
    	end     = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        auto totalTimeSum_paralel = elapsed.count();

//        std::cout << "Total: " <<  acumParalel << std::endl;

//	std::cout << " " << std::endl;
//	std::cout << "Numeros del arreglo: ";
 //	for (uint64_t i = 0; i < totalElementos; i++)
   //     {
//                std::cout << array_numbers[i] <<  " " ;
     //  }

	std::cout << "\n";
        std::cout << "====Llenado arreglo====" << std::endl;
        std::cout << "Tiempo secuencial : "  << totalTimeFill_secuencial<< "[ms]" << std::endl;
        std::cout << "Tiempo openMP    : "  << totalTimeFill_openMP<< "[ms]" << std::endl;
        std::cout << "SpeedUp           : "  << (double)totalTimeFill_secuencial/totalTimeFill_openMP << "\n" <<std::endl;

        std::cout << "====Modulo de Sumas del arreglo====" << std::endl;
        std::cout << "Tiempo secuencial : "  << totalTimeSum_secuencial << "[ms]" << std::endl;
        std::cout << "Tiempo openMP    : "  << totalTimeSum_paralel << "[ms]" << std::endl;
        std::cout << "SpeedUp           : "  << (double)totalTimeSum_secuencial/totalTimeSum_paralel << "\n" <<std::endl;

        std::cout << "====Comprobación de sumas====" << std::endl;
        std::cout << "Suma Total serial: " << acumSecuencial << std::endl;
        std::cout << "Suma Total en openMP: " << acumParalel << "\n" <<std::endl;

        std::cout << "====Tiempos totales==== " << std::endl;
        std::cout << "Tiempo total secuencial: " << totalTimeFill_secuencial + totalTimeSum_secuencial << "[ms]" << std::endl;
        std::cout << "Tiempo Total openMP: " << totalTimeFill_openMP + totalTimeSum_paralel << "[ms]" << "\n" <<std::endl;

	return(EXIT_SUCCESS);

}

