// #include "textedit.h"

// #define BLINK_SPEED 24  // 2的倍数

// texteditor_t textedit_initialize(
//     cbk_t   lpfnPainter,
//     cbk_t   lpfnHandler,
//     char*   szContent,
//     uint8_t nContentSize)
// {
//     texteditor_t m = {
//         .lpfnPainter    = lpfnPainter == 0 ? texteditor_callback_default_painter : lpfnPainter,
//         .lpfnHandler    = lpfnHandler == 0 ? texteditor_callback_default_handler : lpfnHandler,
//         .nWidth         = CONFIG_SCREEN_WIDTH,
//         .nHeight        = CONFIG_SCREEN_HEIGHT,
//         .szEditorTitle  = "--Text Editor--",
//         .wEditorTitle   = _str_w_("--Text Editor--"),
//         .szButtonTitle  = "-Return",
//         .wButtonTitle   = _str_w_("-Return"),
//         .szContent      = szContent,
//         .nContentSize   = nContentSize,
//         .xBlinkArea     = 0,
//         .wBlinkArea     = 0,
//         .nSelectedIndex = 0,
//         .bEditing       = false,
//         .hLine          = 16,
//     };

//     if (nContentSize) texteditor_measure_blink_char(&m);

//     return m;
// }

// void texteditor_measure_blink_pos(texteditor_t* p)
// {
//     if (p->nSelectedIndex == p->nContentSize) return;

//     char c = p->szContent[p->nSelectedIndex];
//     // set end
//     p->szContent[p->nSelectedIndex] = '\0';
//     // get width (hover left pos)
//     p->xBlinkArea = _str_w_(p->szContent);
//     // recover char
//     p->szContent[p->nSelectedIndex] = c;
// }

// void texteditor_measure_blink_char(texteditor_t* p)
// {
//     if (p->nSelectedIndex == p->nContentSize) return;
//     // get width (hover char witdh)
//     char buff[2]  = {p->szContent[p->nSelectedIndex], '\0'};
//     p->wBlinkArea = _str_w_(buff);
// }

// void texteditor_callback_handle_set_prevchar(texteditor_t* p)
// {
//     if (p->bEditing) {
//         // space->Z->...->A->z->...->a->space
//         char ch = p->szContent[p->nSelectedIndex];
//         switch (ch) {
//             case 'a': ch = ' '; break;
//             case 'A': ch = 'z'; break;
//             case ' ': ch = 'Z'; break;
//             default: --ch; break;
//         }
//         p->szContent[p->nSelectedIndex] = ch;
//     } else {
//         // decrease selected
//         if (p->nSelectedIndex > 0) {
//             --p->nSelectedIndex;
//         } else {
//             p->nSelectedIndex = p->nContentSize;
//         }
//         // measure
//         texteditor_measure_blink_pos(p);
//     }
//     // measure
//     texteditor_measure_blink_char(p);
//     // repaint
//     p->bRepaint = true;
// }

// void texteditor_callback_handle_set_nextchar(texteditor_t* p)
// {
//     if (p->bEditing) {
//         // space->a->...->z->A->...->Z->space
//         char ch = p->szContent[p->nSelectedIndex];
//         switch (ch) {
//             case ' ': ch = 'a'; break;
//             case 'z': ch = 'A'; break;
//             case 'Z': ch = ' '; break;
//             default: ++ch; break;
//         }
//         p->szContent[p->nSelectedIndex] = ch;
//     } else {
//         // increase selected
//         if (p->nSelectedIndex < p->nContentSize) {
//             ++p->nSelectedIndex;
//         } else {
//             p->nSelectedIndex = 0;
//         }
//         // measure
//         texteditor_measure_blink_pos(p);
//     }
//     // measure
//     texteditor_measure_blink_char(p);
//     // repaint
//     p->bRepaint = true;
// }

// bool texteditor_callback_handle_switch_editmode(texteditor_t* p)
// {
//     if (p->nSelectedIndex == p->nContentSize) {
//         // swicth fail, beacause cursor on button
//         return false;
//     } else {
//         // edit mode
//         p->bEditing = !p->bEditing;
//         // repaint
//         p->bRepaint = true;
//         // swicth sucess
//         return true;
//     }
// }

// void texteditor_callback_default_handler(texteditor_t* p)
// {
//     switch (key_scan()) {
//         case KEY_ID_NONE: break;
//         case KEY_ID_PREV: {
//             texteditor_callback_handle_set_prevchar(p);
//             break;
//         }
//         case KEY_ID_NEXT: {
//             texteditor_callback_handle_set_nextchar(p);
//             break;
//         }
//         case KEY_ID_OK: {
//             texteditor_callback_handle_switch_editmode(p);
//             break;
//         }
//     }
// }

// void texteditor_callback_default_painter(texteditor_t* p)
// {
//     _clear();

//     // frame
//     _fiil_rounded_rect(4, 6, 120, 52, 8);
//     // title
//     _draw_str((128 - p->wEditorTitle) / 2, 20, p->szEditorTitle);
//     // text
//     uint8_t box_x = 10;
//     _draw_str(box_x, 38, p->szContent);
//     // button
//     _draw_str(80, 50, p->szButtonTitle);

//     // cursor
//     if (p->nSelectedIndex < p->nContentSize) {
//         if (p->blink < BLINK_SPEED / 2 || !p->bEditing) {
//             _set_color(2);
//             _draw_rect(box_x + p->xBlinkArea, 26, p->wBlinkArea + 2, p->hLine);
//             _set_color(1);
//         }
//     } else {
//         // button (hover)
//         _set_color(2);
//         _draw_rounded_rect(78, 38, p->wButtonTitle + 4, p->hLine, 1);
//         _set_color(1);
//     }

//     if (p->bEditing) {
//         p->blink    = (p->blink < BLINK_SPEED) ? (p->blink + 1) : 0;
//         p->bRepaint = true;
//     }

//     _update();
// }
