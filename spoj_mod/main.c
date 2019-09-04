//
//  main.c
//  spoj_mod
//
//  Created by ernesto alvarado on 30/08/19.
//  Copyright © 2019 ernesto alvarado. All rights reserved.
//

// XXX: https://www.spoj.com/problems/MOD/
// XXX: https://math.stackexchange.com/questions/131127/algorithms-for-solving-the-discrete-logarithm-ax-equiv-b-pmodn-when-gcd
// XXX: https://lumakernel.github.io/ecasdqina/math/number-theory/Baby-Step-Giant-Step

#if 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#ifndef ONLINE_JUDGE
#include <unistd.h>
#include <sys/time.h>
#endif
#include <math.h>
#include <stdint.h>
#include <ctype.h>

#ifdef COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#if 1
#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX LONG_MAX
#endif

#ifndef ULONG_LONG_MAX
#define ULONG_LONG_MAX ULONG_MAX
#endif

#define COMUN_TAM_MAX_LINEA (16*200000)
#define HEAG_LOG_MAX_TAM_CADENA 2000

#define COMUN_BUF_STATICO_TAM 1000
#define COMUN_BUF_STATICO (char[COMUN_BUF_STATICO_TAM] ) { '\0' }

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define COMUN_ASSERT_DUROTE 0
#define COMUN_ASSERT_SUAVECITO 1
#define COMUN_ASSERT_NIMADRES 2

#define COMUN_VALOR_INVALIDO ((tipo_dato)UINT_MAX)
#define COMUN_IDX_INVALIDO ((natural)COMUN_VALOR_INVALIDO)
#define COMUN_FUNC_STATICA static

typedef char byteme;
typedef unsigned int natural;
typedef natural tipo_dato;
typedef long long entero_largo;
typedef unsigned long long entero_largo_sin_signo;
typedef long long bitch_vector;

typedef enum BOOLEANOS {
    falso = 0, verdadero
} bool;

#define COMUN_TIPO_ASSERT COMUN_ASSERT_DUROTE
/*
 #define COMUN_TIPO_ASSERT COMUN_ASSERT_SUAVECITO
 #define COMUN_TIPO_ASSERT COMUN_ASSERT_NIMADRES
 */

#define assert_timeout_dummy(condition) 0;

#if COMUN_TIPO_ASSERT == COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if COMUN_TIPO_ASSERT == COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){while(1){printf("");};abort();}
#endif
#if COMUN_TIPO_ASSERT == COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef COMUN_LOG
#define comun_log_debug(formato, args...) \
do \
{ \
size_t profundidad = 0; \
void *array[HEAG_LOG_MAX_TAM_CADENA]; \
profundidad = backtrace(array, HEAG_LOG_MAX_TAM_CADENA); \
comun_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
} \
while(0);
#else
#define comun_log_debug(formato, args...) 0
#endif

#define comun_max(x,y) ((x) < (y) ? (y) : (x))
#define comun_min(x,y) ((x) < (y) ? (x) : (y))

#define comun_calloc_local(tipo) (&(tipo){0})

void comun_log_debug_func(const char *format, ...);

#ifndef ONLINE_JUDGE
COMUN_FUNC_STATICA void comun_current_utc_time(struct timespec *ts) {
    
#ifdef __MACH__
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#else
#ifdef COMUN_LOG
    clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif
    
}

COMUN_FUNC_STATICA char *comun_timestamp(char *stime) {
    time_t ltime;
    long ms;
#ifndef ONLINE_JUDGE
    struct tm result;
    struct timespec spec;
#endif
    char parte_milisecundos[50];
    
    ltime = time(NULL);
    
#ifndef ONLINE_JUDGE
    localtime_r(&ltime, &result);
    asctime_r(&result, stime);
#endif
    
    *(stime + strlen(stime) - 1) = ' ';
    
#ifndef ONLINE_JUDGE
    comun_current_utc_time(&spec);
    ms = round(spec.tv_nsec / 1.0e3);
#endif
    sprintf(parte_milisecundos, "%ld", ms);
    strcat(stime, parte_milisecundos);
    return stime;
}

#endif
#ifdef COMUN_LOG
void comun_log_debug_func(const char *format, ...) {
    
    va_list arg;
    va_list arg2;
    const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
    const char *HEADER =
    "archivo: %s; funcion: %s; linea %d; nivel: %zd 8====D ";
    char formato[HEAG_LOG_MAX_TAM_CADENA + sizeof(HEADER)
                 + sizeof(PEDAZO_TIMESTAMP_HEADER)] = {'\0'};
    char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = {'\0'};
    char cadena_timestamp[100] = {'\0'};
    
    comun_timestamp(cadena_timestamp);
    sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);
    
    strcpy(formato, pedazo_timestamp);
    strcat(formato, HEADER);
    strcat(formato, format);
    strcat(formato, "\n");
    
    va_start(arg, format);
    va_copy(arg2, arg);
    vprintf(formato, arg2);
    va_end(arg2);
    va_end(arg);
    setbuf(stdout, NULL);
}
#endif

#ifdef COMUN_LOG
COMUN_FUNC_STATICA char *comun_arreglo_a_cadena(tipo_dato *arreglo, natural tam_arreglo,
                                                char *buffer) {
    int i;
    char *ap_buffer = NULL;
    int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
    return NULL;
#endif
    
    memset(buffer, 0, 100);
    ap_buffer = buffer;
    
    for (i = 0; i < tam_arreglo; i++) {
        characteres_escritos += sprintf(ap_buffer + characteres_escritos,
                                        "%1d", *(arreglo + i));
        if (i < tam_arreglo - 1) {
            *(ap_buffer + characteres_escritos++) = ',';
        }
    }
    *(ap_buffer + characteres_escritos) = '\0';
    return ap_buffer;
}

