#ifndef _GUIMGR_H_
#define _GUIMGR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gui.h"

#include <sig.h>                    // From SDK, for xyz_t struct definition

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define TOP     1
#define MIDDLE  2
#define BOTTOM  3
    
#define MAX_FORM_BG_VALUE_SIZE      MAX_FILENAME_LEN
#define MAX_PROP_NAME_SIZE          64
#define MAX_PROP_VALUE_SIZE         1024
#define MAX_LIST_ITEMTEXT_SIZE      64
#define MAX_TEXT_ITEMTEXT_SIZE      1024

#define STRIP_LEADING_SPACES        0x01
#define STRIP_TRAILING_SPACES       0x02

#define DISP_TXT_OPT_RESETX         0x01
#define DISP_TXT_OPT_IGNORESTRIP    0x02

// MessageBox icon types
#define MB_ICON_ERROR           1
#define MB_ICON_INFO            2
#define MB_ICON_QUERY           3
#define MB_ICON_WARNING         4

// MessageBox return values for blocked MessageBox calls, when button is pressed
#define MB_OK                   1
#define MB_CANCEL               2
#define MB_YES                  3
#define MB_NO                   4

// MessageBox button combinations
#define MSGBOX_NO_BUTTONS       0   // No buttons; messagebox must be dismissed programmatically by app using HideMsgBox()
#define MSGBOX_OK               1   // Only OK button
#define MSGBOX_OK_CANCEL        2   // OK and CANCEL buttons
#define MSGBOX_YES_NO           3   // YES and NO buttons
#define MSGBOX_YES_NO_CANCEL    4   // YES, NO and CANCEL buttons

// BOOL property types
#define PROP_BOOL_VISIBLE                   1
#define PROP_BOOL_BORDER_VISIBLE            2
#define PROP_BOOL_AUTO_SIZE                 3
#define PROP_BOOL_TRANSPARENT               4
#define PROP_BOOL_BEEP_ON_PRESS             5
#define PROP_BOOL_STAY_PRESSED              6
#define PROP_BOOL_SELECTED                  7
#define PROP_BOOL_BTN_VISUAL_EFFECT         8
#define PROP_BOOL_LISTBOX_NO_SCROLL         9
#define PROP_BOOL_ANIMATE_RUN               10
#define PROP_BOOL_CAPTION_AREA_ACTIVE       11  // For MX850/MX830
#define PROP_BOOL_VIDEO_REPEAT              12
#define PROP_BOOL_ENABLE                    13

// SHORT property types
#define PROP_SHORT_OPTIONS                  21
#define PROP_SHORT_TEXT_JUSTIFICATION       22
#define PROP_SHORT_FONT_ATTRIBUTES          23
#define PROP_SHORT_STYLE                    24
#define PROP_SHORT_ITEMS_TO_SCROLL          25
#define PROP_SHORT_MIN_ENTRIES              26
#define PROP_SHORT_KEYPAD_TYPE              27
#define PROP_SHORT_KEYPAD_IMAGE_POS_LEFT    28
#define PROP_SHORT_KEYPAD_IMAGE_POS_TOP     29
#define PROP_SHORT_BTN_STATE                30  // Button State: 0=Up/Normal, 1=Down/Pressed 
#define PROP_SHORT_ANIM_DELAY_INTERVAL      31
#define PROP_SHORT_ANIM_LOOP_COUNT          32
#define PROP_SHORT_ANIM_PRES_EFFECT         33
#define PROP_SHORT_BTN_RESET_STATE          34  // For resetting state of StayPressed buttons that have been pressed, to their 0=Up/Normal state
#define PROP_SHORT_VIDEO_MODE               35
#define PROP_SHORT_FONT_SIZE                36  // 10-Feb-09: change fontsize of control

// STRING property types
#define PROP_STR_CAPTION                    41  // For label, button, checkbox
#define PROP_STR_UP_IMAGE_FILE_NAME         42  // For button
#define PROP_STR_DOWN_IMAGE_FILE_NAME       43  // For button
#define PROP_STR_IMAGE_FILE_NAME            44  // For image
#define PROP_STR_UNCHECKED_FILE_NAME        45  // For checkbox
#define PROP_STR_CHECKED_FILE_NAME          46  // For checkbox
#define PROP_STR_DISPLAY_STRING             47  // For textbox
#define PROP_STR_FORMAT_STRING              48  // For textbox
#define PROP_STR_CUSTOM_IMAGE_FILE_NAME     49  // For keypad
#define PROP_STR_ANIM_FILE_NAME             50  // For animation
#define PROP_STR_MARQUEE_TEXT               51  // For marquee
#define PROP_STR_FORM_BACKGROUND            52  // For form
#define PROP_STR_VIDEO_FILE_NAME            53
#define PROP_STR_INPUT_STRING               54  // For keypad, editfield (entry pre-populating)
#define PROP_STR_FONT_NAME                  55  // 10-Feb-09: change fontname of control

// LONG property types
#define PROP_LONG_BG_COLOR                  81  // For label, button, listbox, line, box
#define PROP_LONG_FG_COLOR                  82  // For label, button, checkbox, listbox, keypad (textbox)
#define PROP_LONG_TIMEOUT                   83  // For keypad, editfield (entry timeout)

// Form Background Type
#define RGB_COLOR           1              // Form background is painted in specified RGB color
#define IMAGE_FILENAME      2              // Form background is specified image
#define IMAGE_THEMENAME     3              // Form background image file is obtained from theme file

enum SHOWFORM_PRESENTATION_MODE
{   
    PM_NORMAL           = 0x01,
    PM_LEFT_TO_RIGHT    = 0x02,
    PM_RIGHT_TO_LEFT    = 0x03,
    PM_TOP_DOWN         = 0x04,
    PM_BOTTOM_UP        = 0x05,
    PM_FADE             = 0x06             // New form fades in
};

