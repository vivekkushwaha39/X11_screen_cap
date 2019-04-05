#if !defined(_GUI_H)
#define _GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <evas.h>
#include "types.h"

#ifndef PACKED
#define PACKED packed
#endif

#define CTRL_TYPE_KEYPAD            0
#define CTRL_TYPE_LABEL             1
#define CTRL_TYPE_BUTTON            2
#define CTRL_TYPE_IMAGE             3
#define CTRL_TYPE_LINE              4
#define CTRL_TYPE_BOX               5
#define CTRL_TYPE_CHECKBOX          6
#define CTRL_TYPE_LISTBOX           7
#define CTRL_TYPE_ANIMATE           8
#define CTRL_TYPE_MARQUEE           9
#define CTRL_TYPE_TEXTBOX           10
#define CTRL_TYPE_RICHTEXT          11
#define CTRL_TYPE_FORM_BG           12
#define CTRL_TYPE_GROUPBOX          13
#define CTRL_TYPE_RADIOBUTTON       14
#define CTRL_TYPE_MSR               15
#define CTRL_TYPE_TIMER             16
#define CTRL_TYPE_VIDEO             17
#define CTRL_TYPE_EDITFIELD         18
#define CTRL_TYPE_FNKEYBTN          19
#define CTRL_TYPE_HARDKEYPAD        20

#ifdef SLIDE_EVENTS // 22-Feb-10
#define CTRL_TYPE_FORM              21
#endif // SLIDE_EVENTS

#define MAX_LINE_LEN            64
#define MAX_NAME_LEN            64
#define MAX_FILENAME_LEN        32
#define MAX_MARQUEE_LEN         256

// Returns the low byte of the word w.
#define LOBYTE(w)   ((BYTE)(w))

// Returns the high byte of the word w.
#define HIBYTE(w)   ((BYTE)(((WORD)(w) >> 8) & 0xFF))

// Makes a word from low byte and high byte.
#define MAKEWORD(low, high)   ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8)))

// Returns the low word of the double word l.
#define LOWORD(l)   ((WORD)(ULONG)(l))

// Returns the high word of the double word l.
#define HIWORD(l)   ((WORD)((((ULONG)(l)) >> 16) & 0xFFFF))

// Returns the low signed word of the double word l.
#define LOSWORD(l)   ((SWORD)(ULONG)(l))

// Returns the high signed word of the double word l.
#define HISWORD(l)   ((SWORD)((((ULONG)(l)) >> 16) & 0xFFFF))

// Makes a double word from low word and high word.
#define MAKELONG(low, high)   ((ULONG)(((WORD)(low)) | (((ULONG)((WORD)(high))) << 16)))

#define GetRValue(rgba)   ((BYTE)(rgba))                        // Gets the red component from an RGBA value
#define GetGValue(rgba)   ((BYTE)(((USHORT)(rgba)) >> 8))       // Gets the green component from an RGBA value
#define GetBValue(rgba)   ((BYTE)((rgba) >> 16))                // Gets the blue component from an RGBA value
#define GetAValue(rgba)   ((BYTE)((rgba) >> 24))                // Gets the alpha component from an RGBA value

// Makes a RGB triple value from red r, green g, and blue b components.
#define MakeRGB(r, g, b) (long)(((ULONG)((BYTE)(r))) | \
                                ((ULONG)((BYTE)(g)) << 8) | \
                                ((ULONG)((BYTE)(b)) << 16))

// Makes a RGBA value from red r, green g, blue b and alpha a components.
#define MakeRGBA(r, g, b, a) (long)(((ULONG)((BYTE)(r))) | \
                                    ((ULONG)((BYTE)(g)) << 8) | \
                                    ((ULONG)((BYTE)(b)) << 16) | \
                                    ((ULONG)((BYTE)(a)) << 24))

// Font Attributes
#define FONT_STANDARD      0x00
#define FONT_ITALICS       0x01
#define FONT_BOLD          0x02
#define FONT_UNDERLINE     0x04
#define FONT_REVERSE_TEXT  0x08             // Text between two <R> tags
#define FONT_STRIKE_THRU   0x10             // Text between two <S> tags