COMUN_FUNC_STATICA char *comun_arreglo_a_cadena_natural(natural *arreglo,
                                                        natural tam_arreglo, char *buffer) {
    int i;
    char *ap_buffer = NULL;
    int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
    return NULL;
#endif
    
    memset(buffer, 0, 100);
    ap_buffer = buffer;
    
    for (i = 0; i < tam_arreglo; i++) {
        characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
                                        *(arreglo + i));
        if (i < tam_arreglo - 1) {
            *(ap_buffer + characteres_escritos++) = ',';
        }
    }
    *(ap_buffer + characteres_escritos) = '\0';
    return ap_buffer;
}
char *comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
                            natural columas_tam, char *buffer) {
    int i;
    natural inicio_buffer_act = 0;
    for (i = 0; i < filas_tam; i++) {
        comun_arreglo_a_cadena(matrix + i * columas_tam, columas_tam,
                               buffer + inicio_buffer_act);
        inicio_buffer_act += strlen(buffer + inicio_buffer_act);
        buffer[inicio_buffer_act++] = '\n';
        /*        comun_log_debug("pero q llave_arr inicio buffer act %u %s",inicio_buffer_act,buffer);*/
    }
    return buffer;
}

COMUN_FUNC_STATICA char *comun_arreglo_a_cadena_entero_largo_sin_signo(
                                                                       entero_largo_sin_signo *arreglo, entero_largo_sin_signo tam_arreglo,
                                                                       char *buffer) {
    int i;
    char *ap_buffer = NULL;
    int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
    return NULL;
#endif
    
    memset(buffer, 0, 100);
    ap_buffer = buffer;
    
    for (i = 0; i < tam_arreglo; i++) {
        characteres_escritos += sprintf(ap_buffer + characteres_escritos,
                                        "%llu", *(arreglo + i));
        if (i < tam_arreglo - 1) {
            *(ap_buffer + characteres_escritos++) = ',';
        }
    }
    *(ap_buffer + characteres_escritos) = '\0';
    return ap_buffer;
}
#else
COMUN_FUNC_STATICA char *comun_arreglo_a_cadena(tipo_dato *arreglo,
                                                natural tam_arreglo, char *buffer) {
    return NULL;
}
COMUN_FUNC_STATICA char *comun_arreglo_a_cadena_natural(natural *arreglo,
                                                        natural tam_arreglo, char *buffer) {
    return NULL;
}
char *comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
                            natural columas_tam, char *buffer) {
    return NULL;
}

COMUN_FUNC_STATICA char *comun_arreglo_a_cadena_entero_largo_sin_signo(
                                                                       entero_largo_sin_signo *arreglo, entero_largo_sin_signo tam_arreglo,
                                                                       char *buffer) {
    return NULL;
}
#endif

#define comun_arreglo_a_cadena_entero_largo_sin_signo_buf_local(a,a_tam) comun_arreglo_a_cadena_entero_largo_sin_signo(a,a_tam,COMUN_BUF_STATICO)
#define comun_arreglo_a_cadena_buf_local(a,a_tam) comun_arreglo_a_cadena(a,a_tam,COMUN_BUF_STATICO)
#define comun_arreglo_a_cadena_natural_buf_local(a,a_tam) comun_arreglo_a_cadena_natural(a,a_tam,COMUN_BUF_STATICO)

COMUN_FUNC_STATICA void comun_strreplace(char s[], char chr, char repl_chr) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == chr) {
            s[i] = repl_chr;
        }
        i++;
    }
}

COMUN_FUNC_STATICA int comun_lee_matrix_long_stdin(tipo_dato *matrix,
                                                   int *num_filas, int *num_columnas, int num_max_filas,
                                                   int num_max_columnas) {
    int indice_filas = 0;
    int indice_columnas = 0;
    tipo_dato numero = 0;
    char *siguiente_cadena_numero = NULL;
    char *cadena_numero_actual = NULL;
    char *linea = NULL;
    
    linea = calloc(COMUN_TAM_MAX_LINEA, sizeof(char));
    
    while (indice_filas < num_max_filas
           && fgets(linea, COMUN_TAM_MAX_LINEA, stdin)) {
        indice_columnas = 0;
        cadena_numero_actual = linea;
        comun_strreplace(linea, '\n', '\0');
        if (!strlen(linea)) {
            comun_log_debug("weird, linea vacia");
            continue;
        }
        for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
             cadena_numero_actual) {
            numero = (tipo_dato)strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
            if (cadena_numero_actual == siguiente_cadena_numero) {
                break;
            }
            *(matrix + indice_filas * num_max_columnas + indice_columnas) =
            numero;
            indice_columnas++;
        }
        if (num_columnas) {
            num_columnas[indice_filas] = indice_columnas;
        }
        indice_filas++;
        comun_log_debug("las filas son %d, con clos %d", indice_filas,
                        indice_columnas);
    }
    
    *num_filas = indice_filas;
    free(linea);
    return 0;
}

COMUN_FUNC_STATICA natural comun_cuenta_bitchs(tipo_dato num) {
    natural bitch_cnt = 0;
    tipo_dato num_mod = 0;
    num_mod = num;
    while (num_mod) {
        num_mod &= ~(num_mod & (-num_mod));
        bitch_cnt++;
    }
    return bitch_cnt;
}

COMUN_FUNC_STATICA char comun_letra_a_valor_minuscula(char letra) {
    return letra - 'a';
}

