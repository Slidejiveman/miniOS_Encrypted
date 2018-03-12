#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define DEBUG
#define NUM_FUNCS 200 // number of functions possible

/* thread function declarations */ 
// These functions could be expanded to do
// any behavior. Adding sleep to them would
// be a good simulation of the performance
// of different methods.
void *func0() { printf("func0\n"); }
void *func1() { printf("func1\n"); }
void *func2() { printf("func2\n"); }
void *func3() { printf("func3\n"); }
void *func4() { printf("func4\n"); }
void *func5() { printf("func5\n"); }
void *func6() { printf("func6\n"); }
void *func7() { printf("func7\n"); }
void *func8() { printf("func8\n"); }
void *func9() { printf("func9\n"); }

void *func10() { printf("func10\n"); }
void *func11() { printf("func11\n"); }
void *func12() { printf("func12\n"); }
void *func13() { printf("func13\n"); }
void *func14() { printf("func14\n"); }
void *func15() { printf("func15\n"); }
void *func16() { printf("func16\n"); }
void *func17() { printf("func17\n"); }
void *func18() { printf("func18\n"); }
void *func19() { printf("func19\n"); }

void *func20() { printf("func20\n"); }
void *func21() { printf("func21\n"); }
void *func22() { printf("func22\n"); }
void *func23() { printf("func23\n"); }
void *func24() { printf("func24\n"); }
void *func25() { printf("func25\n"); }
void *func26() { printf("func26\n"); }
void *func27() { printf("func27\n"); }
void *func28() { printf("func28\n"); }
void *func29() { printf("func29\n"); }

void *func30() { printf("func30\n"); }
void *func31() { printf("func31\n"); }
void *func32() { printf("func32\n"); }
void *func33() { printf("func33\n"); }
void *func34() { printf("func34\n"); }
void *func35() { printf("func35\n"); }
void *func36() { printf("func36\n"); }
void *func37() { printf("func37\n"); }
void *func38() { printf("func38\n"); }
void *func39() { printf("func39\n"); }

void *func40() { printf("func40\n"); }
void *func41() { printf("func41\n"); }
void *func42() { printf("func42\n"); }
void *func43() { printf("func43\n"); }
void *func44() { printf("func44\n"); }
void *func45() { printf("func45\n"); }
void *func46() { printf("func46\n"); }
void *func47() { printf("func47\n"); }
void *func48() { printf("func48\n"); }
void *func49() { printf("func49\n"); }

void *func50() { printf("func50\n"); }
void *func51() { printf("func51\n"); }
void *func52() { printf("func52\n"); }
void *func53() { printf("func53\n"); }
void *func54() { printf("func54\n"); }
void *func55() { printf("func55\n"); }
void *func56() { printf("func56\n"); }
void *func57() { printf("func57\n"); }
void *func58() { printf("func58\n"); }
void *func59() { printf("func59\n"); }

void *func60() { printf("func60\n"); }
void *func61() { printf("func61\n"); }
void *func62() { printf("func62\n"); }
void *func63() { printf("func63\n"); }
void *func64() { printf("func64\n"); }
void *func65() { printf("func65\n"); }
void *func66() { printf("func66\n"); }
void *func67() { printf("func67\n"); }
void *func68() { printf("func68\n"); }
void *func69() { printf("func69\n"); }

void *func70() { printf("func70\n"); }
void *func71() { printf("func71\n"); }
void *func72() { printf("func72\n"); }
void *func73() { printf("func73\n"); }
void *func74() { printf("func74\n"); }
void *func75() { printf("func75\n"); }
void *func76() { printf("func76\n"); }
void *func77() { printf("func77\n"); }
void *func78() { printf("func78\n"); }
void *func79() { printf("func79\n"); }

void *func80() { printf("func80\n"); }
void *func81() { printf("func81\n"); }
void *func82() { printf("func82\n"); }
void *func83() { printf("func83\n"); }
void *func84() { printf("func84\n"); }
void *func85() { printf("func85\n"); }
void *func86() { printf("func86\n"); }
void *func87() { printf("func87\n"); }
void *func88() { printf("func88\n"); }
void *func89() { printf("func89\n"); }

void *func90() { printf("func90\n"); }
void *func91() { printf("func91\n"); }
void *func92() { printf("func92\n"); }
void *func93() { printf("func93\n"); }
void *func94() { printf("func94\n"); }
void *func95() { printf("func95\n"); }
void *func96() { printf("func96\n"); }
void *func97() { printf("func97\n"); }
void *func98() { printf("func98\n"); }
void *func99() { printf("func99\n"); }

void *func100() { printf("func100\n"); }
void *func101() { printf("func101\n"); }
void *func102() { printf("func102\n"); }
void *func103() { printf("func103\n"); }
void *func104() { printf("func104\n"); }
void *func105() { printf("func105\n"); }
void *func106() { printf("func106\n"); }
void *func107() { printf("func107\n"); }
void *func108() { printf("func108\n"); }
void *func109() { printf("func109\n"); }