// draw() defines
#define FILL                0x01
#define VECTOR              0x02

// Text Justification
#define JUSTIFY_LEFT        0
#define JUSTIFY_CENTER      1
#define JUSTIFY_RIGHT       2

// State Definitions
#define UP                  0
#define DOWN                1

// Generic options bit definitions
#define OPT_VISIBLE                     0x0001  // 0 = not visible; 1 = visible
#define OPT_BORDER_VISIBLE              0x0002  // 0 = border not visible; 1 = border visible

// Label options
#define OPT_LABEL_RAW_WRAP              0x0100  // 03-Nov-09: Raw wrapping of text, i.e., without respecting word boundaries
#define OPT_LABEL_AUTO_FIT              0x0400  // 30-Mar-07: added: adjust fontsize downwards in attempt to fit entire caption
#define OPT_LABEL_AUTO_HEIGHT           0x0800  // 19-Mar-07: added
#define OPT_LABEL_STRIP_LEADING_BLANKS  0x1000
#define OPT_LABEL_AUTO_SIZE             0x2000
#define OPT_LABEL_AUTO_WRAP             0x4000
#define OPT_LABEL_TRANSPARENT           0x8000

// RadioButton options
#define OPT_RADIOBUTTON_TRANSPARENT     0x8000

// Button options
//#define OPT_BUTTON_AUTO_WRAP_CAPTION    0x0100  // 04-Apr-07: added; maps to OPT_LABEL_AUTO_WRAP with other flags
#define OPT_BUTTON_AUTO_FIT_CAPTION     0x0200  // 03-Apr-07: added: uses OPT_LABEL_AUTO_FIT to adjust
                                                //            fontsize downwards in attempt to fit entire caption
#define OPT_BUTTON_RADIOSTYLE           0x0400  // 28-Mar-07: 'radiobutton_normal' and 'radiobutton_pressed' keys
                                                //            in theme indicate radiobutton image files to use
#define OPT_BUTTON_ENABLED              0x0800  // 16-Oct-06: added; set from Button::vf_button_enable/vf_button_disable
#define OPT_BUTTON_ACTIVE_WHEN_INVISIBLE 0x1000  // 12-Jan-07: added: only for FNKEYBTN controls; button can be hidden but still active (e.g., when bound to hard key)
#define OPT_BUTTON_BEEP_ON_PRESS        0x2000  // 17-Oct-05: added; set from Button::bBeepOnPress
#define OPT_BUTTON_STAY_PRESSED         0x4000  // 17-Oct-05: added; set from Button::bStayPressed
#define OPT_BUTTON_NO_VISUAL            0x8000

// Image options
#define OPT_IMAGE_SCALE_ASPECT_RATIO    0x4000  // 01-Nov-06: Scale image (with preserving aspect ratio) or not
#define OPT_IMAGE_SCALE                 0x8000  // 01-Nov-06: Scale image or not

// Listbox options
#define OPT_LISTBOX_TRANSPARENT         0x2000
#define OPT_LISTBOX_IS_TEXTBOX          0x4000
#define OPT_LISTBOX_NO_SCROLL           0x8000

