#ifndef RENDERBOX_LEX_H_H
#define RENDERBOX_LEX_H_H


#import <memory>
#import <string>


namespace renderbox {

    // ASCII info based on: http://llvm.org/svn/llvm-project/cfe/trunk/include/clang/Basic/CharInfo.h
    //                      http://llvm.org/svn/llvm-project/cfe/trunk/lib/Basic/CharInfo.cpp

    enum {
        CHAR_HORZ_WS = 0x0001,  // '\t', '\f', '\v'.  Note, no '\0'
        CHAR_VERT_WS = 0x0002,  // '\r', '\n'
        CHAR_SPACE   = 0x0004,  // ' '
        CHAR_DIGIT   = 0x0008,  // 0-9
        CHAR_XLETTER = 0x0010,  // a-f,A-F
        CHAR_UPPER   = 0x0020,  // A-Z
        CHAR_LOWER   = 0x0040,  // a-z
        CHAR_UNDER   = 0x0080,  // _
        CHAR_PERIOD  = 0x0100,  // .
        CHAR_RAWDEL  = 0x0200,  // {}[]#<>%:;?*+-/^&|~!=,"'
        CHAR_PUNCT   = 0x0400,  // `$@()
        CHAR_XUPPER  = CHAR_XLETTER | CHAR_UPPER,
        CHAR_XLOWER  = CHAR_XLETTER | CHAR_LOWER,
    };

