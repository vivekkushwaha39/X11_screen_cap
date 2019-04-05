#ifndef MAGSWIPE_H_
#define MAGSWIPE_H_

#define FALSE 0
#define TRUE 1

static void onMSR(unsigned char *pszMSRData, int iNumBytes);
int InitMagSwipe(void);
int disable_msr(void);
int enable_msr(void);
int parse_Track(unsigned char* msr_buf,int iLength);
void GoodSwipe_tone();
void BadSwipe_tone();
extern void getTrack2Data(unsigned char* msr_buf);
int cycle_led(void *pData);
#endif /*MAGSWIPE_H_*/
