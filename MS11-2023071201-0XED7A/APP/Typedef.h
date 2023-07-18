
//;===========================================================
//;          ����������Ŀ��ʹ�õ�һ����������
//;===========================================================


#ifndef _Typedef_H
#define _Typedef_H

typedef     unsigned char           U8;
typedef     unsigned short int      U16;
typedef     unsigned long  int      U32;

typedef     signed   char           S8;
typedef     signed   short int      S16;
typedef     signed   long  int      S32;
typedef              bit            BOOL;

#if 1
typedef     unsigned char           uint8_t;
typedef     unsigned short int      uint16_t;
typedef     unsigned long  int      uint32_t;

typedef     signed   char           int8_t;
typedef     signed   short int      int16_t;
typedef     signed   long  int      int32_t;
#endif

#ifndef TRUE
#define  TRUE       1
#endif

#ifndef FALSE
#define  FALSE      0
#endif

#define LOWBYTE(X)        ((U8)((X)&0xff))
#define HIGHBYTE(X)       ((U8)((X)>>8))
#define BSET(X,N)          (X)|=(1<<N)
#define BCLR(X,N)          (X)&=~(1<<N)
#define BTST(X,N)          (X)&(1<<N)
#define BCPL(X,N)          (X)^=(1<<N)
#define BSETEX(X,N)        X[(N)>>3]|=(1<<((N)&7))
#define BCLREX(X,N)        X[(N)>>3]&=~(1<<((N)&7))
#define LENOF(ARR)         ((U8)(sizeof(ARR)/sizeof(ARR[0])))

#define INCEX(X)           if(++X==0)X--
#define DECEX(X)           if(X!=0)X--

#define ABS(X)             ( (X < 0) ? (-X) : (X) )


#define BIG_ENDIAN         0

#if BIG_ENDIAN

typedef union
{
    U16 W;
    struct
    {
        U8 H;
        U8 L;
    } B;
} WORD;
typedef union
{
    U32 D;
    struct
    {
        U16 H;
        U16 L;
    } W;
    struct
    {
        U8 T;
        U8 M;
        U8 H;
        U8 L;
    } B;
} DWORD;

#else

typedef union
{
    U16 W;
    struct
    {
        U8 L;
        U8 H;
    } B;
} WORD;
typedef union
{
    U32 D;
    struct
    {
        U16 L;
        U16 H;
    } W;
    struct
    {
        U8 L;
        U8 H;
        U8 M;
        U8 T;
    } B;
} DWORD;

#endif

#endif