// Keypad's textbox options
#define OPT_TB_BORDER                   0x0010  // maps to: VF_TEXTBOX_FLAG_BORDER = 1 << 0 (0x01)
#define OPT_TB_PWD_MASK                 0x0020  // maps to: VF_TEXTBOX_FLAG_SECRET = 1 << 1 (0x02) - Password - echo with '*'
#define OPT_TB_LZ_MASK                  0x0040  // maps to: VF_TEXTBOX_FLAG_LEADING_ZERO = 1 << 2 (0x04) - Leading Zero allowed
#define OPT_TB_RL_MASK                  0x0080  // maps to: VF_TEXTBOX_FLAG_RIGHT_TO_LEFT = 1 << 3 (0x08) - Right-to-Left entry
#define OPT_TB_SKIP_LITERALS_MASK       0x0100  // maps to: VF_TEXTBOX_FLAG_SKIP_LITERALS = 1 << 4 (0x10) - don't return literal characters from the DisplayString
#define OPT_TB_CUSTOM_FONTS_MASK        0x0200  // maps to: VF_TEXTBOX_FLAG_CUSTOM_FONTS = 1 << 5 (0x20) - use application-specified fonts and font size
// KeyPad options
#define OPT_KP_WINK                     0x2000  // maps to: vf_keypad_wink_set() - added 29-Feb-08
#define OPT_KP_BEEP                     0x4000  // maps to: KP_OPT_BEEP (0x01) - beep on each entry
#define OPT_KP_APPCONTROL               0x8000  // maps to: KP_OPT_APPCONTROL (0x8000) - app to process every keypress

// EditField options
#define OPT_TB_BORDER                   0x0010  // maps to: VF_TEXTBOX_FLAG_BORDER = 1 << 0 (0x01)
#define OPT_TB_PWD_MASK                 0x0020  // maps to: VF_TEXTBOX_FLAG_SECRET = 1 << 1 (0x02) - Password - echo with '*'
#define OPT_TB_LZ_MASK                  0x0040  // maps to: VF_TEXTBOX_FLAG_LEADING_ZERO = 1 << 2 (0x04) - Leading Zero allowed
#define OPT_TB_RL_MASK                  0x0080  // maps to: VF_TEXTBOX_FLAG_RIGHT_TO_LEFT = 1 << 3 (0x08) - Right-to-Left entry
#define OPT_TB_SKIP_LITERALS_MASK       0x0100  // maps to: VF_TEXTBOX_FLAG_SKIP_LITERALS = 1 << 4 (0x10) - don't return literal characters from the DisplayString
#define OPT_TB_CUSTOM_FONTS_MASK        0x0200  // maps to: VF_TEXTBOX_FLAG_CUSTOM_FONTS = 1 << 5 (0x20) - use application-specified fonts and font size
#define TB_OPT_CURSOR                   0x0400  // maps to: VF_TEXTBOX_FLAG_SKIP_LITERALS = 1 << 6 (0x40) - enable textbox cursor
#define OPT_TB_BEEP                     0x4000  // beep on each entry - processed by guimgr library
#define OPT_TB_APPCONTROL               0x8000  // app to process every keypress - processed by guimgr library

// HardKeyPad options
#define OPT_HARDKEYPAD_ENABLED          0x0001
#define OPT_HARDKEYPAD_BEEP             0x0002

#define CLR_MASK                        0x0800  // 1=Just Clear - never Cancel

#define KBD_CANCEL                      0x18    // ASCII Cancel

#define OPT_MARQUEE_TRANSPARENT         0x8000

// Image options
#define OPT_IMAGE_SCALE                 0x8000      // Scale image to specified image width and height

// RichText options
#define OPT_RICHTEXT_RAW_WRAP               0x0100  // Raw wrapping of text, i.e., without respecting word boundaries
#define OPT_RICHTEXT_BORDER_IF_SCROLLBARS   0x0200  // Display border if scrollbars are displayed
#define OPT_RICHTEXT_VSCROLL                0x0400  // Add vertical scrollbar
#define OPT_RICHTEXT_HSCROLL                0x0800  // Add horizontal scrollbar
#define OPT_RICHTEXT_STRIP_LEADING_BLANKS   0x1000  // Strip leading blanks in line of text
#define OPT_RICHTEXT_AUTO_SIZE              0x2000  // Ignore height parameter, display all text provided
#define OPT_RICHTEXT_AUTO_WRAP              0x4000  // Auto-wrap text
#define OPT_RICHTEXT_TRANSPARENT            0x8000  // Make text background transparent

#define VIDEO_MODE_STOP     0
#define VIDEO_MODE_PLAY     1
#define VIDEO_MODE_PAUSE    2

