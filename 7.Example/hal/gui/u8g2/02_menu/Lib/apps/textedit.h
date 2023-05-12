#ifndef __TEXTEDIT_H__
#define __TEXTEDIT_H__

#include "base.h"

typedef struct {
    bool bRepaint;
    // callback
    cbk_t lpfnPainter;
    cbk_t lpfnHandler;

    uint8_t nWidth;
    uint8_t nHeight;

    const char*   szEditorTitle;
    const uint8_t wEditorTitle;
    const uint8_t nEditorTitleSize;

    const char*   szButtonTitle;
    const uint8_t wButtonTitle;

    char*   szContent;
    uint8_t nContentSize;  // strlen

    uint8_t nSelectedIndex;

    bool    bEditing;  // 编辑中
    uint8_t blink;     // 光标闪烁

    uint8_t xBlinkArea, wBlinkArea;

    uint8_t hLine;

} texteditor_t;

texteditor_t textedit_initialize(
    cbk_t   lpfnPainter,
    cbk_t   lpfnHandler,
    char*   szContent,
    uint8_t nContentSize);

void texteditor_measure_blink_pos(texteditor_t* p);
void texteditor_measure_blink_char(texteditor_t* p);

void texteditor_callback_handle_set_prevchar(texteditor_t* p);
void texteditor_callback_handle_set_nextchar(texteditor_t* p);
bool texteditor_callback_handle_switch_editmode(texteditor_t* p);

void texteditor_callback_default_handler(texteditor_t* p);
void texteditor_callback_default_painter(texteditor_t* p);

#endif