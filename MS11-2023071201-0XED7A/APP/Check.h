#ifndef _Check_H
#define _Check_H

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef _Check_SRC
#define EXTERN
#else
#define EXTERN extern
#endif


EXTERN void SelfVerify(void);
EXTERN void DoorCheck( void );
EXTERN void FanSpeedCheck( void );
EXTERN void FanErrCheck( void );
#endif