// 26-Apr-06: Made local to add __attribute__ ((packed)) and imgPressedFileName[] field.
#define MY_MAX_NUM_HOTSPOTS 64  // #define'd in svcsec.h as 20

typedef struct my_touch_hotspot_info
{
   unsigned short  x1;         // starting x of hotspot
   unsigned short  y1;         // starting y of hotspot
   unsigned short  x2;         // ending x of hotspot
   unsigned short  y2;         // ending y of hotspot
   char            result;     // ASCII value to return
   char            options;    // RESERVED; value is 0x00 for enhanced MHS format
} __attribute__ ((packed)) MY_TOUCH_HOTSPOT_INFO;

// Enhancement to above struct to accomodate SHIFT and CAPSLOCK keypad functionality
#define MAX_UTF8_STR_SIZE 16
typedef struct my_touch_hotspot_info2
{
   unsigned short  x1;         // starting x of hotspot
   unsigned short  y1;         // starting y of hotspot
   unsigned short  x2;         // ending x of hotspot
   unsigned short  y2;         // ending y of hotspot
   char            result;     // ASCII value to return
   char            options;    // RESERVED; value is 0x02 for enhanced MHS format
   unsigned char   uchShiftResult;                          // Shifted Decimal value for the hotspot to use
   char            szUTF8String[MAX_UTF8_STR_SIZE+1];       // Quoted UTF-8 string for the hotspot to use
   char            szShiftUTF8String[MAX_UTF8_STR_SIZE+1];  // Quoted Shifted UTF-8 string for the hotspot to use
   unsigned char   uchModifier;                             // Modifier flags for the hotspot; see KP_MODIFIER_xxx_BIT values in vf_keypad.h
} __attribute__ ((packed)) MY_TOUCH_HOTSPOT_INFO2;

typedef struct my_touch_hs_s
{
    char    imgPressedFileName[MAX_FILENAME_LEN+1]; // 26-Apr-06: Pressed image filename for custom keypad
    short   num_hotspots;       // the number of active hotspots
    MY_TOUCH_HOTSPOT_INFO touch_spot[MY_MAX_NUM_HOTSPOTS];
} __attribute__ ((packed)) MY_TOUCH_HS_S;

// 15-Nov-07: struct to read in entire enhanced MHS data from FRM file
typedef struct my_touch_hs_s2
{
    char    imgPressedFileName[MAX_FILENAME_LEN+1];
    short   num_hotspots;       // the number of active hotspots
    MY_TOUCH_HOTSPOT_INFO2 touch_spot[MY_MAX_NUM_HOTSPOTS];
} __attribute__ ((packed)) MY_TOUCH_HS_S2;

typedef struct tagLabel
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    short   fontIndex;
    short   fontSize;
    char    fontAttributes;
    USHORT  options;                    // bit defined options: see OPT_xxx above
    char    caption[MAX_NAME_LEN+1];
    BOOL    borderVisible;              // Whether the border is visible or not
    char    textJustification;          // LEFT, RIGHT, CENTER
    BOOL    bAutoSize;                  // 08-Apr-07: adjust fontsize downwards in attempt to fit entire caption
    ULONG   rgbBGColor;                 // Rectangular region background color
    ULONG   rgbFGColor;                 // Foreground (pen) color for caption
} __attribute__ ((packed)) Label;

typedef struct tagButton
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    short   fontIndex;
    short   fontSize;
    char    fontAttributes;
    short   cbIndex;                                    // value auto-generated by FormManager tool, mneumonic supplied by user
    short   iHotSpotID;                                 // value auto-generated by FormManager tool
    short   state;                                      // Button State: 0=Up/Normal, 1=Down/Pressed
    USHORT  options;                                    // bit defined options: see OPT_xxx above
    char    caption[MAX_NAME_LEN+1];
    char    imageFileName[MAX_FILENAME_LEN+1];          // jpg/png filename for regular button face
    char    imageFileNamePressed[MAX_FILENAME_LEN+1];   // jpg/png filename for button face after press
    BOOL    bBeepOnPress;
    BOOL    bStayPressed;                               // Show button depressed after it is pressed
    BOOL    borderVisible;                              // Whether the border is visible or not
    ULONG   rgbBGColor;                                 // Button's background color (for button's "up" image); the "down" image will still come form the current theme
    ULONG   rgbFGColor;                                 // Foreground (pen) color for button caption
} __attribute__ ((packed)) Button;

