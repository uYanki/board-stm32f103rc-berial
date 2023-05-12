/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <string.h>
#include "charset.h"
#include <tcutil.h>
#include "comn.h"
#include "utils.h"
#include "xmalloc.h"
#include "xstring.h"

/* utf-8: 0xc0, 0xe0, 0xf0, 0xf8, 0xfc */
uint8_t utf8charlen[256] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1};

/* gbk(gb2312|big5): 0x81 ~ 0xfe */
uint8_t gbkcharlen[256] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};

uint8_t lowercharmap[256] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

uint8_t uppercharmap[256] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

/* transfer quanjiao ascii chars (GB2312 zone3) to banjiao
   transfer quanjiao keyboard letter/number/symbol to banjiao
   this transformation can be made in-place, i.e. szIn == szOut
   注意: ￥ -> $
*/
void gbkqj2bj(const char* szIn, char* szOut)
{
    uint8_t        ch;
    const uint8_t* pin  = (const uint8_t*)szIn;
    uint8_t*       pout = (uint8_t*)szOut;

    if (!szIn || !szOut)
        return;

    while (*pin) {
        if (*pin < 0x80) {
            *pout++ = *pin++;
        } else {
            if (*pin == 0xA3 && (pin[1] >= 0xA1)) {
                ch      = pin[1] - 0x80;
                *pout++ = ch;
                pin += 2;
            } else {
                *pout++ = *pin++;
                *pout++ = *pin++;
            }
        }
    }
    *pout = 0;
}

/* transfer chinese symbols (GB2312 zone1) to corresponding ascii chars
   transfer some special chinese quanjiao symbols to its corresponding banjiao form
   this transformation can be made in-place, i.e. szIn == szOut
   ** 0xA1A1    -> 0x20
      0xA1AA — -> -
      、 -> ,
      。 -> .
      ～ -> ~
      ‘ ’-> '
      “ ”-> "
      ∶-> :
      ＄ -> $
*/
void gbkqj2bjext(const char* szIn, char* szOut)
{
    int            ch;
    const uint8_t* pin  = (const uint8_t*)szIn;
    uint8_t*       pout = (uint8_t*)szOut;

    static const uint8_t l_symbol[96] =
        {
            0, ' ', ',', '.', 4, 5, 6, 7, 8, 9, '-', '~', 12, 13, '\'', '\'',
            '\"', '\"', 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
            32, 33, 34, ':', 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
            48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
            64, 65, 66, 67, 68, 69, 70, '$', 72, 73, 74, 75, 76, 77, 78, 79,
            80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95};

    if (!szIn || !szOut)
        return;

    while (*pin) {
        if (*pin < 0x80) {
            *pout++ = *pin++;
        } else if (*pin == 0xA1 && (ch = pin[1] - 0xA0) >= 1 && l_symbol[ch] != ch) {  // in zone1, and the second byte is in the table
            // assert(ch >= 1 && ch <= 95);
            *pout++ = l_symbol[ch];
            pin += 2;
        } else {
            *pout++ = *pin++;
            *pout++ = *pin++;
        }
    }
    *pout = 0;
}

// utf-8 space char: 0xe3 0x80 0x80 --> 0x20
//   NO-BREAK SPACE: 0xC2 0xA0 --> 0x20
void utf8space2ascii(const char* in, char* out)
{
    uint8_t* pi = (uint8_t*)in;
    uint8_t* po = (uint8_t*)out;
    uint8_t* pe;
    int      len, clen, n;

    if (!in || !out) return;

    len = strlen(in);
    pe  = pi + len;

    while (pi != pe) {
        clen = UTF8CHLEN(*pi);
        if (pi + clen > pe)
            break;

        if (clen == 3 && *pi == 0xe3 && *(pi + 1) == 0x80 && *(pi + 2) == 0x80) {
            *po = ' ';
            ++po;
        } else if (clen == 2 && *pi == 0xc2 && *(pi + 1) == 0xa0) {
            *po = ' ';
            ++po;
        } else {
            for (n = 0; n < clen; n++)
                *(po + n) = *(pi + n);

            po += clen;
        }

        pi += clen;
    }

    *po = 0;
}