// Initializes the GUI Manager library
BOOL InitGUIManager(char *pszFontFileTable);

// Returns the GUI Manager library version
BOOL GetGUIManagerVersion(char* pszVersion, int len);

// Returns name of currently executing application (with and without path)
void GetAppName(char *pszAppNameWithPath, char *pszAppName);

// Reload a new font file table
BOOL ReInitFontTable(char *pszFontFileTable);

// Initialize the given form and prepare it to display
BOOL InitForm(char* strFormName);

// Extended version of InitForm, with extra parameter bDisableFormAfterOneEvent indicating whether to 
// internally lock the form after the first event, to prevent further GUI events/callbacks.
BOOL InitFormEx(char* strFormName, BOOL bDisableFormAfterOneEvent);

// Shows the previously initialized form
BOOL ShowForm(enum SHOWFORM_PRESENTATION_MODE ePM);

// Clean up all on-screen controls in current form; also stop any sigcap and PINPad activity if any.
// Note: This clears up even the form's background; however the background "curtain" image (see SCREEN_BACKGROUND 
// setting in .profile) that is behind all forms will remain.
void ClearScreen(void);

// Retrieves values for the given control
BOOL GetBOOLValue(int iControlID, char cPropName, BOOL* pbPropValue);
BOOL GetShortValue(int iControlID, char cPropName, unsigned short* pusPropValue);
BOOL GetLongValue(int iControlID, char cPropName, unsigned long* pulPropValue);
BOOL GetStringValue(int iControlID, char cPropName, char* pcPropValue);

// Sets the given value to the specified control
BOOL SetBOOLValue(int iControlID, char cPropName, BOOL bPropValue);
BOOL SetShortValue(int iControlID, char cPropName, unsigned short sPropValue);
BOOL SetLongValue(int iControlID, char cPropName, unsigned long lPropValue);
BOOL SetStringValue(int iControlID, char cPropName, char* pcPropValue);

// Returns TRUE, if the specified form is loaded. Otherwise, FALSE
BOOL IsFormLoaded(/* in */ char *pszFormName);

// Returns the currently loaded form. Returns empty string, if no form is loaded
void GetLoadedForm(/* out */ char *pszFormName);

// Call this function to change the default screen background.
// Input: pszScreenBGFileName - image filename to use for the screen background.
//        The filename MUST be a png or jpg file (extension must be ".png" or ".jpg")
// NOTE: 
// The screen background is not to be confused with a form's background; it is *behind* any form's background.
// If the specified image file is not found or is invalid, this function will silently
// default to creating an all-black rectangle screen background.
void SetScreenBackGround(char *pszScreenBGFileName);

// Clears the contents of the list box
// Returns:
//     <0 : failure (invalid ControlID, etc.)
//      1 : success
short ClearListbox(int iControlID);

// Adds the given itemtext at the given index.
// if iIndex is 0, then the item is added to the top of the list box.
// if iIndex is -1, then the item is added to the bottom of the list box.
// NOTE: ListBox indexes are 0-based
// Returns:
//     <0 : failure
//      1 : success
short AddListboxItem(int iControlID, int iIndex, char *pszItemText);

// Removes the item at the specified index 
// Returns:
//     <0 : failure (invalid ControlID, invalid index, etc.)
//      1 : success
// NOTE: ListBox indexes are 0-based
short RemoveListboxItem(int iControlID, int iIndex);

// Retrieves the item text at the specified index
// Returns:
//     <0 : failure (invalid ControlID, invalid index, etc.)
//      1 : success
// NOTE: ListBox indexes are 0-based
short GetListboxItem(int iControlID, int iIndex, char *pszItemText);

// Returns the item count in the list box
// Returns:
//     <0 : failure (invalid ControlID, etc.)
//      1 : success
short GetListboxItemCount(int iControlID);

// Returns the currently selected index, if any
// Returns:
//     <0 : failure (invalid ControlID, etc.)
//      1 : success
// NOTE: ListBox indexes are 0-based
short GetListboxSelectedItem(int iControlID);

// Sets the given index to the top of the list
// Returns:
//     <0 : failure (invalid ControlID, invalid index, etc.)
//      1 : success
// NOTE: ListBox indexes are 0-based
short SetListboxTopItem(int iControlID, int iIndex);

// Programmatically selects/de-selects the given index in the listbox
// if iIndex is -1, then the LAST item in the list is selected/de-selected.
// Returns:
//     <0 : failure (invalid ControlID, invalid index, etc.)
//      1 : success
// NOTE: ListBox indexes are 0-based
short SelectListboxItem(int iControlID, int iIndex, BOOL bSelect);

// Adds (appends) the given itemtext to the text box.
// Use ClearListbox() to clear previous contents if required
// or ReplaceTextboxItem() to replace textbox contents.
// Note: ReplaceTextboxItem() will refresh the contents of the textbox without any flicker.
// Returns:
//     <0 : failure
//      1 : success
short AddTextboxItem(short iControlID, char *pszItemText);

// Replace text in the textbox (if any) with the given itemtext.
// Returns:
//     <0 : failure
//      1 : success
short ReplaceTextboxItem(short iControlID, char *pszItemText);