typedef struct tagImage
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    short   cbIndex;
    short   iHotSpotID;
    USHORT  options;                            // bit defined options: see OPT_xxx above
    BOOL    borderVisible;                      // Whether the border is visible or not
    char    imageFileName[MAX_FILENAME_LEN+1];
} __attribute__ ((packed)) Image;

typedef struct tagCheckBox
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    short   cbIndex;
    short   iHotSpotID;
    short   fontIndex;
    short   fontSize;
    char    fontAttributes;
    USHORT  options;                                // bit defined options: see OPT_xxx above
    char    caption[MAX_NAME_LEN+1];
    char    uncheckedFileName[MAX_FILENAME_LEN+1];  // jpg/png filename for unchecked box
    char    checkedFileName[MAX_FILENAME_LEN+1];    // jpg/png filename for checked box
    BOOL    borderVisible;                          // Whether the border is visible or not
    BOOL    bSelected;
    BOOL    bBeepOnPress;
    ULONG   rgbFGColor;
} __attribute__ ((packed)) CheckBox;

typedef struct tagRadioButton
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    short   cbIndex;
    short   iHotSpotID;
    short   fontIndex;
    short   fontSize;
    char    fontAttributes;
    USHORT  options;                                // bit defined options: see OPT_xxx above
    char    caption[MAX_NAME_LEN+1];
    BOOL    bTransparent;                           // 03-Apr-07: added: whether background is transparent or not
    BOOL    bSelected;
    BOOL    bBeepOnPress;
    ULONG   rgbBGColor;     // 03-Apr-07: added
    ULONG   rgbFGColor;
    char    chGroupNum;
} __attribute__ ((packed)) RadioButton;

typedef struct tagListBox
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    short   fontIndex;
    short   fontSize;
    char    fontAttributes;
    short   cbIndex_OnSelect;
    short   iHotSpotID_OnSelect;
    short   cbIndex_OnUp;
    short   iHotSpotID_OnUp;
    short   cbIndex_OnDown;
    short   iHotSpotID_OnDown;
    USHORT  options;                        // bit defined options: see OPT_xxx above
    BOOL    borderVisible;                  // Whether the border is visible or not
    BOOL    bBeepOnPress;
    signed char bScrollValue;               // number of items to scroll
    ULONG   rgbBGColor;                     // Rectangular region background color
    ULONG   rgbFGColor;                     // Foreground (pen) color for item text
} __attribute__ ((packed)) ListBox;

typedef struct tagLine
{
    short   iControlID;
    short   x1;
    short   y1;
    short   x2;
    short   y2;
    short   style;                          // SOLID, DOTTED
    USHORT  options;                        // bit defined options: see OPT_xxx above
    ULONG   rgbColor;
} __attribute__ ((packed)) Line;

typedef struct tagBox
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    short   style;                          // SOLID, DOTTED
    USHORT  options;                        // bit defined options: see OPT_xxx above
    ULONG   rgbColor;
} __attribute__ ((packed)) Box;