COMUN_FUNC_STATICA natural comun_max_natural(natural *nums, natural nums_tam) {
    natural max = 0;
    int i = 0;
    
    for (i = 0; i < nums_tam; i++) {
        natural num_act = nums[i];
        if (num_act > max) {
            max = num_act;
        }
    }
    
    return max;
}

COMUN_FUNC_STATICA char *comun_trimea(char *cad, natural cad_tam) {
    char tmp = '\0';
    natural i = 0;
    natural j = 0;
    
    comun_log_debug("entrada %s cad_tam %u", cad, cad_tam);
    while (j < cad_tam && cad[j] != '\0') {
        comun_log_debug("en j %u car %c", j, cad[j]);
        while (j < cad_tam && !isalpha(cad[j])) {
            comun_log_debug("brincando j %u car %c", j, cad[j]);
            j++;
        }
        comun_log_debug("aora j %u car %c", j, cad[j]);
        if (j == cad_tam) {
            comun_log_debug("q ped");
            break;
        }
        tmp = cad[i];
        cad[i] = cad[j];
        cad[j] = tmp;
        i++;
        j++;
    }
    comun_log_debug("llave_arr '%c'", cad[j]);
    
    i = 0;
    while (isalpha(cad[i++]))
        ;
    comun_log_debug("salida %s", cad);
    cad[i - 1] = '\0';
    
    return cad;
}

#endif

#define COMUN_LIMPIA_MEM(m,s) (memset(m,0,s))
#define COMUN_LIMPIA_MEM_STATICA(m) (memset(m,0,sizeof(*(m))))

COMUN_FUNC_STATICA bool comun_es_digito(char c) {
    return c >= '0' && c <= '9';
}

COMUN_FUNC_STATICA byteme comun_caracter_a_num(char c) {
    return c - '0';
}

COMUN_FUNC_STATICA void comun_invierte_arreglo_byteme(byteme *a, natural a_tam) {
    natural i = 0;
    natural j = a_tam - 1;
    while (i < j) {
        byteme t = a[i];
        a[i] = a[j];
        a[j] = t;
        i++;
        j--;
    }
}

COMUN_FUNC_STATICA void comun_invierte_arreglo_natural(natural *a,
                                                       natural a_tam) {
    natural i = 0;
    natural j = a_tam - 1;
    while (i < j) {
        natural t = a[i];
        a[i] = a[j];
        a[j] = t;
        i++;
        j--;
    }
}

COMUN_FUNC_STATICA natural comun_encuentra_minimo_natural(natural *a,
                                                          natural a_tam) {
    natural min = COMUN_VALOR_INVALIDO;
    natural i;
    for (i = 0; i < a_tam; i++) {
        if (min > a[i]) {
            min = a[i];
        }
    }
    return min;
}

COMUN_FUNC_STATICA entero_largo comun_mcd(entero_largo a, entero_largo b) {
    entero_largo r = COMUN_VALOR_INVALIDO;
    while (a && b) {
        entero_largo tmp = b;
        b = a % b;
        a = tmp;
    }
    
    if (!a) {
        r = b;
    }
    if (!b) {
        r = a;
    }
    return r;
}

#define comun_compara_tipo(tipo) COMUN_FUNC_STATICA int comun_compara_##tipo(const void *pa, const void *pb) { \
int r = 0; \
tipo a = *(tipo *)pa; \
tipo b = *(tipo *)pb; \
if (a < b) { \
r = -1; \
} else { \
if (a > b) { \
r = 1; \
} \
} \
return r; \
}

comun_compara_tipo(natural)

COMUN_FUNC_STATICA natural comun_encuentra_divisores(natural n,
                                                     natural *divisores, natural divisores_tam) {
    natural divisores_cnt = 0;
    natural i = 0;
    for (i = 1; i * i < n; i++) {
        if (!(n % i)) {
            assert_timeout(divisores_cnt < divisores_tam);
            divisores[divisores_cnt++] = i;
            assert_timeout(divisores_cnt < divisores_tam);
            divisores[divisores_cnt++] = n / i;
        }
    }
    
    if (i * i == n) {
        assert_timeout(divisores_cnt < divisores_tam);
        divisores[divisores_cnt++] = n / i;
    }
    qsort(divisores, divisores_cnt, sizeof(natural), comun_compara_natural);
    return divisores_cnt;
}

#endif


#if 1

#if 1

#define XXH_PUBLIC_API static inline
#define FORCE_INLINE static inline
#define XXH_FORCE_NATIVE_FORMAT 0

#define XXH_rotl32(x,r) ((x << r) | (x >> (32 - r)))
#define XXH_rotl64(x,r) ((x << r) | (x >> (64 - r)))

typedef uint64_t U64;
typedef uint8_t BYTE;
typedef uint16_t U16;
typedef uint32_t U32;

static const U64 PRIME64_1 = 11400714785074694791ULL;
static const U64 PRIME64_2 = 14029467366897019727ULL;
static const U64 PRIME64_3 = 1609587929392839161ULL;
static const U64 PRIME64_4 = 9650029242287828579ULL;
static const U64 PRIME64_5 = 2870177450012600261ULL;

typedef enum {
    XXH_OK = 0, XXH_ERROR
} XXH_errorcode;

typedef enum {
    XXH_bigEndian = 0, XXH_littleEndian = 1
} XXH_endianess;

typedef struct XXH64_state_s {
    uint64_t total_len;
    uint64_t v1;
    uint64_t v2;
    uint64_t v3;
    uint64_t v4;
    uint64_t mem64[4];
    uint32_t memsize;
    uint32_t reserved[2]; /* never read nor write, might be removed in a future version */
} XXH64_state_t;
/* typedef'd to XXH64_state_t */