// Display a popup messagebox on top of the existing form.
// Parameters:
// pszTitle - string title of messagebox
// pszText - text to be displayed in messagebox
// icon - one of the defined MB_ICON_xxx above
// buttons - valid values: MSGBOX_OK, MSGBOX_OK_CANCEL, MSGBOX_YES_NO, MSGBOX_YES_NO_CANCEL
// bBlocked - indicates if the messagebox is blocked till one of the buttons is pressed or not.
// for bBlocked=TRUE: retval indicates which messagebox button was pressed {MK_OK | MB_CANCEL | MB_YES | MB_NO}
//    The p_fnMSGBOXCallBack parameter is ignored; 'button' parameter cannot be MSGBOX_NO_BUTTONS (if
//    MSGBOX_NO_BUTTONS is supplied, the code will default to MB_OK)
// for bBlocked=FALSE:
//    button parameter *may be* MSGBOX_NO_BUTTONS
//    The p_fnMSGBOXCallBack parameter is used to return which messagebox button was pressed to the caller
//    in the iButtonPressed parameter, if any button was pressed (before the dialog was programmatically
//    dismissed). Otherwise the dialog must be dismissed programmatically by the app, by calling HideMsgBox().
//
typedef void (*p_fnMSGBOXCallBack)(int iButtonPressed);
short MsgBox(char *pszTitle, char *pszText, short icon, short buttons, BOOL bBlocked, p_fnMSGBOXCallBack pCallback);

// Hides the previously shown message box.
// This should be called only for an *unblocked* message box *without* any buttons,
// (i.e., MsgBox() called with bBlocked=FALSE and buttons=MSGBOX_NO_BUTTONS)
short HideMsgBox(void);

// Returns TRUE, if a message box is present. Otherwise, FALSE
BOOL IsMsgBoxPresent(void);

/* EVENTS: */
/*
** All events match one of the following function signatures:
** where pData maps to one of the xxxEVENTDATA typedefs in gui.h.

There is at most one event handler for each control. If a control has more than one event, 
the iEventType field will identify the event type.

Example 1:
For a Button event, pData maps to BUTTONEVENTDATA, where:
BUTTONEVENTDATA.mouse_event.canvas.x = x coordinate of the button click
BUTTONEVENTDATA.mouse_event.canvas.y = y coordinate of the button click.

Example 2:
For a ListBox item-select event, pData maps to LISTBOXEVENTDATA, where:
LISTBOXEVENTDATA.iEventType = ON_SELECT
LISTBOXEVENTDATA.iIndex = < index of item selected >

For a ListBox scroll-down event, pData maps to LISTBOXEVENTDATA, where:
LISTBOXEVENTDATA.iEventType = ON_SCROLL_DOWN
LISTBOXEVENTDATA.iIndex = < not applicable >

Example 3:
For an Image event, pData maps to EVASEVENTDATA, where:
EVASEVENTDATA.event_info.canvas.x = x coordinate of the image click
EVASEVENTDATA.event_info.canvas.y = y coordinate of the image click.
*/
typedef void (*Event_Callback_Function)(char *pszFormName, int iControlID, void *pData);
typedef void (*BUTTON_CALLBACK_FN)(void *data, Evas_Object *button, Evas_Event_Mouse_Up *mouse_event);
typedef void (*CHECKBOX_CALLBACK_FN)(Evas_Object *obj, void *data);
typedef void (*EVAS_CALLBACK_FN)(void *data, Evas *e, Evas_Object *obj, void *event_info);
typedef void (*LIST_ITEMSEL_CALLBACK_FN)(Evas_Object *obj, int row, int col, void *data);
typedef void (*LIST_SCROLLUP_CALLBACK_FN)(Evas_Object *obj, void *data);
typedef void (*LIST_SCROLLDOWN_CALLBACK_FN)(Evas_Object *obj, void *data);

// Hook function for program to route all callbacks to itself
typedef void (*GUITrapHook)(char *pszFormName, int iControlType, int iControlID, void *pData, int iCBIndex);
void SetTrapHook(GUITrapHook fn);

// Return pointer to control data. Caller is expected to cast from void* to appropriate type.
// Returns:
//    NULL: failure (invalid ControlID, etc.)
//  !NULL : success; pointer to valid control struct (e.g., Label*, Button*, etc. -- see gui.h for control definitions)
// Use this function if you know both the ControlID and ControlType.
void* GetControlData(int iControlID, char chControlType);

// Return pointer to control data and control type in pchControlType. 
// Caller is expected to cast from void* to appropriate type (defined in gui.h).
// Returns:
//    NULL: failure (invalid ControlID, etc.)
//  !NULL : success; 
//          retval is pointer to valid control struct (e.g., Label*, Button*, etc. -- see gui.h for control definitions)
//          pchControlType contains control type CTRL_TYPE_xxx (see gui.h)
// Use this function if you know only the ControlID.
// This function can be used to do a reverse lookup on all controls in the form.
void* GetControlDataEx(int iControlID, char *pchControlType);

// Returns the number of controls for the currently loaded form.
// Return values:
// <0  : failure
// >= 0: success
// NOTE: it is OK for a form to have 0 controls (e.g., a form with only a form background)
int iGetNumControls(void);

