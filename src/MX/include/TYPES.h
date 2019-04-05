#ifndef TYPES_H
#define TYPES_H

#ifndef __CHAR__
#define __CHAR__
typedef signed char         CHAR;
#endif

#ifndef __UCHAR__
#define __UCHAR__
typedef unsigned char       UCHAR;
#endif

#ifndef __UBYTE__
#define __UBYTE__
typedef unsigned char       UBYTE;
#endif

#ifndef __SHORT__
#define __SHORT__
typedef signed short int    SHORT;
#endif

#ifndef __USHORT__
#define __USHORT__
typedef unsigned short int  USHORT;
#endif

#ifndef __WORD__
#define __WORD__
typedef unsigned short int  WORD;
#endif

//typedef char              BYTE;
#ifndef __BYTE__
#define __BYTE__
typedef unsigned char     BYTE;
#endif

#ifndef __BOOL__
#define __BOOL__
typedef unsigned char       BOOL;
#endif

#ifndef __DWORD__
#define __DWORD__
typedef unsigned long int   DWORD;
#endif

typedef signed long int     LONG;
typedef unsigned long int   ULONG;

#ifndef PACKED
#define PACKED packed
#endif

#endif /* TYPES_H */