typedef enum {
    XXH_aligned, XXH_unaligned
} XXH_alignment;

static int XXH_isLittleEndian(void) {
    const union {
        U32 u;
        BYTE c[4];
    } one = { 1 }; /* don't use static : performance detrimental  */
    return one.c[0];
}
#define XXH_CPU_LITTLE_ENDIAN   XXH_isLittleEndian()

static U64 XXH64_round(U64 acc, U64 input) {
    acc += input * PRIME64_2;
    acc = XXH_rotl64(acc, 31);
    acc *= PRIME64_1;
    return acc;
}

static U64 XXH_read64(const void* memPtr) {
    U64 val;
    memcpy(&val, memPtr, sizeof(val));
    return val;
}

static U64 XXH_swap64(U64 x) {
    return ((x << 56) & 0xff00000000000000ULL)
    | ((x << 40) & 0x00ff000000000000ULL)
    | ((x << 24) & 0x0000ff0000000000ULL)
    | ((x << 8) & 0x000000ff00000000ULL)
    | ((x >> 8) & 0x00000000ff000000ULL)
    | ((x >> 24) & 0x0000000000ff0000ULL)
    | ((x >> 40) & 0x000000000000ff00ULL)
    | ((x >> 56) & 0x00000000000000ffULL);
}

FORCE_INLINE U64 XXH_readLE64_align(const void* ptr, XXH_endianess endian,
                                    XXH_alignment align) {
    if (align == XXH_unaligned)
        return endian == XXH_littleEndian ?
        XXH_read64(ptr) : XXH_swap64(XXH_read64(ptr));
    else
        return endian == XXH_littleEndian ?
        *(const U64*) ptr : XXH_swap64(*(const U64*) ptr);
}

static U64 XXH64_mergeRound(U64 acc, U64 val) {
    val = XXH64_round(0, val);
    acc ^= val;
    acc = acc * PRIME64_1 + PRIME64_4;
    return acc;
}

static U32 XXH_read32(const void* memPtr) {
    U32 val;
    memcpy(&val, memPtr, sizeof(val));
    return val;
}

static U32 XXH_swap32(U32 x) {
    return ((x << 24) & 0xff000000) | ((x << 8) & 0x00ff0000)
    | ((x >> 8) & 0x0000ff00) | ((x >> 24) & 0x000000ff);
}

FORCE_INLINE U32 XXH_readLE32_align(const void* ptr, XXH_endianess endian,
                                    XXH_alignment align) {
    if (align == XXH_unaligned)
        return endian == XXH_littleEndian ?
        XXH_read32(ptr) : XXH_swap32(XXH_read32(ptr));
    else
        return endian == XXH_littleEndian ?
        *(const U32*) ptr : XXH_swap32(*(const U32*) ptr);
}

#define XXH_get32bits(p) XXH_readLE32_align(p, endian, align)

#define XXH_get64bits(p) XXH_readLE64_align(p, endian, align)

static U64 XXH64_avalanche(U64 h64) {
    h64 ^= h64 >> 33;
    h64 *= PRIME64_2;
    h64 ^= h64 >> 29;
    h64 *= PRIME64_3;
    h64 ^= h64 >> 32;
    return h64;
}

static U64 XXH64_finalize(U64 h64, const void* ptr, size_t len,
                          XXH_endianess endian, XXH_alignment align) {
    const BYTE* p = (const BYTE*) ptr;
    
#define PROCESS1_64          \
h64 ^= (*p) * PRIME64_5; \
p++;                     \
h64 = XXH_rotl64(h64, 11) * PRIME64_1;
    
#define PROCESS4_64          \
h64 ^= (U64)(XXH_get32bits(p)) * PRIME64_1; \
p+=4;                    \
h64 = XXH_rotl64(h64, 23) * PRIME64_2 + PRIME64_3;
    
#define PROCESS8_64 {        \
U64 const k1 = XXH64_round(0, XXH_get64bits(p)); \
p+=8;                    \
h64 ^= k1;               \
h64  = XXH_rotl64(h64,27) * PRIME64_1 + PRIME64_4; \
}
    
    switch (len & 31) {
        case 24:
            PROCESS8_64
            ;
            /* fallthrough */
        case 16:
            PROCESS8_64
            ;
            /* fallthrough */
        case 8:
            PROCESS8_64
            ;
            return XXH64_avalanche(h64);
            
        case 28:
            PROCESS8_64
            ;
            /* fallthrough */
        case 20:
            PROCESS8_64
            ;
            /* fallthrough */
        case 12:
            PROCESS8_64
            ;
            /* fallthrough */
        case 4:
            PROCESS4_64
            ;
            return XXH64_avalanche(h64);
            
        case 25:
            PROCESS8_64
            ;
            /* fallthrough */
        case 17:
            PROCESS8_64
            ;
            /* fallthrough */
        case 9:
            PROCESS8_64
            ;
            PROCESS1_64
            ;
            return XXH64_avalanche(h64);
            
        case 29:
            PROCESS8_64
            ;
            /* fallthrough */
        case 21:
            PROCESS8_64
            ;
            /* fallthrough */
        case 13:
            PROCESS8_64
            ;
            /* fallthrough */
        case 5:
            PROCESS4_64
            ;
            PROCESS1_64
            ;
            return XXH64_avalanche(h64);
            
        case 26:
            PROCESS8_64
            ;
            /* fallthrough */
        case 18:
            PROCESS8_64
            ;
            /* fallthrough */
        case 10:
            PROCESS8_64
            ;
            PROCESS1_64
            ;
            PROCESS1_64
            ;
            return XXH64_avalanche(h64);
            
        case 30:
            PROCESS8_64
            ;
            /* fallthrough */
        case 22:
            PROCESS8_64
            ;
            /* fallthrough */
        case 14:
            PROCESS8_64
            ;
            /* fallthrough */
        case 6:
            PROCESS4_64
            ;
            PROCESS1_64
            ;
            PROCESS1_64
            ;
            return XXH64_avalanche(h64);
            
        case 27:
            PROCESS8_64
            ;
            /* fallthrough */
        case 19:
            PROCESS8_64
            ;
            /* fallthrough */
        case 11:
            PROCESS8_64
            ;
            PROCESS1_64
            ;
            PROCESS1_64
            ;
            PROCESS1_64
            ;
            return XXH64_avalanche(h64);
            
        case 31:
            PROCESS8_64
            ;
            /* fallthrough */
        case 23:
            PROCESS8_64
            ;
            /* fallthrough */
        case 15:
            PROCESS8_64
            ;
            /* fallthrough */
        case 7:
            PROCESS4_64
            ;
            /* fallthrough */
        case 3:
            PROCESS1_64
            ;
            /* fallthrough */
        case 2:
            PROCESS1_64
            ;
            /* fallthrough */
        case 1:
            PROCESS1_64
            ;
            /* fallthrough */
        case 0:
            return XXH64_avalanche(h64);
    }
    
    /* impossible to reach */
    assert(0);
    return 0; /* unreachable, but some compilers complain without it */
}