/*
** Display long text buffer in specified font and position.
** 
** Inputs:
**  pszBuffer[] - NULL-terminated string
**  X - x position to begin writing text
**  Y - y position to begin writing text
**  iFontIndex - valid index into font file GUIFONTS.TBL
**  iPixelWrapHeight - height in pixels at which to stop writing text
**  iPixelWrapWidth  - width in pixels at which to wrap around and start writing the next line
**  iOptionsMask - 2 options presently available 
**  This is a mask value representing the decimal value of the SUM of the options.
**  Bit value 0 implies the option is OFF, value 1 implies the option is ON.
**  Bit position        Meaning
**            0         Reset X coordinate to 0 after wrapping
**            1         Do not strip leading blanks after wrap  
**  - Reset X coordinate to 0 after wrapping
**          (if this option bit is set, the X at which to write a new line is reset to the zeroth position on the display)
**          (if this option bit is not set, the X at which to write a new line is the same as
**           the first X position parameter).
**  - Do not strip leading blanks after wrap
**          (if this option bit is set, does not auto strip spaces)
**          (if this option bit is not set, formats and strips spaces 'intelligently')
** 
**  piNewX - New value of X position after writing the text
**  piNewY - New value of Y position after writing the text
**  
** The pszBuffer parameter can contain embedded directives for turning on bold, italics, underline, 
** reverse-video and strike-through attributes for sections of the text. You can intersperse font 
** attributes: 
** - for bold, <B> will turn ON the bold attribute, the next <B> will turn it OFF
** - for italics, <I> will turn ON the italics attribute, the next <I> will turn it OFF
** - for underline, <U> will turn ON the underline attribute, the next <U> will turn it OFF
** - for reverse-video, <R> will turn ON the reverse-video attribute, the next <R> will turn it OFF
** - for strike-through, <S> will turn ON the strike-through attribute, the next <S> will turn it OFF
** The effects of an attribute once turned on remain in effect until turned off.
** 
** Return values:
** <=0 : Error
**  >0 : Number of lines written
*/
int iDisplayText(char *pszBuffer, unsigned int X, unsigned int Y, 
                 char *pszNormalFontName, char *pszBoldFontName, 
                 char *pszItalicsFontName, char *pszBoldItalicsFontName,
                 int iFontSize, long lFGColor, long lBGColor, long lSelColor,
                 unsigned int iPixelWrapHeight, unsigned int iPixelWrapWidth,
                 unsigned int iOptions, int *piNewX, int *piNewY);

/* 
** Function returns the control data for checkboxes only in the present form 
** that is displayed, in the format:
** ControlID<FS>FieldType<FS>DATA<FS> 
** where FieldType is "BOOL" (for Checkboxes), and 
** DATA = '0' if checkbox is not checked,
** DATA = '1' if checkbox is checked
*/
void GetControlDataFromForm(char* pszControlData); 

/* 
** Function returns the control coordinates(x1,y1,x2,y2) of the specified iControlID 
*/
BOOL GetControlCoordinates(int iControlID, unsigned short *x1, unsigned short *y1, unsigned short *x2, unsigned short *y2);

// Function sets the control coordinates(x1,y1,x2,y2) of the specified iControlID.
// Note: 
// - for Line controls, 'x2' and 'y2' represent the end point of the line
// - for other controls, the width is (x2-x1), and height is (y2-y1); ensure that (x2 > x1) and (y2 > y1)
BOOL SetControlCoordinates(int iControlID, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);

// Function to get the Font Index in the GUIFONTS.TBL file if presented with pszFontName
int GetFontIndex(char* pszFontName);

// Extract fontname from font table, given a font index
BOOL GetFontName(int iFontIndex, char *pszFontName);

void GetRGBValues(ULONG rgbaColor, int *pr, int *pg, int *pb);
void GetRGBAValues(ULONG rgbaColor, int *pr, int *pg, int *pb, int *pa);
int  iGetTextWidthAndHeight(char *pszText, int iFontSize, char *pszFontName, int *piTextWidth, int *piTextHeight);

typedef struct tagFontRecord
{
    char szFontFileName[MAX_FILENAME_LEN+1];
} __attribute__ ((packed)) FONT_RECORD;

int GetScreenshot(char *pszFileName, int X, int Y, int iWidth, int iHeight);

// PIN entry functions -- begin
typedef struct tagIPP_CALLBACK_DATA
{
    int  iStatus;               // <0 => Error (see #defines below); >0 => Success; # of valid bytes in szResponsePkt[] below
    char szResponsePkt[256+1];  // Response packet from IPP module for iStatus>0
} __attribute__ ((packed)) IPP_CALLBACK_DATA;

typedef int (*p_fnPINCallBack)(IPP_CALLBACK_DATA *pData);   // Callback signature for PINEntry

// #defines for iKeyMgmt parameter of GetPINEntry() function
#define KM_1DES_MS          0x00    // 1DES Master/Session (non-GISKE) key
#define KM_1DES_DUKPT_0     0x01    // 1DES DUKPT - with DUKPT engine 0
#define KM_1DES_DUKPT_1     0x11    // 1DES DUKPT - with DUKPT engine 1
#define KM_1DES_DUKPT_2     0x21    // 1DES DUKPT - with DUKPT engine 2
#define KM_3DES_MS          0x02    // 3DES Master/Session GISKE data block
#define KM_3DES_DUKPT_0     0x03    // 3DES DUKPT - with DUKPT engine 0
#define KM_3DES_DUKPT_1     0x13    // 3DES DUKPT - with DUKPT engine 1
#define KM_3DES_DUKPT_2     0x23    // 3DES DUKPT - with DUKPT engine 2
#define KM_VSS              0xF0    // 28-Aug-06: PIN entry will use VeriShield Security Scripts mechanism

// Return values:
//  <0: Error
// >=0: Success
int GetPINEntry(int iKeyMgmt, int iMinPINLen, int iMaxPINLen, 
                BOOL bNullPinAllowed, int iMaxTimeInSeconds,
                char *szPAN, 
                char *szWKey, // Ignored in DUKPT
                int iMasterKeyIndex, // valid range: 0x30-0x39 for Master/Session; ignored in DUKPT 
                p_fnPINCallBack pCallback);

// Return values:
//  <0: Error
// >=0: Success
// NOTE: when using this function, the keypad type used MUST be a custom keypad.
int GetPINEntryEx(int iKeyMgmt, int iMinPINLen, int iMaxPINLen, 
                BOOL bNullPinAllowed, int iMaxTimeInSeconds,
                char *szPAN, 
                char *szWKey, // Ignored in DUKPT
                int iMasterKeyIndex, // valid range: 0x30-0x39 for Master/Session; ignored in DUKPT
                p_fnPINCallBack pCallback, 
                char *pszKbdNormalImageAfterPINStart, 
                char *pszKbdPressedImageAfterPINStart);