    const uint16_t CHAR_ASCII_INFO[256] = {
        // 0 NUL         1 SOH         2 STX         3 ETX
        // 4 EOT         5 ENQ         6 ACK         7 BEL
        0           , 0           , 0           , 0           ,
        0           , 0           , 0           , 0           ,
        // 8 BS          9 HT         10 NL         11 VT
        //12 NP         13 CR         14 SO         15 SI
        0           , CHAR_HORZ_WS, CHAR_VERT_WS, CHAR_HORZ_WS,
        CHAR_HORZ_WS, CHAR_VERT_WS, 0           , 0           ,
        //16 DLE        17 DC1        18 DC2        19 DC3
        //20 DC4        21 NAK        22 SYN        23 ETB
        0           , 0           , 0           , 0           ,
        0           , 0           , 0           , 0           ,
        //24 CAN        25 EM         26 SUB        27 ESC
        //28 FS         29 GS         30 RS         31 US
        0           , 0           , 0           , 0           ,
        0           , 0           , 0           , 0           ,
        //32 SP         33  !         34  "         35  #
        //36  $         37  %         38  &         39  '
        CHAR_SPACE  , CHAR_RAWDEL , CHAR_RAWDEL , CHAR_RAWDEL ,
        CHAR_PUNCT  , CHAR_RAWDEL , CHAR_RAWDEL , CHAR_RAWDEL ,
        //40  (         41  )         42  *         43  +
        //44  ,         45  -         46  .         47  /
        CHAR_PUNCT  , CHAR_PUNCT  , CHAR_RAWDEL , CHAR_RAWDEL ,
        CHAR_RAWDEL , CHAR_RAWDEL , CHAR_PERIOD , CHAR_RAWDEL ,
        //48  0         49  1         50  2         51  3
        //52  4         53  5         54  6         55  7
        CHAR_DIGIT  , CHAR_DIGIT  , CHAR_DIGIT  , CHAR_DIGIT  ,
        CHAR_DIGIT  , CHAR_DIGIT  , CHAR_DIGIT  , CHAR_DIGIT  ,
        //56  8         57  9         58  :         59  ;
        //60  <         61  =         62  >         63  ?
        CHAR_DIGIT  , CHAR_DIGIT  , CHAR_RAWDEL , CHAR_RAWDEL ,
        CHAR_RAWDEL , CHAR_RAWDEL , CHAR_RAWDEL , CHAR_RAWDEL ,
        //64  @         65  A         66  B         67  C
        //68  D         69  E         70  F         71  G
        CHAR_PUNCT  , CHAR_XUPPER , CHAR_XUPPER , CHAR_XUPPER ,
        CHAR_XUPPER , CHAR_XUPPER , CHAR_XUPPER , CHAR_UPPER  ,
        //72  H         73  I         74  J         75  K
        //76  L         77  M         78  N         79  O
        CHAR_UPPER  , CHAR_UPPER  , CHAR_UPPER  , CHAR_UPPER  ,
        CHAR_UPPER  , CHAR_UPPER  , CHAR_UPPER  , CHAR_UPPER  ,
        //80  P         81  Q         82  R         83  S
        //84  T         85  U         86  V         87  W
        CHAR_UPPER  , CHAR_UPPER  , CHAR_UPPER  , CHAR_UPPER  ,
        CHAR_UPPER  , CHAR_UPPER  , CHAR_UPPER  , CHAR_UPPER  ,
        //88  X         89  Y         90  Z         91  [
        //92  \         93  ]         94  ^         95  _
        CHAR_UPPER  , CHAR_UPPER  , CHAR_UPPER  , CHAR_RAWDEL ,
        CHAR_PUNCT  , CHAR_RAWDEL , CHAR_RAWDEL , CHAR_UNDER  ,
        //96  `         97  a         98  b         99  c
        //100  d       101  e        102  f        103  g
        CHAR_PUNCT  , CHAR_XLOWER , CHAR_XLOWER , CHAR_XLOWER ,
        CHAR_XLOWER , CHAR_XLOWER , CHAR_XLOWER , CHAR_LOWER  ,
        //104  h       105  i        106  j        107  k
        //108  l       109  m        110  n        111  o
        CHAR_LOWER  , CHAR_LOWER  , CHAR_LOWER  , CHAR_LOWER  ,
        CHAR_LOWER  , CHAR_LOWER  , CHAR_LOWER  , CHAR_LOWER  ,
        //112  p       113  q        114  r        115  s
        //116  t       117  u        118  v        119  w
        CHAR_LOWER  , CHAR_LOWER  , CHAR_LOWER  , CHAR_LOWER  ,
        CHAR_LOWER  , CHAR_LOWER  , CHAR_LOWER  , CHAR_LOWER  ,
        //120  x       121  y        122  z        123  {
        //124  |       125  }        126  ~        127 DEL
        CHAR_LOWER  , CHAR_LOWER  , CHAR_LOWER  , CHAR_RAWDEL ,
        CHAR_RAWDEL , CHAR_RAWDEL , CHAR_RAWDEL , 0
    };

    inline bool isIdentifierBody(const char c) {
        return (CHAR_ASCII_INFO[c] & (CHAR_UPPER | CHAR_LOWER | CHAR_DIGIT | CHAR_UNDER)) != 0;
    }

    inline bool isHorizontalWhitespace(const char c) {
        return (CHAR_ASCII_INFO[c] & (CHAR_HORZ_WS | CHAR_SPACE)) != 0;
    }

    inline bool isWhitespace(const char c) {
        return (CHAR_ASCII_INFO[c] & (CHAR_HORZ_WS | CHAR_VERT_WS | CHAR_SPACE)) != 0;
    }

    struct Source {

        std::unique_ptr<char> source;
        size_t size;

        explicit Source(const char *filename) {

            FILE *f = fopen(filename, "r");

            // Determine file size
            fseek(f, 0, SEEK_END);
            long size_ = ftell(f);

            if (size_ < 0) throw 2; // Failed to tell size
            size = static_cast<size_t>(size_);

            source.reset(new char[size + 1]);

            rewind(f);
            fread(source.get(), sizeof(char), size, f);
            source.get()[size] = '\0';

        };

        explicit Source(const std::string &filename)
                : Source(filename.c_str()) {

        };

    };

}


#endif //RENDERBOX_LEX_H_H