// utf-8: mbc2sbc,support in == out.
// utf-8 first range: 0xef 0xbc 0x81 --> 0xef 0xbc 0xbf
//             ascii:           0x21 -->           0x5f
// utf-8 second range: 0xef 0xbd 0x80 --> 0xef 0xbd 0x9e
//              ascii:           0x60 -->           0x7e
// first: 0x60, second: 0x20
// utf-8 space char: 0xe3 0x80 0x80 --> 0x20
//   NO-BREAK SPACE: 0xC2 0xA0 --> 0x20
void utf8qj2bj(const char* in, char* out)
{
    uint8_t* pi = (uint8_t*)in;
    uint8_t* po = (uint8_t*)out;
    uint8_t* pe;
    int      len, clen, n;

    if (!in || !out) return;

    len = strlen(in);
    pe  = pi + len;

    while (pi != pe) {
        clen = UTF8CHLEN(*pi);
        if (pi + clen > pe)
            break;

        if (clen == 3 && *pi == 0xef && *(pi + 1) == 0xbc && *(pi + 2) >= 0x81 && *(pi + 2) <= 0xbf) {
            *po = *(pi + 2) - 0x60;
            ++po;
        } else if (clen == 3 && *pi == 0xef && *(pi + 1) == 0xbd && *(pi + 2) >= 0x80 && *(pi + 2) <= 0x9e) {
            *po = *(pi + 2) - 0x20;
            ++po;
        } else if (clen == 3 && *pi == 0xe3 && *(pi + 1) == 0x80 && *(pi + 2) == 0x80) {
            *po = ' ';
            ++po;
        } else if (clen == 2 && *pi == 0xc2 && *(pi + 1) == 0xa0) {
            *po = ' ';
            ++po;
        } else {
            for (n = 0; n < clen; n++)
                *(po + n) = *(pi + n);

            po += clen;
        }

        pi += clen;
    }

    *po = 0;
}

struct utf8qjext {
    char* s;
    char  c;
};

static struct utf8qjext UTF8QJEXTs[] = {
    {"\xe2\x80\x94",  '-'}, /* 0xe28094   — -> -   */
    {"\xe3\x80\x81",  ','}, /* 0xe38081  、 -> ,    */
    {"\xe3\x80\x82",  '.'}, /* 0xe38082  。 -> .    */
    {"\xe2\x80\x98", '\''}, /* 0xe28098 ‘ -> '     */
    {"\xe2\x80\x99", '\''}, /* 0xe28099  ’-> '     */
    {"\xe2\x80\x9c",  '"'}, /* 0xe2809c   “-> "    */
    {"\xe2\x80\x9d",  '"'}, /* 0xe2809d   ”-> "    */
    {"\xe2\x88\xb6",  ':'}, /* 0xe288b6   ∶-> :    */
    {"\xef\xbf\xa5",  '$'}  /* 0xefbfa5   ￥-> $    */
};

void utf8qj2bjext(const char* in, char* out)
{
    uint8_t* pi = (uint8_t*)in;
    uint8_t* po = (uint8_t*)out;
    uint8_t* pe;
    int      i, len, clen, n;

    if (!in || !out) return;

    len = strlen(in);
    pe  = pi + len;

    while (pi != pe) {
        clen = UTF8CHLEN(*pi);
        if (pi + clen > pe)
            break;
        for (i = 0; i < countof(UTF8QJEXTs); i++) {
            len = strlen(UTF8QJEXTs[i].s);
            if (len == clen && memcmp(pi, UTF8QJEXTs[i].s, clen) == 0) {
                *po = UTF8QJEXTs[i].c;
                ++po;
                break;
            }
        }
        if (i == countof(UTF8QJEXTs)) {
            for (n = 0; n < clen; n++)
                *(po + n) = *(pi + n);
            po += clen;
        }
        pi += clen;
    }

    *po = 0;
}