#define KP_CUSTOM_ENHANCED 4
// Note: the width and height used for the keypad image will be determined from the image filename itself
typedef struct tagKeyPad
{
    short   iControlID;
    short   left;                           // associated textbox's left position
    short   top;                            // associated textbox's top position
    short   width;                          // associated textbox's width position
    short   height;                         // associated textbox's height position
    short   fontIndex;
    short   fontSize;
    char    fontAttributes;
    USHORT  options;                        // bit defined options: see OPT_xxx above
    USHORT  min_entries;                    // minimum entries required before <ENTER> is valid
    short   keypad;                         // KP_ALPHA = 1, KP_NUMERIC = 2, KP_CUSTOM = 3, KP_CUSTOM_ENHANCED = 4
    short   cbIndex;                        // value auto-generated by FormManager tool, mneumonic supplied by user
    short   iHotSpotID;                     // value auto-generated by FormManager tool
    short   imgLeft;                        // keypad image left position
    short   imgTop;                         // keypad image top position
    char    display_string[MAX_LINE_LEN+1];
    char    format_string[MAX_LINE_LEN+1];
    char    imgFileName[MAX_FILENAME_LEN+1];    // Normal image filename for custom keypad
    ULONG   rgbFGColor;                     // 15-Aug-05: Added since vf_textbox now supports text color (finally!)
    MY_TOUCH_HS_S *imgHotSpots;  // 26-Apr-06: Changed from touch_hs_s to use my_touch_hs_s because we want '__attribute__ ((packed))' for reading from disk
                                 // 15-Nov-07: Note: This can be either MY_TOUCH_HS_S (old MHS format) or MY_TOUCH_HS_S2 (new MHS format)
} __attribute__ ((packed)) KeyPad;

typedef struct tagAnimate
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    short   cbIndex;
    short   iHotSpotID;
    short   iMsDelay;                           // Millisec delay between frames
    short   iLoopCount;                         // -1 = Loop forever
    USHORT  options;                            // bit defined options: see OPT_xxx above
    short   iMode;                              // 0 = Stop, 1 = Run
    char    chPresentationEffect;
    char    eetFileName[MAX_FILENAME_LEN+1];    // .EET File Name
} __attribute__ ((packed)) Animate;

typedef struct tagMarquee
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    short   fontIndex;
    short   fontSize;
    char    fontAttributes;
    USHORT  options;                    // bit defined options: see OPT_xxx above
    char    caption[MAX_MARQUEE_LEN+1];
    BOOL    borderVisible;              // Whether the border is visible or not
    ULONG   rgbBGColor;                 // Rectangular region background color
    ULONG   rgbFGColor;                 // Foreground (pen) color for caption
    USHORT  shTimerIntervalMilliSec;    // Valid range is 1-1000 milliseconds
} __attribute__ ((packed)) Marquee;

typedef struct tagVideo
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    short   cbIndex;
    short   iHotSpotID;                          // 0 = No hotspot
    USHORT  options;                             // bit defined options: see OPT_xxx above
    char    chMode;                              // VIDEO_MODE_xxx: 0 = Stop, 1 = Play, 2 = Pause
    BOOL    bRepeatVideo;                        // TRUE => repeat video playback after reaching end
    char    szVideoFileName[MAX_FILENAME_LEN+1]; // .MPG/.AVI File Name
} __attribute__ ((packed)) Video;

typedef struct tagRichText
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    USHORT  options;                            // bit defined options: see OPT_xxx above
    short   fontIndex;
    short   boldFontNameIndex;
    short   italicsFontNameIndex;
    short   boldItalicsFontNameIndex;
    short   fontSize;
    char    fontAttributes;
    long    lrgbBGColor;                        // Valid only if OPT_RICHTEXT_TRANSPARENT is off
    long    lrgbFGColor;                        // Text color
    long    lrgbSelColor;                       // RGB color to use for highlighting (i.e., text within <R> <R> tags)
} __attribute__ ((packed)) RichText;

/****************************************************************
 *  Event Data Structures                                       *
 ****************************************************************/

typedef struct ControlEventData
{
    short   iControlID;     // iControlID of object that caused the event
    short   type;           // Control type
} __attribute__ ((packed)) CONTROLEVENTDATA;

typedef struct ButtonEventData
{
    Evas_Object *obj;
    Evas_Event_Mouse_Up mouse_event; // 07-Mar-08: was 'void *mouse_event'
} __attribute__ ((packed)) BUTTONEVENTDATA;