FORCE_INLINE U64 XXH64_endian_align(const void* input, size_t len, U64 seed,
                                    XXH_endianess endian, XXH_alignment align) {
    const BYTE* p = (const BYTE*) input;
    const BYTE* bEnd = p + len;
    U64 h64;
    
    if (len >= 32) {
        const BYTE* const limit = bEnd - 32;
        U64 v1 = seed + PRIME64_1 + PRIME64_2;
        U64 v2 = seed + PRIME64_2;
        U64 v3 = seed + 0;
        U64 v4 = seed - PRIME64_1;
        
        do {
            v1 = XXH64_round(v1, XXH_get64bits(p));
            p += 8;
            v2 = XXH64_round(v2, XXH_get64bits(p));
            p += 8;
            v3 = XXH64_round(v3, XXH_get64bits(p));
            p += 8;
            v4 = XXH64_round(v4, XXH_get64bits(p));
            p += 8;
        } while (p <= limit);
        
        h64 =
        XXH_rotl64(v1,
                   1) + XXH_rotl64(v2, 7) + XXH_rotl64(v3, 12) + XXH_rotl64(v4, 18);
        h64 = XXH64_mergeRound(h64, v1);
        h64 = XXH64_mergeRound(h64, v2);
        h64 = XXH64_mergeRound(h64, v3);
        h64 = XXH64_mergeRound(h64, v4);
        
    } else {
        h64 = seed + PRIME64_5;
    }
    
    h64 += (U64) len;
    
    return XXH64_finalize(h64, p, len, endian, align);
}

#define XXH_FORCE_ALIGN_CHECK 0
XXH_PUBLIC_API unsigned long long XXH64(const void* input, size_t len,
                                        unsigned long long seed) {
#if 0
    /* Simple version, good for code maintenance, but unfortunately slow for small inputs */
    XXH64_state_t state;
    XXH64_reset(&state, seed);
    XXH64_update(&state, input, len);
    return XXH64_digest(&state);
#else
    XXH_endianess endian_detected = (XXH_endianess) XXH_CPU_LITTLE_ENDIAN;
    
    if (XXH_FORCE_ALIGN_CHECK) {
        if ((((size_t) input) & 7) == 0) { /* Input is aligned, let's leverage the speed advantage */
            if ((endian_detected == XXH_littleEndian) || XXH_FORCE_NATIVE_FORMAT)
                return XXH64_endian_align(input, len, seed, XXH_littleEndian,
                                          XXH_aligned);
            else
                return XXH64_endian_align(input, len, seed, XXH_bigEndian,
                                          XXH_aligned);
        }
    }
    
    if ((endian_detected == XXH_littleEndian) || XXH_FORCE_NATIVE_FORMAT)
        return XXH64_endian_align(input, len, seed, XXH_littleEndian,
                                  XXH_unaligned);
    else
        return XXH64_endian_align(input, len, seed, XXH_bigEndian,
                                  XXH_unaligned);
#endif
}

#endif

typedef natural hm_iter;
#define HASH_MAP_VALOR_INVALIDO ((hm_iter)COMUN_VALOR_INVALIDO)
typedef struct hash_map_entry {
    const void *llave;
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
    natural llave_tam;
#endif
    entero_largo valor;
} hm_entry;
typedef struct hash_map_cubeta {
    uint64_t hash;
    hm_entry *entry;
} hm_cubeta;
typedef struct hash_map_robin_hood_back_shift {
    hm_cubeta *buckets_;
    uint64_t num_buckets_;
    uint64_t num_buckets_used_;
    uint64_t probing_min_;
    uint64_t probing_max_;
} hm_rr_bs_tabla;

static inline entero_largo_sin_signo hash_map_robin_hood_hashear(
                                                                 hm_rr_bs_tabla *ht, byteme *llave_arr, natural llave_arr_tam) {
    entero_largo_sin_signo ass = 0;
    
    ass = XXH64(llave_arr, llave_arr_tam, ass) % ht->num_buckets_;
    comun_log_debug("%u mapea a %u", *(natural *)llave_arr, ass);
    return ass;
}