void CancelPINEntry(void);

// GetPINEntry() return codes
#define PIN_ENTRY_SUCCESS   1       // 28-Aug-06: added; IPP_CALLBACK_DATA.szResponsePkt[] = PIN block data
#define IPP_IDLE            0
#define GET_PIN1            -1
#define GET_PIN2            -2      // Too many PIN sessions requested in a short period of time. (Try again in few seconds)
#define SENDING_PIN         -3
#define ENCRYPTING          -4
#define REC_ENCRYPT         -5
#define ABORT_BY_TIME       -6
#define ABORT_BY_PROG       -7
#define ABORT_BY_KEY        -8
#define ABORT_WITH_DATA     -9
#define IPP_COM_ERROR       -10
#define FAILURE_TO_READ     -11
#define NO_PIN_ENTERED      -12
#define MIN_PIN_ERROR       -13     // 09-Jan-06: Minimum # of PIN digits is less than 4; IPP_CALLBACK_DATA.szResponsePkt[] = # of PIN digits entered
#define PIN_ENTRY_FEEDBACK  -14     // 09-Oct-09: for PIN session feedback callbacks

#define IPP_SETUP_ERROR     -99

#define PIN_TIMEOUT     60      // In seconds 
// PIN entry functions -- end

// SigCap functions -- begin
typedef struct tagSIGCAP_CALLBACK_DATA
{
    int  iStatus; // Various status values (see sigcap.h); 
                  // For success values (SIGCAP_DONE_KEY, SIGCAP_DONE_TO, SIGCAP_ABORT_MEM, SIGCAP_DATA), the
                  // file <SIG_PARAM_BLOCK.szFileName> contains the sigcap points in specified format.
    int  iNumPoints;          // Only valid for iStatus==SIGCAP_DATA; number of xyz_t points in *pstSigCapRTPoints
    xyz_t *pstSigCapRTPoints; // Only valid for iStatus==SIGCAP_DATA; actual realtime sigcap points
} __attribute__ ((packed)) SIGCAP_CALLBACK_DATA;

typedef int (*p_fnSIGCAPCallBack)(SIGCAP_CALLBACK_DATA *pData); // Callback signature for SigCap

typedef struct tagSIG_PARAM_BLOCK
{
    short shStartTO;        /* Maximum time to wait for user to begin signing.  
                             * Specified in seconds. Set to 0 for no timeout.
                             */
    short shDoneTO;         /* Amount of time to wait after a user completes signing (pen up). 
                             * Specified in seconds. Set to 0 for no timeout.
                             */
    short shDoneKey;        /* HotSpotID that specifies the key the user can press when they have completed signing.
                             * *** NOTE ***: Current form for sigcap MUST have DONE button/FnKeyBtn.
                             */
    short shAbortKey;       /* HotSpotID that specifies the key the user can press to abort a signature capture.
                             * *** NOTE ***: Current form for sigcap MUST have CANCEL button/FnKeyBtn.
                             */
    short shOverallTimeout; /* Overall signing timeout */
    short shSaveMode;       /* Type of file saved: 
                             * SIGCAP_RAW_FILE     - 0 = VFI RAW file format
                             * SIGCAP_TIF_nnnDPI   - 10, 20, 30, 40 = Saves a TIF File in the required DPI (nnn = 100/200/300/400)
                             * SIGCAP_3BA_FILE     - 50 = 3 Byte ASCII file format
                             * SIGCAP_BMP_FILE     - 60 = Windows BMP file format
                             */
    char szFileName[32];    /* Filename for the saved file. If this param is NULL than
                             * a default file name of SIG.PTS will be used for raw files.
                             */
    short X1, Y1, X2, Y2;   /* Signing box area */
} __attribute__ ((packed)) SIG_PARAM_BLOCK;

// SIGCAP_CALLBACK_DATA.iStatus values
#define SIGCAP_IDLE           1     // Signature session idle (i.e., not yet initiated)
#define SIGCAP_WAIT_PEN_DOWN  2     // Signature capture waiting for the first pen down detect.
#define SIGCAP_ON             3     // Signature capture in progress.
#define SIGCAP_DONE_KEY       4     // Signature capture ended due to DONE keypress.
                                    // Maps to SIG_CAPTURE_COMPLETE 
                                    // Normal completion. The user made at least one stroke within the allotted time
#define SIGCAP_DONE_TO        5     // Signature capture ended due to idle time out after pen down detected. 
                                    // Maps to SIG_CAPTURE_TIMEOUT 
                                    // The user took too long to complete their signature. It was most likely truncated
#define SIGCAP_ABORT_KEY      6     // Signature capture abort due to CANCEL keypress (and there is no signature data)
#define SIGCAP_ABORT_TO       7     // Abort due to time out before pen down detected
                                    // Maps to SIG_CAPTURE_NOSIGN
                                    // User never started signing. No input was detected during the signing period.
#define SIGCAP_ABORT_APP      8     // Signature capture abort due to application request, i.e., app called CancelSigCapture()
#define SIGCAP_ABORT_MEM      9     // Signature capture abort due to memory exhaustion. 
                                    // Maps to SIG_CAPTURE_MAX_MEM - memory limit reached. 
                                    // The user signed for so long and so much that the maximum amount of 
                                    // memory for storing the data was reached.
#define SIGCAP_INIT_FAILURE  10     // Failed to initialize signature capture session properly;
                                    // all prerequisite conditions not met (e.g., Current form for sigcap
                                    // does not have DONE and/or CANCEL buttons with required HotSpotIDs; 
                                    // failed to create the vf_sigcap widget, ...)