// 常用全角和中文分隔符转换为同语义的ASCII字符。
static struct utf8qjext UTF8QJEXT2s[] = {
    {"\xef\xbc\xbf", '_'}, /* 0xefbcbf ＿ -> _     */
    {"\xef\xbc\x8d", '-'}, /* 0xefbc8d － -> -     */
    {"\xef\xbd\x9c", '|'}, /* 0xefbd9c ｜ -> |     */
    {"\xef\xbc\x8c", ','}, /* 0xefbc8c ， -> ,     */
    {"\xef\xbc\x9b", ';'}, /* 0xefbc9b ； -> ;     */
    {"\xe2\x80\x94", '-'}, /* 0xe28094 — -> -     */
    {"\xe3\x80\x81", ','}  /* 0xe38081 、 -> ,     */
};

void utf8qj2bjext2(const char* in, char* out)
{
    uint8_t* pi = (uint8_t*)in;
    uint8_t* po = (uint8_t*)out;
    uint8_t* pe;
    int      i, len, clen, n;

    if (!in || !out) return;

    len = strlen(in);
    pe  = pi + len;

    while (pi != pe) {
        clen = UTF8CHLEN(*pi);
        if (pi + clen > pe)
            break;
        for (i = 0; i < countof(UTF8QJEXT2s); i++) {
            len = strlen(UTF8QJEXT2s[i].s);
            if (len == clen && memcmp(pi, UTF8QJEXT2s[i].s, clen) == 0) {
                *po = UTF8QJEXT2s[i].c;
                ++po;
                break;
            }
        }
        if (i == countof(UTF8QJEXT2s)) {
            for (n = 0; n < clen; n++)
                *(po + n) = *(pi + n);
            po += clen;
        }
        pi += clen;
    }

    *po = 0;
}

// support in == out. 大写的全角字母直接转换为小写的半角英文字母
// 注意不是全角的大写字母不会转换。
// utf-8 upper char: 0xef 0xbc 0xa1 --> 0xef 0xbc 0xba
// ascii lower char:           0x61 --> 0x7a
//             range: 0x40
void utf8qj2bjtolower(const char* in, char* out)
{
    uint8_t* pi = (uint8_t*)in;
    uint8_t* po = (uint8_t*)out;
    uint8_t* pe;
    int      len, clen, n;

    if (!in || !out) return;
    len = strlen(in);
    pe  = pi + len;

    while (pi != pe) {
        clen = UTF8CHLEN(*pi);
        if (pi + clen > pe)
            break;

        if (clen == 3 && *pi == 0xef && *(pi + 1) == 0xbc && *(pi + 2) >= 0xa1 && *(pi + 2) <= 0xba) {
            *po = *(pi + 2) - 0x40;
            ++po;
        } else if (clen == 3 && *pi == 0xef && *(pi + 1) == 0xbc && *(pi + 2) >= 0x81 && *(pi + 2) <= 0xbf) {
            *po = *(pi + 2) - 0x60;
            ++po;
        } else if (clen == 3 && *pi == 0xef && *(pi + 1) == 0xbd && *(pi + 2) >= 0x80 && *(pi + 2) <= 0x9e) {
            *po = *(pi + 2) - 0x20;
            ++po;
        } else if (clen == 3 && *pi == 0xe3 && *(pi + 1) == 0x80 && *(pi + 2) == 0x80) {
            *po = ' ';
            ++po;
        } else if (clen == 2 && *pi == 0xc2 && *(pi + 1) == 0xa0) {
            *po = ' ';
            ++po;
        } else {
            for (n = 0; n < clen; n++)
                *(po + n) = *(pi + n);

            po += clen;
        }

        pi += clen;
    }

    *po = 0;
}

const char* PUNCTS1[] = {
    "\xE3\x80\x82",  // 。
    "\xEF\xBC\x9F",  // ？
    "\xEF\xBC\x81",  // ！
    "\xEF\xBC\x8C",  // ，
    "\xEF\xBC\x9B",  // ；
    "\xE3\x80\x81",  // 、
    "\xEF\xBC\x9A",  // ：
    "!", "?", ",", ";", ":"};