int hash_map_robin_hood_back_shift_init(hm_rr_bs_tabla *ht, int num_cubetas) {
    ht->num_buckets_ = num_cubetas;
    ht->buckets_ = (hm_cubeta *) calloc(ht->num_buckets_, sizeof(hm_cubeta));
    ht->num_buckets_used_ = 0;
    ht->probing_max_ = num_cubetas;
    return 0;
}
int hash_map_robin_hood_back_shift_fini(hm_rr_bs_tabla *ht) {
    uint32_t i = 0;
    i = 0;
    if (ht->buckets_ != NULL) {
        for (i = 0; i < ht->num_buckets_; i++) {
            if (ht->buckets_[i].entry != NULL) {
                free(ht->buckets_[i].entry);
                ht->buckets_[i].entry = NULL;
            }
        }
        free(ht->buckets_);
    }
    return 0;
}
static inline int hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(
                                                                                 hm_rr_bs_tabla *ht, uint64_t index_stored, uint64_t *distance) {
    hm_cubeta cubeta = ht->buckets_[index_stored];
    *distance = 0;
    if (cubeta.entry == NULL)
        return -1;
    uint64_t num_cubetas = ht->num_buckets_;
    uint64_t index_init = cubeta.hash % num_cubetas;
    if (index_init <= index_stored) {
        *distance = index_stored - index_init;
    } else {
        *distance = index_stored + (num_cubetas - index_init);
    }
    return 0;
}
hm_iter hash_map_robin_hood_back_shift_obten(hm_rr_bs_tabla *ht,
                                             const void *key,
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
                                             natural key_len,
#endif
                                             entero_largo *value) {
    uint64_t num_cubetas = ht->num_buckets_;
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
    uint64_t hash = hash_map_robin_hood_hashear(ht, (void *) key, key_len);
#else
    uint64_t hash = (entero_largo)key%num_cubetas;
#endif
    uint64_t index_init = hash;
    uint64_t probe_distance = 0;
    uint64_t index_current=HASH_MAP_VALOR_INVALIDO;
    bool found = falso;
    uint32_t i;
    *value = HASH_MAP_VALOR_INVALIDO;
    for (i = 0; i < num_cubetas; i++) {
        index_current = (index_init + i) % num_cubetas;
        hm_entry *entrada = ht->buckets_[index_current].entry;
        if (entrada == NULL) {
            break;
        }
        hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
                                                                       index_current, &probe_distance);
        if (i > probe_distance) {
            break;
        }
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
        if (!memcmp(entrada->llave, key, entrada->llave_tam))
#else
            if (entrada->llave==key)
#endif
            {
                *value = entrada->valor;
                found = verdadero;
                break;
            }
    }
    if (found)
        return (natural)index_current;
    return HASH_MAP_VALOR_INVALIDO;
}
hm_iter hash_map_robin_hood_back_shift_pon(hm_rr_bs_tabla *ht, const void *key,
                                           
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
                                           natural key_len,
#endif
                                           entero_largo value, bool *nuevo_entry) {
    uint64_t num_cubetas = ht->num_buckets_;
    uint64_t prob_max = ht->probing_max_;
    uint64_t prob_min = ht->probing_min_;
    hm_cubeta *cubetas = ht->buckets_;
    *nuevo_entry = verdadero;
    if (ht->num_buckets_used_ == num_cubetas) {
        *nuevo_entry = falso;
        return HASH_MAP_VALOR_INVALIDO;
    }
    ht->num_buckets_used_ += 1;
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
    uint64_t hash = hash_map_robin_hood_hashear(ht, (void *) key, key_len);
#else
    uint64_t hash = (entero_largo)key%num_cubetas;
#endif
    uint64_t index_init = hash;
    hm_entry *entry = (hm_entry *) calloc(1, sizeof(hm_entry));
    entry->llave = key;
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
    entry->llave_tam = key_len;
#endif
    entry->valor = value;
    uint64_t index_current = index_init % num_cubetas;
    uint64_t probe_current = 0;
    uint64_t probe_distance;
    hm_entry *entry_temp;
    uint64_t hash_temp;
    uint64_t i;
    for (i = 0; i < num_cubetas; i++) {
        index_current = (index_init + i) % num_cubetas;
        hm_cubeta *cubeta = cubetas + index_current;
        if (cubeta->entry == NULL) {
            cubeta->entry = entry;
            cubeta->hash = hash;
            if (index_current > prob_max) {
                ht->probing_max_ = index_current;
            }
            if (index_current < prob_min) {
                ht->probing_min_ = index_current;
            }
            break;
        } else {
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
            if (!memcmp(cubeta->entry->llave, key, cubeta->entry->llave_tam))
#else
                if (cubeta->entry->llave==key)
#endif
                {
                    free(entry);
                    *nuevo_entry = falso;
                    break;
                }
            hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
                                                                           index_current, &probe_distance);
            if (probe_current > probe_distance) {
                // Swapping the current bucket with the one to insert
                entry_temp = cubeta->entry;
                hash_temp = cubeta->hash;
                cubeta->entry = entry;
                cubeta->hash = hash;
                entry = entry_temp;
                hash = hash_temp;
                probe_current = probe_distance;
            }
        }
        probe_current++;
    }
    return (natural)index_current;
}
int hash_map_robin_hood_back_shift_borra(hm_rr_bs_tabla *ht, const void *key
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
                                         ,natural key_len
#endif
) {
    uint64_t num_cubetas = ht->num_buckets_;
    uint64_t prob_max = ht->probing_max_;
    uint64_t prob_min = ht->probing_max_;
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
    uint64_t hash = hash_map_robin_hood_hashear(ht, (void *) key, key_len);
#else
    uint64_t hash = (entero_largo)key%num_cubetas;
#endif
    uint64_t index_init = hash;
    bool found = falso;
    uint64_t index_current = 0;
    uint64_t probe_distance = 0;
    hm_entry *entrada = NULL;
    uint64_t i = 0;
    for (i = 0; i < num_cubetas; i++) {
        index_current = (index_init + i) % num_cubetas;
        entrada = ht->buckets_[index_current].entry;
        hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
                                                                       index_current, &probe_distance);
        if (entrada == NULL || i > probe_distance) {
            break;
        }
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
        if (!memcmp(entrada->llave, key, entrada->llave_tam))
#else
            if (entrada->llave==key)
#endif
            {
                found = verdadero;
                break;
            }
    }
    if (found) {
        free(entrada);
        uint64_t i = 1;
        uint64_t index_previous = 0, index_swap = 0;
        for (i = 1; i < num_cubetas; i++) {
            index_previous = (index_current + i - 1) % num_cubetas;
            index_swap = (index_current + i) % num_cubetas;
            hm_cubeta *cubeta_swap = ht->buckets_ + index_swap;
            hm_cubeta *cubeta_previous = ht->buckets_ + index_previous;
            if (cubeta_swap->entry == NULL) {
                cubeta_previous->entry = NULL;
                break;
            }
            uint64_t distance;
            if (hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(
                                                                               ht, index_swap, &distance) != 0) {
                fprintf(stderr, "Error in FillDistanceToInitIndex()");
            }
            if (!distance) {
                cubeta_previous->entry = NULL;
                break;
            }
            cubeta_previous->entry = cubeta_swap->entry;
            cubeta_previous->hash = cubeta_swap->hash;
        }
        if (i < num_cubetas) {
            if (index_previous == prob_min) {
                prob_min++;
                if (prob_min >= num_cubetas) {
                    prob_min = 0;
                } else {
                    while (prob_min < num_cubetas
                           && ht->buckets_[prob_min].entry) {
                        prob_min++;
                    }
                    if (prob_min >= num_cubetas) {
                        prob_min = num_cubetas;
                    }
                }
                ht->probing_min_ = prob_min;
            }
            if (index_previous == prob_max) {
                prob_max--;
                if (prob_max >= num_cubetas) {
                    prob_max = num_cubetas;
                } else {
                    while (((int64_t) prob_max) >= 0
                           && ht->buckets_[prob_max].entry) {
                        prob_max--;
                    }
                    if (prob_max >= num_cubetas) {
                        prob_max = 0;
                    }
                }
                ht->probing_max_ = prob_max;
            }
        }
        ht->num_buckets_used_--;
        return 0;
    }
    return 1;
}
static inline void hash_map_robin_hood_back_shift_indice_pon_valor(
                                                                   hm_rr_bs_tabla *ht, hm_iter indice, entero_largo valor) {
    assert_timeout(indice <= ht->probing_max_ && indice >= ht->probing_min_);
    hm_entry *entrada = ht->buckets_[indice].entry;
    assert_timeout(entrada);
    entrada->valor = valor;
}
int hash_map_robin_hood_back_shift_indice_borra(hm_rr_bs_tabla *ht,
                                                hm_iter indice) {
    assert_timeout(indice <= ht->probing_max_ && indice >= ht->probing_min_);
    uint64_t num_cubetas = ht->num_buckets_;
    uint64_t prob_max = ht->probing_max_;
    uint64_t prob_min = ht->probing_max_;
    uint64_t index_current = indice;
    hm_entry *entrada = ht->buckets_[index_current].entry;
    assert_timeout(entrada);
    free(entrada);
    uint64_t i = 1;
    uint64_t index_previous = 0, index_swap = 0;
    for (i = 1; i < num_cubetas; i++) {
        index_previous = (index_current + i - 1) % num_cubetas;
        index_swap = (index_current + i) % num_cubetas;
        hm_cubeta *cubeta_swap = ht->buckets_ + index_swap;
        hm_cubeta *cubeta_previous = ht->buckets_ + index_previous;
        if (cubeta_swap->entry == NULL) {
            cubeta_previous->entry = NULL;
            break;
        }
        uint64_t distance;
        if (hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
                                                                           index_swap, &distance) != 0) {
            fprintf(stderr, "Error in FillDistanceToInitIndex()");
        }
        if (!distance) {
            cubeta_previous->entry = NULL;
            break;
        }
        cubeta_previous->entry = cubeta_swap->entry;
        cubeta_previous->hash = cubeta_swap->hash;
    }
    if (i < num_cubetas) {
        if (index_previous == prob_min) {
            prob_min++;
            if (prob_min >= num_cubetas) {
                prob_min = 0;
            } else {
                while (prob_min < num_cubetas && ht->buckets_[prob_min].entry) {
                    prob_min++;
                }
                if (prob_min >= num_cubetas) {
                    prob_min = num_cubetas;
                }
            }
            ht->probing_min_ = prob_min;
        }
        if (index_previous == prob_max) {
            prob_max--;
            if (prob_max >= num_cubetas) {
                prob_max = num_cubetas;
            } else {
                while (((int64_t) prob_max) >= 0 && ht->buckets_[prob_max].entry) {
                    prob_max--;
                }
                if (prob_max >= num_cubetas) {
                    prob_max = 0;
                }
            }
            ht->probing_max_ = prob_max;
        }
    }
    ht->num_buckets_used_--;
    return 0;
}