typedef struct CheckboxEventData
{
    Evas_Object *obj;
    BOOL bSelected;     // TRUE => checked; FALSE => unchecked
} __attribute__ ((packed)) CHECKBOXEVENTDATA;

typedef struct RadioButtonEventData
{
    short iControlID;                   // iControlID of object that caused the event
    char  chGroupNum;                   // Group# of selected radiobutton
    char  szCaption[MAX_NAME_LEN+1];    // Caption text of selected radiobutton
} __attribute__ ((packed)) RADIOBUTTONEVENTDATA;

typedef struct EvasEventData
{
    Evas *e;
    Evas_Object *obj;
    Evas_Event_Mouse_Down event_info; // 07-Mar-08: was 'void *event_info'
} __attribute__ ((packed)) EVASEVENTDATA;

#define ON_SELECT           1
#define ON_SCROLL_DOWN      2
#define ON_SCROLL_UP        3
#define ON_DESELECT         4
typedef struct ListboxEventData
{
    Evas_Object *obj;
    short iIndex;         // This is a 0-based index; value is -1 if no item is selected
    short iEventType;     // { 1=ON_SELECT; 2=ON_SCROLL_DOWN, 3=ON_SCROLL_UP; 4=ON_DESELECT }
} __attribute__ ((packed)) LISTBOXEVENTDATA;

#define ON_KEYPRESS         1   // Note: This is also used for CTRL_FNKEYBTN presses
#define ON_ENTER            2
#define ON_CANCEL           3
#define ON_TIMEOUT          4
#define ON_CLEARKEY         5   // Keypad session aborted due to CLEAR key press when buffer is empty, *if* INTERAC_ENTRY=1

#define USER_ABORT          1
#define PGM_ABORT           2

#define MAX_INPUT_SIZE      120 // 27-Nov-07: added
typedef struct KeypadEventData
{
    short iEventType;     // { 1=ON_KEYPRESS; 2=ON_ENTER, 3=ON_CANCEL, 4=ON_TIMEOUT, 5=ON_CLEARKEY }
    union eventData
    {
        short   iKeyValue;      // for ON_KEYPRESS, ON_TIMEOUT (only for OPT_TB_APPCONTROL, with value=0x0D/ENTER)
        char    pszValue[MAX_INPUT_SIZE+1]; // for ON_ENTER, ON_TIMEOUT
        short   iReasonCode;    // for ON_CANCEL, {USER_ABORT, PGM_ABORT} -or- ON_CLEARKEY {USER_ABORT)
    } val;
} __attribute__ ((packed)) KEYPADEVENTDATA;

#define ON_KEYPRESS         1
#define ON_ANIMATION_END    2
typedef struct AnimationEventData
{
    short iEventType;     // { 1=ON_KEYPRESS; 2=ON_ANIMATION_END }
} __attribute__ ((packed)) ANIMATIONEVENTDATA;

#define ON_KEYPRESS         1
#define ON_MEDIA_END        2
typedef struct VideoEventData
{
    short iEventType;     // { 1=ON_KEYPRESS; 2=ON_MEDIA_END }
    char  szVideoFileName[MAX_FILENAME_LEN+1]; // .MPG/.AVI File Name
} __attribute__ ((packed)) VIDEOEVENTDATA;

#ifdef SLIDE_EVENTS // 22-Feb-10
typedef struct FormEventData
{
    short iSlideDirection;     // { 1=SLIDE_LEFT; 2=SLIDE_RIGHT, 3=SLIDE_UP, 4=SLIDE_DOWN }
} __attribute__ ((packed)) FORMEVENTDATA;
#endif // SLIDE_EVENTS

typedef union tagALL_EVENTS
{
    BUTTONEVENTDATA      ev_btn;
    CHECKBOXEVENTDATA    ev_chk;
    RADIOBUTTONEVENTDATA ev_rb;
    EVASEVENTDATA        ev_evas;
    LISTBOXEVENTDATA     ev_lst;
    KEYPADEVENTDATA      ev_kbd;
    ANIMATIONEVENTDATA   ev_ani;
    VIDEOEVENTDATA       ev_vid;
#ifdef SLIDE_EVENTS // 22-Feb-10
    FORMEVENTDATA        ev_form; // 22-Feb-10: added for form slide events
#endif // SLIDE_EVENTS
} __attribute__ ((packed)) ALL_EVENTS;