void *func110() { printf("func110\n"); }
void *func111() { printf("func111\n"); }
void *func112() { printf("func112\n"); }
void *func113() { printf("func113\n"); }
void *func114() { printf("func114\n"); }
void *func115() { printf("func115\n"); }
void *func116() { printf("func116\n"); }
void *func117() { printf("func117\n"); }
void *func118() { printf("func118\n"); }
void *func119() { printf("func119\n"); }

void *func120() { printf("func120\n"); }
void *func121() { printf("func121\n"); }
void *func122() { printf("func122\n"); }
void *func123() { printf("func123\n"); }
void *func124() { printf("func124\n"); }
void *func125() { printf("func125\n"); }
void *func126() { printf("func126\n"); }
void *func127() { printf("func127\n"); }
void *func128() { printf("func128\n"); }
void *func129() { printf("func129\n"); }

void *func130() { printf("func130\n"); }
void *func131() { printf("func131\n"); }
void *func132() { printf("func132\n"); }
void *func133() { printf("func133\n"); }
void *func134() { printf("func134\n"); }
void *func135() { printf("func135\n"); }
void *func136() { printf("func136\n"); }
void *func137() { printf("func137\n"); }
void *func138() { printf("func138\n"); }
void *func139() { printf("func139\n"); }
 
void *func140() { printf("func140\n"); }
void *func141() { printf("func141\n"); }
void *func142() { printf("func142\n"); }
void *func143() { printf("func143\n"); }
void *func144() { printf("func144\n"); }
void *func145() { printf("func145\n"); }
void *func146() { printf("func146\n"); }
void *func147() { printf("func147\n"); }
void *func148() { printf("func148\n"); }
void *func149() { printf("func149\n"); }

void *func150() { printf("func150\n"); }
void *func151() { printf("func151\n"); }
void *func152() { printf("func152\n"); }
void *func153() { printf("func153\n"); }
void *func154() { printf("func154\n"); }
void *func155() { printf("func155\n"); }
void *func156() { printf("func156\n"); }
void *func157() { printf("func157\n"); }
void *func158() { printf("func158\n"); }
void *func159() { printf("func159\n"); }

void *func160() { printf("func160\n"); }
void *func161() { printf("func161\n"); }
void *func162() { printf("func162\n"); }
void *func163() { printf("func163\n"); }
void *func164() { printf("func164\n"); }
void *func165() { printf("func165\n"); }
void *func166() { printf("func166\n"); }
void *func167() { printf("func167\n"); }
void *func168() { printf("func168\n"); }
void *func169() { printf("func169\n"); }

void *func170() { printf("func170\n"); }
void *func171() { printf("func171\n"); }
void *func172() { printf("func172\n"); }
void *func173() { printf("func173\n"); }
void *func174() { printf("func174\n"); }
void *func175() { printf("func175\n"); }
void *func176() { printf("func176\n"); }
void *func177() { printf("func177\n"); }
void *func178() { printf("func178\n"); }
void *func179() { printf("func179\n"); }
 
void *func180() { printf("func180\n"); }
void *func181() { printf("func181\n"); }
void *func182() { printf("func182\n"); }
void *func183() { printf("func183\n"); }
void *func184() { printf("func184\n"); }
void *func185() { printf("func185\n"); }
void *func186() { printf("func186\n"); }
void *func187() { printf("func187\n"); }
void *func188() { printf("func188\n"); }
void *func189() { printf("func189\n"); }

void *func190() { printf("func190\n"); }
void *func191() { printf("func191\n"); }
void *func192() { printf("func192\n"); }
void *func193() { printf("func193\n"); }
void *func194() { printf("func194\n"); }
void *func195() { printf("func195\n"); }
void *func196() { printf("func196\n"); }
void *func197() { printf("func197\n"); }
void *func198() { printf("func198\n"); }
void *func199() { printf("func199\n"); }

/* type definitions  */
// function pointer arrays have messy syntax
// this typedef helps to make things cleaner
typedef void* (*Func_ptr_t)();

// the function map is a mapping of threads
// to the functions that they run
typedef struct _func_map {
    int ptid; 
    Func_ptr_t func;
    pthread_t thread;
} Func_map;