static inline void hash_map_robin_hood_back_shift_reemplazar(hm_rr_bs_tabla *ht,
                                                             void *llave,
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
                                                             natural llave_tam,
#endif
                                                             entero_largo valor) {
    hm_iter iter = 0;
    entero_largo *valor_int = &(entero_largo ) { 0 };
    
    iter = hash_map_robin_hood_back_shift_obten(ht, llave,
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
                                                llave_tam,
#endif
                                                valor_int);
    
    assert_timeout(iter!=HASH_MAP_VALOR_INVALIDO);
    
    hash_map_robin_hood_back_shift_indice_pon_valor(ht, iter, valor);
}

static inline void hash_map_robin_hood_back_shift_insertar_nuevo(
                                                                 hm_rr_bs_tabla *ht, void *llave,
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
                                                                 natural llave_tam,
#endif
                                                                 entero_largo valor) {
    hm_iter iter = 0;
    bool nuevo = falso;
    iter = hash_map_robin_hood_back_shift_pon(ht, llave,
#ifdef HASH_MAP_ROUND_ROBIN_HASHEAR
                                              llave_tam,
#endif
                                              valor,
                                              &nuevo);
    
    assert_timeout(iter!=HASH_MAP_VALOR_INVALIDO);
    assert_timeout(nuevo);
}