#define SIGCAP_RESET_SESSION 99     // User pressed CANCEL key AFTER starting to sign.
                                    // *** NOTE ***: 
                                    // This automatically re-initiates a signature capture after notifying the app
                                    // with a return code of 99. The application should not call InitSigCapture() 
                                    // again to start a new signature for this return code.
#define SIGCAP_NO_MEM       100     // Signature capture cannot start due to a lack of memory or unable to create sigcap object
#define SIGCAP_DATA          21     // Realtime sigcap data supplied in callback

// SIG_PARAM_BLOCK.shSaveMode values
#define SIGCAP_RAW_FILE     0      // VeriFone raw sigcap format, *derived* from vf_sigcap widget
#define SIGCAP_TIF_100DPI   10     // TIFF 100 DPI
#define SIGCAP_TIF_200DPI   20     // TIFF 200 DPI
#define SIGCAP_TIF_300DPI   30     // TIFF 300 DPI
#define SIGCAP_TIF_400DPI   40     // TIFF 400 DPI
#define SIGCAP_3BA_FILE     50     // 3-Byte ASCII format
#define SIGCAP_BMP_FILE     60     // Windows BMP format
#define SIGCAP_NCR_FILE     70     // NCR signature format
#define SIGCAP_PIX_FILE     80     // FST-level pixel signature format (same as SIGCAP_RAW_FILE, except points are in pixels, not scaled)
#define SIGCAP_TP_RAW_FILE  90     // VeriFone raw sigcap format, from raw touchpanel coordinate system (more points in 2048 x 1536 touchpanel coordinate system)
#define SIGCAP_REALTIME_DATA 'R'   // VeriFone realtime sigcap (in points; each point is 4 bytes in VeriFone raw XXYY format)

// Function to initiate signature capture
// Return values:
//  0: Success
// <0: Failure 
//    -1: we're already in the middle of a sigcap session
//    -2: no callback specified
//    -3: invalid sigcap savemode
//    -4: current form does NOT contain DONE button and/or CANCEL BUTTON for SIGCAP entry
int InitSigCapture(SIG_PARAM_BLOCK *pSigParams, p_fnSIGCAPCallBack pCallback);

// Function to cancel signature capture
// Return values:
//  0: Success
// <0: Failure 
int CancelSigCapture(void);

// Function to programmatically terminate sigcap IF it has already begun.
// NOTE: 
// - the pre-condition is that the sigcap MUST be in the SIGCAP_ON state
// - the application will receive NOT receive a SIGCAP_DONE_KEY callback
// Return values:
//  0: Success
// <0: Failure 
int CompleteSigCapture(void);

// SigCap functions -- end

// Callback signature for notifying app that touchpanel was touched after backlight was turned off
typedef void (*p_fnTouchPanelCallBack)(void); 

// Function to turn OFF touchpanel display
// Return values:
// <= 0: Failure
//    1: Success
int TurnOffDisplay(p_fnTouchPanelCallBack pCallback);

// Function to turn touchpanel display to specified backlight value
// 'iNewBackLightVal' needs to be lower than the current *backlight setting
// Return values:
// <= 0: Failure: 'iNewBackLightVal' is not lower than the current *backlight setting; pCallback is NULL, etc.
//    1: Success
int TurnDownDisplay(int iNewBackLightVal, p_fnTouchPanelCallBack pCallback);

// Function to turn ON touchpanel display (and restore backlight it to previous brightness level)
// Failure (-1) => touchpanel display is NOT off
// Success  (1) => touchpanel display is restored
int TurnOnDisplay(void);

// ////////// Timer functions -- begin //////////
// Timer callback function.
// 'void *pUserData' is app-supplied data that was passed in the CreateTimer() call.
// Return values:
//    1: Keeps timer alive (i.e., it will automatically be rescheduled for previously specified interval)
// != 1: Cancel timer
typedef int (*p_fnTIMERCallBack)(void *pUserData);  // Callback signature for Timer

// Creates a timer to call the given timer callback function in the specified period of time.
// 'void *pUserData' is optional app data that can be passed in, and it will be
// made available to the callback function when the callback is made. Please note that it is the 
// app's responsibility to make sure this data is global or allocated from the heap, 
// and freed (if previously allocated) when needed no more [i.e., when timer is cancelled implicitly
// via the timer callback return value -- see p_fnTIMERCallBack definition, 
// or explicitly via the CancelTimer() call].
// Return values:
//  <0: Failure
// >=0: Success; TimerID -- ID of timer for use in the ChangeTimerInterval and CancelTimer() calls
int CreateTimer(double dblTimerIntervalInSeconds, p_fnTIMERCallBack pCallback, void *pUserData);

// Change a timer's interval.
// Return values:
// <0: Failure; invalid TimerID
// >0: Success; timer interval of specified timer was modified
int ChangeTimerInterval(int iTimerID, double dblTimerIntervalInSeconds);

// Cancels a previously created timer.
// Return values:
// <0: Failure; invalid TimerID
// >0: Success; specified timer was cancelled
int CancelTimer(int iTimerID);

// ////////// Timer functions -- end //////////

// Play the specified media file
// Parameters:
// pszAudioFileName - name of audio file to play; 
//    must be of type .WAV ONLY; other types are not supported
// BOOL bBlocked - indicates if the function is blocked till the sound file plays to completion or not.
//     TRUE  => call will be blocked till audio file plays till completion.
//              the pCallback parameter is ignored (and may be NULL).
//     FALSE => call will return immediately after initiating audio file to begin playing.
//              pCallback is optional (may be NULL), and specifies the name of the callback 
//              function to call when the audio file has played to completion.
// NOTE:
// Only 1 audio file can be playing at any given time. So, for example, if the 
// caller makes a first call to PlayAudio() that is not blocked, and makes another 
// call soon after that *before* receiving the p_fnSOUNDCallBack callback function 
// when the first audio file completes, the second call to PlayAudio() will be blocked.
typedef void (*p_fnAUDIOCallBack)(char *pszAudioFileName);
int PlayAudio(char *pszAudioFileName, BOOL bBlocked, p_fnAUDIOCallBack pCallback);