/* global variables  */
// emulates the hardware interrupt vector as a key to function map
Func_map INTERRUPT_VECTOR[NUM_FUNCS] = { { 0, func0 }, { 1, func1 }, { 2, func2 }, { 3, func3 },  
                                         { 4, func4 }, { 5, func5 }, { 6, func6 }, { 7, func7 },
                                         { 8, func8 }, { 9, func9 }, { 10, func10 }, { 11, func11 },
                                         { 12, func12 }, { 13, func13 }, { 14, func14 }, { 15, func15 },
                                         { 16, func16 }, { 17, func17 }, { 18, func18 }, { 19, func19 },
                                         { 20, func20 }, { 21, func21 }, { 22, func22 }, { 23, func23 },
                                         { 24, func24 }, { 25, func25 }, { 26, func26 }, { 27, func27 },
                                         { 28, func28 }, { 29, func29 }, { 30, func30 }, { 31, func31 },
                                         { 32, func32 }, { 33, func33 }, { 34, func34 }, { 35, func35 },
                                         { 36, func36 }, { 37, func37 }, { 38, func38 }, { 39, func39 },
                                         { 40, func40 }, { 41, func41 }, { 42, func42 }, { 43, func43 },
                                         { 44, func44 }, { 45, func45 }, { 46, func46 }, { 47, func47 },
                                         { 48, func48 }, { 49, func49 }, { 50, func50 }, { 51, func51 },
                                         { 52, func52 }, { 53, func53 }, { 54, func54 }, { 55, func55 },
                                         { 56, func56 }, { 57, func57 }, { 58, func58 }, { 59, func59 },
                                         { 60, func60 }, { 61, func61 }, { 62, func62 }, { 63, func63 },
                                         { 64, func64 }, { 65, func65 }, { 66, func66 }, { 67, func67 },
                                         { 68, func68 }, { 69, func69 }, { 70, func70 }, { 71, func71 },
                                         { 72, func72 }, { 73, func73 }, { 74, func74 }, { 75, func75 },
                                         { 76, func76 }, { 77, func77 }, { 78, func78 }, { 79, func79 },
                                         { 80, func80 }, { 81, func81 }, { 82, func82 }, { 83, func83 },
                                         { 84, func84 }, { 85, func85 }, { 86, func86 }, { 87, func87 },
                                         { 88, func88 }, { 89, func89 }, { 90, func90 }, { 91, func91 },
                                         { 92, func92 }, { 93, func93 }, { 94, func94 }, { 95, func95 },
                                         { 96, func96 }, { 97, func97 }, { 98, func98 }, { 99, func99 },
                                         { 100, func100 }, { 101, func101 }, { 102, func102 }, { 103, func103 },
                                         { 104, func104 }, { 105, func105 }, { 106, func106 }, { 107, func107 },
                                         { 108, func108 }, { 109, func109 }, { 110, func110 }, { 111, func111 },
                                         { 112, func112 }, { 113, func113 }, { 114, func114 }, { 115, func115 },
                                         { 116, func116 }, { 117, func117 }, { 118, func118 }, { 119, func119 },
                                         { 120, func120 }, { 121, func121 }, { 122, func122 }, { 123, func123 },
                                         { 124, func124 }, { 125, func125 }, { 126, func126 }, { 127, func127 },
                                         { 128, func128 }, { 129, func129 }, { 130, func130 }, { 131, func131 },
                                         { 132, func132 }, { 133, func133 }, { 134, func134 }, { 135, func135 },
                                         { 136, func136 }, { 137, func137 }, { 138, func138 }, { 139, func139 },
                                         { 140, func140 }, { 141, func141 }, { 142, func142 }, { 143, func143 },
                                         { 144, func144 }, { 145, func145 }, { 146, func146 }, { 147, func147 },
                                         { 148, func148 }, { 149, func149 }, { 150, func150 }, { 151, func151 },
                                         { 152, func152 }, { 153, func153 }, { 154, func154 }, { 155, func155 },
                                         { 156, func156 }, { 157, func157 }, { 158, func158 }, { 159, func159 },
                                         { 160, func160 }, { 161, func161 }, { 162, func162 }, { 163, func163 },
                                         { 164, func164 }, { 165, func165 }, { 166, func166 }, { 167, func167 },
                                         { 168, func168 }, { 169, func169 }, { 170, func170 }, { 171, func171 },
                                         { 172, func172 }, { 173, func173 }, { 174, func174 }, { 175, func175 },
                                         { 176, func176 }, { 177, func177 }, { 178, func178 }, { 179, func179 },
                                         { 180, func180 }, { 181, func181 }, { 182, func182 }, { 183, func183 },
                                         { 184, func184 }, { 185, func185 }, { 186, func186 }, { 187, func187 },
                                         { 188, func188 }, { 189, func189 }, { 190, func190 }, { 191, func191 },
                                         { 192, func192 }, { 193, func193 }, { 194, func194 }, { 195, func195 },
                                         { 196, func196 }, { 197, func197 }, { 198, func198 }, { 199, func199 }
                                       };

/* helper functions  */

/* processing  */
// Main initializes 20 threads by incrementing
// over the command set, which are the functions
// chosen to be implemented by the customer.
int main(int argc, char *argv[]) 
{
    // local variables 
    int i, rc;
 
    // loop over the command set, and create pthreads to run 
    // for each enabled function.
    for (i = 1; i < argc; ++i) // program name counts as an argument
    {
#ifndef DEBUG
        sleep(1);
#endif
        if ((rc =  pthread_create(&INTERRUPT_VECTOR[atoi(argv[i])].thread, NULL, INTERRUPT_VECTOR[atoi(argv[i])].func , NULL)))
        {
            fprintf(stderr, "error creating thread # %d\n", INTERRUPT_VECTOR[atoi(argv[i])].ptid);
            return EXIT_FAILURE;
        } 
    }

    for (i = 1; i < argc; ++i)
    {
        pthread_join(INTERRUPT_VECTOR[atoi(argv[i])].thread, NULL);
    } 
 
    return EXIT_SUCCESS;
}
