/*
 * =====================================================
 *  AUTOSAR Standard Types Definition
 * =====================================================
 *  This file contains standard type definitions used
 *  across all AUTOSAR modules
 * =====================================================
 */

#ifndef STD_TYPES_H
#define STD_TYPES_H

/*
 * =====================================================
 *  PLATFORM TYPES
 * =====================================================
 */

/* Boolean type */
#ifndef FALSE
#define FALSE 0u
#endif

#ifndef TRUE  
#define TRUE 1u
#endif

typedef unsigned char boolean;

/* Integer types */
typedef signed char         sint8;
typedef unsigned char       uint8;
typedef signed short        sint16;
typedef unsigned short      uint16;
typedef signed long         sint32;
typedef unsigned long       uint32;
typedef signed long long    sint64;
typedef unsigned long long  uint64;

/* Floating point types */
typedef float               float32;
typedef double              float64;

/* Pointer to void */
typedef void*               VoidPtr;

/* Const pointer to void */
typedef const void*         ConstVoidPtr;

/*
 * =====================================================
 *  STANDARD RETURN TYPES
 * =====================================================
 */

/* Standard return type */
typedef uint8 Std_ReturnType;

/* Standard return values */
#define E_OK                0x00u
#define E_NOT_OK            0x01u

/*
 * =====================================================
 *  VERSION INFO TYPE
 * =====================================================
 */

/* Version information structure */
typedef struct
{
    uint16 vendorID;
    uint16 moduleID;
    uint8  sw_major_version;
    uint8  sw_minor_version;
    uint8  sw_patch_version;
} Std_VersionInfoType;

/*
 * =====================================================
 *  NULL POINTER DEFINITION
 * =====================================================
 */

#ifndef NULL_PTR
#define NULL_PTR ((void*)0)
#endif

/*
 * =====================================================
 *  COMPILER ABSTRACTION MACROS
 * =====================================================
 */

/* Function inlining */
#define INLINE              inline

/* Local functions */
#define LOCAL_INLINE        static inline

/* Compiler specific attributes */
#define STATIC              static

/*
 * =====================================================
 *  MEMORY CLASSIFICATION MACROS
 * =====================================================
 */

/* Automatic variables */
#define AUTOMATIC

/* Static variables */  
#define STATIC_VAR          static

/* Global variables */
#define GLOBAL_VAR

/* Constants */
#define CONST_VAR           const

/*
 * =====================================================
 *  BIT MANIPULATION MACROS
 * =====================================================
 */

/* Set bit */
#define SET_BIT(REG, BIT)       ((REG) |= (BIT))

/* Clear bit */
#define CLEAR_BIT(REG, BIT)     ((REG) &= ~(BIT))

/* Read bit */
#define READ_BIT(REG, BIT)      ((REG) & (BIT))

/* Clear and set bit */
#define MODIFY_REG(REG, CLEARMASK, SETMASK) \
    ((REG) = (((REG) & (~(CLEARMASK))) | (SETMASK)))

/*
 * =====================================================
 *  UTILITY MACROS
 * =====================================================
 */

/* Suppress unused parameter warning */
#define UNUSED(x)               ((void)(x))

/* Array size calculation */
#define ARRAY_SIZE(arr)         (sizeof(arr) / sizeof((arr)[0]))

/* Min/Max macros */
#define MIN(a, b)               (((a) < (b)) ? (a) : (b))
#define MAX(a, b)               (((a) > (b)) ? (a) : (b))

/* Absolute value */
#define ABS(x)                  (((x) < 0) ? -(x) : (x))

/*
 * =====================================================
 *  MEMORY ALIGNMENT MACROS
 * =====================================================
 */

/* Align value to specified boundary */
#define ALIGN_UP(value, alignment)      (((value) + (alignment) - 1) & ~((alignment) - 1))
#define ALIGN_DOWN(value, alignment)    ((value) & ~((alignment) - 1))

/* Check if value is aligned */
#define IS_ALIGNED(value, alignment)    (((value) & ((alignment) - 1)) == 0)

/*
 * =====================================================
 *  DEVELOPMENT ERROR DETECTION
 * =====================================================
 */

#ifdef STD_ON
#undef STD_ON
#endif
#define STD_ON      1u

#ifdef STD_OFF  
#undef STD_OFF
#endif
#define STD_OFF     0u

/*
 * =====================================================
 *  ENDIANNESS DEFINITION
 * =====================================================
 */

/* CPU byte order */
#define CPU_BYTE_ORDER          LOW_BYTE_FIRST

/* Byte order definitions */
#define HIGH_BYTE_FIRST         0u
#define LOW_BYTE_FIRST          1u

/*
 * =====================================================
 *  BIT ORDER DEFINITION  
 * =====================================================
 */

/* CPU bit order */
#define CPU_BIT_ORDER           LSB_FIRST

/* Bit order definitions */
#define MSB_FIRST               0u
#define LSB_FIRST               1u

#endif /* STD_TYPES_H */

/*
 * =====================================================
 *  IMPLEMENTATION NOTES
 * =====================================================
 * 
 * 1. Type Safety:
 *    - Always use AUTOSAR standard types for consistency
 *    - Avoid mixing signed and unsigned types
 *    - Use explicit casting when necessary
 * 
 * 2. Return Values:
 *    - Always check E_OK/E_NOT_OK return values
 *    - Use Std_ReturnType for function returns
 *    - Implement proper error handling
 * 
 * 3. Memory Management:
 *    - Use NULL_PTR instead of NULL
 *    - Always validate pointer parameters
 *    - Initialize variables to avoid undefined behavior
 * 
 * 4. Compiler Compatibility:
 *    - These definitions work with GCC ARM compiler
 *    - Adjust types if using different compiler
 *    - Check compiler documentation for specific requirements
 * 
 * =====================================================
 */