#endif


#if 1
COMUN_FUNC_STATICA entero_largo paso_bebe_paso_gigante(entero_largo a,entero_largo b, entero_largo g){
    entero_largo r=-1;
    hm_rr_bs_tabla *ht=comun_calloc_local(hm_rr_bs_tabla);
    
    a%=g;
    b%=g;
    if(b==1){
        return 0;
    }
    entero_largo mcd=0;
    entero_largo b_inv=1;
    entero_largo bias=0;
    while ((mcd=comun_mcd(a,g))!=1) {
        if(b%mcd){
            return -1;
        }
        b/=mcd;
        g/=mcd;
        b_inv=(b_inv*(a/mcd))%g;
        bias++;
        if(b==b_inv){
            return bias;
        }
    }
    comun_log_debug("a %lld b %lld g %llu bias %lld",a,b,g,bias);

    entero_largo Q=floor(sqrt(g))+1;
    entero_largo Q_tmp=Q;
    entero_largo pot_acum=a%g;
    entero_largo a_Q=1;
    hash_map_robin_hood_back_shift_init(ht, (natural)Q<<2);
    
    while(Q_tmp){
        if(Q_tmp&1){
            a_Q=(a_Q*pot_acum)%g;
        }
        Q_tmp>>=1;
        pot_acum=(pot_acum*pot_acum)%g;
    }
    entero_largo a_Q_x=b_inv;
    comun_log_debug("a^Q^x %lld con b %lld Q %lld",a_Q,b_inv,Q);
    for(natural x=1;x<=Q;x++){
        a_Q_x=(a_Q_x*a_Q)%g;
        entero_largo x_tmp=HASH_MAP_VALOR_INVALIDO;
        hm_iter iter=hash_map_robin_hood_back_shift_obten(ht, (void *)a_Q_x, &x_tmp);
        if(iter==HASH_MAP_VALOR_INVALIDO){
            hash_map_robin_hood_back_shift_insertar_nuevo(ht, (void *)a_Q_x, (entero_largo)x);
            comun_log_debug("a^Q^x %lld no estaba agregando valor %lld",a_Q_x,x);
        }
        else{
            comun_log_debug("a^Q^x %lld ya estaba con valor %lld",a_Q_x,x_tmp);
        }
    }
    
    entero_largo b_a_y=b%g;
    for(natural y=0;y<=Q;y++){
        entero_largo x=0;
        hm_iter iter=hash_map_robin_hood_back_shift_obten(ht, (void *)b_a_y, &x);
        comun_log_debug("b*a^y %lld tiene x %llu", b_a_y, x);
        if(iter!=HASH_MAP_VALOR_INVALIDO){
            entero_largo r_tmp=Q*x-y+bias;
            if(r==-1 || r_tmp<r){
                r=r_tmp;
            }
        }
        
        b_a_y=(b_a_y*a)%g;
    }
    
    hash_map_robin_hood_back_shift_fini(ht);
    return r;
}
#endif

COMUN_FUNC_STATICA void codechef_fibonacci_number_main(){
    natural x=0;
    natural z=0;
    natural k=0;
#ifdef __APPLE__
    if (getenv("STDIN")) {
        if (!freopen(getenv("STDIN"), "r", stdin)) {
            printf("no se pudo stdin con %s",getenv("STDIN"));
            exit (1);
        }
    }
    if (getenv("STDOUT")) {
        if (!freopen(getenv("STDOUT"), "w", stdout)) {
            printf("no se pudo stdin con %s",getenv("STDOUT"));
            exit (1);
        }
    }
    if (getenv("STDERR")) {
        if (!freopen(getenv("STDERR"), "w", stderr)) {
            printf("no se pudo stdin con %s",getenv("STDERR"));
            exit (1);
        }
    }
#endif
    
    while(scanf("%u %u %u\n",&x,&z,&k)) {
        if(!x && !z && !k){
            break;
        }
        entero_largo r=paso_bebe_paso_gigante(x, k, z);
        if(r!=-1){
            
        printf("%lld\n",r);
        }
        else{
            
        printf("No Solution\n");
        }
    }
}

int main(int argc, const char * argv[]) {
    codechef_fibonacci_number_main();
    return 0;
}