// Stop all threads and restart specified app
int GuiMgrRestartApp(char *pszFileName);

// Plays wav file specified in theme file key 'normal-beep'
// Return values:
//  1: Success
// -1: Failure: Normal beeper object not created (missing 'normal-beep' section in metrics, etc.)
int iNormalBeep(void);

// Plays wav file specified in theme file key 'error-beep'
// Return values:
//  1: Success
// -1: Failure: Error beeper object not created (missing 'error-beep' section in metrics, etc.)
int iErrorBeep(void);

/*
** Display image using specified image filename at specified [x,y] location.
** - pszImageFileName MUST be a png or jpg file with a '.png' or '.jpg' extension.
** - iWidth and iHeight are optional, i.e., can be set to 0 to signify that the image's true 
**   dimensions be used from the image file (provided iOptions is not set to 0x0004)
** - Valid iOptions values:
**   OPT_IMAGE_SCALE - scale image to specified dimensions; 
**      NOTE: non-zero iWidth and iHeight parameters must be supplied if this option is set
**            otherwise no scaling will be performed; the image will be displayed with its true size
** Return values:
** <=0 : Error
**  >0 : Success
*/
int iDisplayImage(unsigned int X, unsigned int Y, char *pszImageFileName, int iWidth, int iHeight, int iOptions);

// bEnable=TRUE  -> allow form input
// bEnable=FALSE -> disallow form input
BOOL bEnableForm(BOOL bEnable);

/*
** Returns if form is currently enabled or not.
** Return values:
**  TRUE: form is enabled
** FALSE: form is NOT enabled
*/
BOOL bFormEnabled(void);

/*
** Display a running clock display in the specified Label control.
** Supported DateTime formats for the 'pszDateTimeFormat' parameter are same as Linux strftime() function.
** See standard Linux documentation for more information.
** e.g., iRetVal = DisplayClockLabel(WELCOME_LBL_CLOCK, "%a %d-%b-%Y %I:%M:%S %p", 1.0);
** Return values:
** <=0 : Error
**  >0 : Success
* */
int DisplayClockLabel(int iLabelControlID,              // ControlID MUST belong to Label control 
                      char *pszDateTimeFormat,          // Use same format as Linux strftime() function
                      double dblUpdateFreqInSeconds);   // Update frequence in seconds

//
// Request guimgr thread callback (so that app can create raw Evas/FP objects) -- begin
// Note: 
// - Only one guimgr thread callback is allowed, per app.
// - If all is OK, the callback function is called with reason=GUIMGR_THREAD_CB_REASON_CALLBACK. 
// - The app-supplied callback function is saved till the current form is destroyed, at which point
//   the callback function is called for one last time with the reason=GUIMGR_THREAD_CB_REASON_FORMDEL. 
//   This gives the app the opportunity to clean up any raw Evas/FP objects it created when the callback 
//   was first called (with reason=GUIMGR_THREAD_CB_REASON_CALLBACK).
// - ### USE THIS FEATURE WITH EXTREME CAUTION!!!
enum guimgr_thread_callback_reason
{
    // Callback called due to callback request
    GUIMGR_THREAD_CB_REASON_CALLBACK = 1,

    // Callback called when current form is being destroyed 
    // (so any raw Evas/FP objects can be cleaned up if required)
    GUIMGR_THREAD_CB_REASON_FORMDEL  = 2
};
typedef int (*p_fnGuiMgrThreadCallBack)(enum guimgr_thread_callback_reason eReasonCode, void *pUserData);  // Callback function signature for GuiMgrThreadCallback
int GetGuiMgrThreadCallback(p_fnGuiMgrThreadCallBack callback, void *pUserData);
// Request guimgr thread callback (so that app can create raw Evas/FP objects) -- end

int SetFocus(int iControlID, BOOL bFocus);

// Key beeping control flags for hard keypad
#define KEY_REGION_FUNCTIONKEYS         0x01
#define KEY_REGION_CANCEL_CLEAR_ENTER   0x02
#define KEY_REGION_NUMKEYPAD            0x04

#define KEY_REGION_ALL_HARDKEYS        (KEY_REGION_FUNCTIONKEYS | KEY_REGION_CANCEL_CLEAR_ENTER | KEY_REGION_NUMKEYPAD)

// Function to control key beeps
// chKeyBeepMask - mask to be set with KEY_REGION_xxx values above
// If the bit value is ON, the beep from that region is allowed; if it is OFF, it is not.
// Unused bit flag positions are reserved for future use.
// E.g., 
// - to turn OFF all key beeps: SetKeyBeepFlags(0x00)
// - to turn ON function key beeps: 
//      SetKeyBeepFlags(KEY_REGION_FUNCTIONKEYS)
// - to turn ON function key & CANCEL/CLEAR/ENTER beeps: 
//      SetKeyBeepFlags(KEY_REGION_FUNCTIONKEYS | KEY_REGION_CANCEL_CLEAR_ENTER)
// - to turn ON beeps for all hard keys: 
//      SetKeyBeepFlags(KEY_REGION_FUNCTIONKEYS | KEY_REGION_CANCEL_CLEAR_ENTER | KEY_REGION_NUMKEYPAD)
//      or
//      SetKeyBeepFlags(KEY_REGION_ALL_HARDKEYS)
// - to preserve existing key beep status and incrementally add a flag:
//      - call GetKeyBeepFlags() to get current mask
//      - then call SetKeyBeepFlags() and 'OR' the new mask value with the current mask obtained above
void SetKeyBeepFlags(unsigned char uchKeyBeepMask);

