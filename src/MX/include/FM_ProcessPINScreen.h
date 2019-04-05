#ifndef FM_PROCESSPINSCREEN_H_
#define FM_PROCESSPINSCREEN_H_

#include "../Main.h"
#include "AbstractScreen.h"
#include "../PP1000se.h"

class FM_ProcessPINScreen : public AbstractScreen
{
public:
//	FM_ProcessPINScreen();
	FM_ProcessPINScreen(string, DeviceRequest*, Wait*);
	virtual ~FM_ProcessPINScreen();
	
	// implementation of template method
    void init();
    
    // this is a template method that subclass can override if they
    // chose.  
    virtual void postInit();  
    
    // This method will handle notifications regarding PIN events
	void notify(string);
  
    // This method is a hook for subclasses that may need to provide some
    // cleanup after processing.  The default implementation will do
    // nothing.
    virtual void cleanup();
    
protected:
	short  minLength;
	short  maxLength;
	string pinData;
	int    timeout;  

};

#endif /*FM_PROCESSPINSCREEN_H_*/
