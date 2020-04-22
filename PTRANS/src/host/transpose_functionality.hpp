/*
Copyright (c) 2019 Marius Meyer

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef SRC_HOST_TRANSPOSE_FUNCTIONALITY_H_
#define SRC_HOST_TRANSPOSE_FUNCTIONALITY_H_

/* C++ standard library headers */
#include <memory>

/* Project's headers */
#include "execution.h"
#include "cxxopts.hpp"
#include "setup/fpga_setup.hpp"
#include "parameters.h"

/*
Short description of the program.
Moreover the version and build time is also compiled into the description.
*/
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

#define PROGRAM_DESCRIPTION "Implementation of the matrix transposition benchmark"\
                            " proposed in the HPCC benchmark suite for FPGA.\n"\
                            "Version: " VERSION

struct ProgramSettings {
    uint numRepetitions;
    cl_uint matrixSize;
    int defaultPlatform;
    int defaultDevice;
    bool useMemInterleaving;
    std::string kernelFileName;
    std::string kernelName;
};


/**
Parses and returns program options using the cxxopts library.
Supports the following parameters:
    - file name of the FPGA kernel file (-f,--file)
    - number of repetitions (-n)
    - number of kernel replications (-r)
    - data size (-d)
    - use memory interleaving
@see https://github.com/jarro2783/cxxopts

@return program settings that are created from the given program arguments
*/
std::shared_ptr<ProgramSettings>
parseProgramParameters(int argc, char *argv[]);

/**
 * Reference implementation that takes two matrices and calculates
 *  A_out = trans(A) + B
 *  where A, B and A_out are matrices of size n*n.
 *
 * @param A matrix that has to be transposed
 * @param B matrix that will be added to the transposed matrix
 * @param A_out matrix where the result of the calculation is stored
 * @param n dimension of the matrices
 */
void
transposeReference(HOST_DATA_TYPE *const A, HOST_DATA_TYPE *const B,
                   HOST_DATA_TYPE *A_out, cl_uint n);


void
generateInputData(cl_uint matrix_size, HOST_DATA_TYPE *A, HOST_DATA_TYPE *B);


/**
Prints the execution results to stdout

@param results The execution results
*/
void
printResults(std::shared_ptr<bm_execution::ExecutionTimings> results, cl_uint matrixSize);

/**
 * Prints the used configuration to std out before starting the actual benchmark.
 *
 * @param programSettings The program settings retrieved from the command line
 * @param device The device used for execution
 */
void printFinalConfiguration(const std::shared_ptr<ProgramSettings> &programSettings,
                             const cl::Device &device);


/**
 *  Prints the aggregated error for the result matrix to stdout.
 *  All values in the result matrix should be 0 because of the used matrix generation scheme.
 *  All values of the matrix are aggregated to get to total aggregated error.
 *
 * @param matrixSize Size of the result matrix in one dimension (i. e. N for matrix size N*N)
 * @param result The buffer containing the result matrix
 * @returns the aggregated normalized error
 */
double
printCalculationError(cl_uint matrixSize, const HOST_DATA_TYPE *result);

#endif // SRC_HOST_TRANSPOSE_FUNCTIONALITY_H_