// Function to get current key beep control value
void GetKeyBeepFlags(unsigned char *puchKeyBeepMask);

// Register app-defined callback function to be called on shutdown
typedef void (*p_fnAppShutdownCallBack)(int iSignal); // Callback prototype for app shutdown
void RegisterAppShutDownCallback(p_fnAppShutdownCallBack pCallBack);

// Get RFS info in format: RFS={RFSValue}<FS>RFSDATE={RFSDATE}<FS>SVCPAK={S}<FS>PACKAGES={P}
// where:
// {S} = semicolon-separated list of all svcpaks installed on device
// {P} = semicolon-separated list of all packages installed on device (e.g., ssl)
// Sample output: "RFS=RFS00016<FS>RFSDATE=03/29/07<FS>SVCPAK=svcpak-016-04.01<FS>PACKAGES=openssl-0.9.8b-vfi.01;vfissh-01.02;vpsec-01.02"
int GuiMgrGetRFSDetails(char *pszRFSInfo, int iBuffSize);

typedef enum tagKEY_MGMT_TYPE
{
    KMT_MS_1DES             = 0,
    KMT_MS_3DES             = 1,
    KMT_MS_MIXED_1DES_3DES  = 2,    // Mixed Mode 1DES Master/Session & 3DES GISKE
    KMT_DUKPT_1DES          = 3,
    KMT_DUKPT_3DES          = 4
} KEY_MGMT_TYPE;

#define MAX_KSN_SIZE        20
#define MAX_PIN_KEYS        (10+3)  // 10 slots for Master/Session, 3 slots for DUKPT
typedef struct tagPINKeyStatus
{
    BOOL bKeyPresent;               
    int iKeyIndex;                  // Slot# 0-9 for MasterSession, Engine# 0-2 for DUKPT
    KEY_MGMT_TYPE eKMType;
    char *pszKSN;                   // Key Serial Number; only valid for DUKPT!, null-terminated; max size is MAX_KSN_SIZE
} PIN_KEY_STATUS;

typedef struct tagAllPINKeyStatus
{
    PIN_KEY_STATUS stPKS[MAX_PIN_KEYS];
} ALL_PIN_KEY_STATUS;

extern ALL_PIN_KEY_STATUS m_stALL_PIN_KEY_STATUS;

BOOL bGetPINKeyStatus(ALL_PIN_KEY_STATUS *pstALL_PIN_KEY_STATUS);

// Creates a timer to call the given app-supplied callback function when there is no touchpanel activity
// within the specified period of time.
// 'void *pUserData' is optional app data that can be passed in, and it will be
// made available to the callback function when the callback is made. Please note that it is the 
// app's responsibility to make sure this data is global or allocated from the heap, 
// and freed (if previously allocated) when needed no more [i.e., when timer is cancelled implicitly
// via the timer callback return value -- see p_fnTIMERCallBack definition,
// or explicitly via the CancelIdleTPTimer() call].
//
// NOTE:
// The application needs to cancel a previously created idle TP timer before creating a new one. 
//
// Return values:
//  <0: Failure
// >=0: Success; NOTE: this is NOT the ID of timer created as in a CreateTimer() call
int CreateIdleTPTimer(double dblTimerIntervalInSeconds, p_fnTIMERCallBack pCallback, void *pUserData);

// Cancels a previously created timer.
// Return values:
// <0: Failure; idle TP timer not previously created
// >0: Success; idle TP timer was cancelled
int CancelIdleTPTimer(void);

// 12-Jan-10: added - begin
// Linux environment variable MX760_INTERNAL_BUZZER = 0|1 indicates whether to use the 
// MX760 internal buzzer or not. The default value is 1 (i.e., use the internal buzzer).
// If the internal buzzer is not used, all beep sounds are emitted via the external 
// buzzer port (and would need an external buzzer to be connected to be heard).
// There are Linux environment variables defined for each internal buzzer sequence; 
// if not found, the lib uses predefined defaults.
// Each env var is of the form: 
// 
//     var=<buzzer-string>
// 
// where <buzzer-string> is <play-time>,<pause-time>,<play-time>,<pause-time>,...,<play-time>,<pause-time>
// i.e., alternating sequence of play and pause times; note also that these values need to be specified
// in pairs, i.e., each play time needs a corresponding pause time value (which may be 0 to signify no pause).
// All values are integer strings.
//
// These are the pre-defined defaults for each type of string:
// MX760_NORMAL_BEEP=100,0
// MX760_ERROR_BEEP=50,0,50,0,50,0,50,0,50,0
// MX760_GOOD_SWIPE=100,0,100,0,100,0
// MX760_BAD_SWIPE=100,50,500,0,100,0
// ----------------------------------------------
// API functions for MX760 *internal* buzzer
int MX760_NormalTone(void);
int MX760_ErrorTone(void);
int MX760_GoodSwipeTone(void);
int MX760_BadSwipeTone(void);
// 12-Jan-10: added - end

#ifdef SLIDE_EVENTS // 22-Feb-10
#define SLIDE_NONE      0
#define SLIDE_LEFT      1
#define SLIDE_RIGHT     2
#define SLIDE_UP        3
#define SLIDE_DOWN      4
typedef void (*p_fnSlideEventCallBack)(int iSlideDirection);
BOOL bEnableFormSlideEvents(BOOL bEnable, p_fnSlideEventCallBack pCallback);
#endif // SLIDE_EVENTS

#ifdef __cplusplus
}
#endif

#endif /* _GUIMGR_H_ */