// EDITFIELD control
typedef struct tagEditField
{
    short   iControlID;
    short   left;                               // Textbox's left position
    short   top;                                // Textbox's top position
    short   width;                              // Textbox's width position
    short   height;                             // Textbox's height position
    short   fontIndex;
    short   fontSize;
    char    fontAttributes;
    USHORT  options;                            // bit defined options
    USHORT  min_entries;                        // minimum entries required before <ENTER> is valid
    short   keypad;                             // KP_ALPHA = 1, KP_NUMERIC = 2, KP_CUSTOM = 3
                                                // NOTE: we use this at runtime to determine whether to switch the
                                                // hard keypad into alpha mode or not, using the fp_keytranslate_set() function.
                                                // Only ALPHA and NUMERIC types matter.
    short   cbIndex;                            // value auto-generated by FormManager tool, mneumonic supplied by user
    short   iHotSpotID;                         // value auto-generated by FormManager tool
    char    display_string[MAX_LINE_LEN+1];
    char    format_string[MAX_LINE_LEN+1];
    ULONG   rgbFGColor;                         // Text color
} __attribute__ ((packed)) EditField;

// FNKEYBTN control
typedef struct tagFnKeyBtn
{
    short   iControlID;
    short   left;
    short   top;
    short   width;
    short   height;
    short   fontIndex;
    short   fontSize;
    char    fontAttributes;
    short   cbIndex;         // value auto-generated by FormManager tool, mneumonic supplied by user
    short   iHotSpotID;      // value auto-generated by FormManager tool
    short   state;           // Button State: 0=Up/Normal, 1=Down/Pressed
    USHORT  options;         // bit defined options
    char    caption[MAX_NAME_LEN+1];
    char    imageFileName[MAX_FILENAME_LEN+1];          // jpg/png filename for regular button face
    char    imageFileNamePressed[MAX_FILENAME_LEN+1];   // jpg/png filename for button face after press
    BOOL    bBeepOnPress;
    BOOL    bStayPressed;    // Show button depressed after it is pressed
    BOOL    borderVisible;   // Whether the border is visible or not
    ULONG   rgbBGColor;      // Button's background color (for button's "up" image); the "down" image will still come form the current theme
    ULONG   rgbFGColor;      // Foreground (pen) color for button caption
    BOOL    bCaptionAreaIsActive; // T=>user can press button area to generate callback; F=>button caption area will be shown 'flat'/unpressable
    short   ushAssocFuncKey; // ANY hard keypad button: { F1-F4, 0-9, ENTER, CLEAR, CANCEL }
} __attribute__ ((packed)) FnKeyBtn;

// HARDKEYPAD control
typedef struct tagHardKeyPad
{
    short   iControlID;
    short   cbIndex;        // value auto-generated by FormManager tool, mneumonic supplied by user
    short   iHotSpotID;     // value auto-generated by FormManager tool
    USHORT  options;        // bit defined options
    short   keypad;         // KP_ALPHA = 1, KP_NUMERIC = 2, KP_CUSTOM = 3
    unsigned char uchActiveKeys[17];    // Slots: 0-16 = total of 17 slots
                                        // Slots correspond to the following values in order:
                                        //   0-9 : 0x30-0x39
                                        // 10-13 : 0xF1, 0xF2, 0xF3, 0xF4,
                                        // 14-16 : 0x1B (CANCEL), 0x08 (BACKSPACE), 0x0D (ENTER)
                                        // NOTE: uchActiveKeys[n] = '0' for an empty slot (inactive key)
} __attribute__ ((packed)) HardKeyPad;

#ifdef __cplusplus
}
#endif

#endif // GUI.H