const char* PUNCTS2[] = {
    "\xE2\x80\x98",  // ‘
    "\xE2\x80\x99",  //  ’
    "\xE2\x80\x9C",  // “
    "\xE2\x80\x9D",  //  ”
    "\xE3\x80\x94",  // 〔
    "\xE3\x80\x95",  // 〕
    "\xE3\x80\x88",  // 〈
    "\xE3\x80\x89",  // 〉
    "\xE3\x80\x8A",  // 《
    "\xE3\x80\x8B",  // 》
    "\xE3\x80\x8C",  // 「
    "\xE3\x80\x8D",  // 」
    "\xE3\x80\x8E",  // 『
    "\xE3\x80\x8F",  // 』
    "\xE3\x80\x96",  // 〖
    "\xE3\x80\x97",  //  〗
    "\xE3\x80\x90",  // 【
    "\xE3\x80\x91",  //  】
    "\xEF\xBC\x82",  //  ＂
    "\xEF\xBC\x88",  //  （
    "\xEF\xBC\x89",  //  ）
    "\xEF\xBC\x9C",  //  ＜
    "\xEF\xBC\x9E",  //  ＞
    "\xEF\xBC\xBB",  // ［
    "\xEF\xBC\xBD",  //  ］
    "\xEF\xBD\x9B",  // ｛
    "\xEF\xBD\x9D",  //  ｝
    "\xEF\xBD\x9C",  //  ｜
    "'", "\"", "(", ")", "<", ">", "[", "]", "{", "}", "|"};

int ispunct1(const char* ch, int clen)
{
    int i;

    for (i = 0; i < countof(PUNCTS1); i++) {
        if (clen != strlen(PUNCTS1[i]))
            continue;
        if (memcmp(ch, PUNCTS1[i], clen) == 0)
            return 1;
    }

    return 0;
}

int ispunct2(const char* ch, int clen)
{
    int i;

    for (i = 0; i < countof(PUNCTS2); i++) {
        if (clen != strlen(PUNCTS2[i]))
            continue;
        if (memcmp(ch, PUNCTS2[i], clen) == 0)
            return 1;
    }

    return 0;
}

void* utf8fan2jian_init(const char* unihan)
{
    char * data, *hzbuf, *hz, *hzfan, *hzjian;
    TCMAP* map;
    int    hznum, hzlen;

    data = loadfile(unihan, NULL);
    if (!data) return NULL;

    hznum = getlinenum(data, "\n");
    map   = tcmapnew2(hznum);

    hzbuf = data;
    while ((hz = getaline(hzbuf, "\n", &hzlen)) != NULL) {
        hzjian = strchr(hz, '=');
        if (!hzjian) return NULL;
        *hzjian++ = 0;
        hzfan     = hz;
        trim(hzfan);
        trim(hzjian);
        tcmapput2(map, hzfan, hzjian);
        hzbuf = hz + hzlen + 1;
    }

    xfree(data);
    return map;
}

void utf8fan2jian(void* map, const char* fan, char* jian)
{
    uint8_t*       f = (uint8_t*)fan;
    uint8_t*       j = (uint8_t*)jian;
    uint8_t*       e;
    const uint8_t* s;
    int            len, clen;

    if (!fan || !jian) return;
    e = f + strlen(fan);

    while (f != e) {
        clen = UTF8CHLEN(*f);
        if (f + clen > e) break;
        s = tcmapget((TCMAP*)map, f, clen, &len);
        if (s) {
            memcpy(j, s, len);
            j += len;
        } else {
            memcpy(j, f, clen);
            j += clen;
        }

        f += clen;
    }

    *j = 0;
}

void utf8fan2jian_free(void* map)
{
    tcmapdel((TCMAP*)map);
}

#ifdef TEST_CHARSET
#include "xstring.h"

int main()
{
    char  utf8[1024], out[1024];
    void* map = utf8fan2jian_init("/tmp/unihan.txt");

    while (fgets(utf8, 1024, stdin)) {
        trim(utf8);
        if (strcasecmp(utf8, "exit") == 0)
            break;
        utf8fan2jian(map, utf8, utf8);
        printf("%s\n", utf8);
        /*
        utf8qj2bjext2(utf8,out);
        printf("ext2: %s\n",out);
        utf8qj2bjext(utf8,out);
        printf("ext: %s\n",out);
        utf8qj2bj(utf8,out);
        printf("%s\n",out);
        utf8qj2bjtolower(utf8,out);
        printf("tolow: %s\n",out);*/
    }

    utf8fan2jian_free(map);

    return 1;
}

#endif
