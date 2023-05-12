INSTALL_PATH = /usr/local
LIB_INSTALL_PATH = $(INSTALL_PATH)/lib
INC_INSTALL_PATH = $(INSTALL_PATH)/include

CC=gcc
CXX=g++
MTFlAGS=-D_REENTRANT
MACROS=
LIBS=-lpcreposix -lpcre -ltokyocabinet -liconv -lz -lpthread
LIBPATH=
CFLAGS=-fPIC -Wall $(MACROS) $(MTFlAGS)
CXXFLAGS=-fPIC -Wall $(MACROS) $(MTFlAGS)
AR=ar
ARFLAGS=rcv
RM=rm -f

LINKERNAME=libutils-c.so
SONAME=libutils-c.so.0
TARGET=libutils-c.so.0.0.1
OBJS= opt.o conf.o conf_hash.o log.o dir.o xmalloc.o xstring.o sock.o utils.o sig.o mempool.o mpool.o datime.o\
      regexp.o aes.o crypt.o md5.o sha1.o rand.o crc32.o fsfhash.o phphash.o hbm.o mwm.o xml.o category.o charset.o \
      url.o urlparse.o htmlparse.o htmlextract.o html.o htmlurl.o htmlentities.o http.o \
      recognizer.o headword.o record.o threadpool.o workqueue.o testif.o
     

all:$(TARGET) 
.PHONE:all

$(TARGET):$(OBJS)
	$(CC) -shared -Wl,-soname,$(SONAME) -o $@ $^

debug: CFLAGS += -g
debug: CXXFLAGS += -g
debug: all

install:
	cp -f  *.h  $(INC_INSTALL_PATH)
	cp -f $(TARGET) $(LIB_INSTALL_PATH)
	ln -fs $(TARGET)  $(LIB_INSTALL_PATH)/$(LINKERNAME) 

clean:
	$(RM) $(TARGET) $(OBJS)

test_log: 
	gcc -g -o log.exe -DTEST_LOG log.c -lutils-c $(LIBS)
test_conf:
	gcc -g -o conf.exe -DTEST_CONF conf.c -lutils-c $(LIBS)
test_opt:
	gcc -g -o opt.exe -DTEST_OPT opt.c -lutils-c $(LIBS)
test_category:
	gcc -g -o category.exe -DTEST_CATEGORY category.c -lutils-c $(LIBS)
test_xstring:
	gcc -g -o xstring.exe -DTEST_XSTRING xstring.c -lutils-c $(LIBS)
test_charset:
	gcc -g -o charset.exe -DTEST_CHARSET charset.c -lutils-c $(LIBS)  
test_recognizer:
	gcc -g -o recognizer.exe -DTEST_RECOGNIZER recognizer.c -lutils-c $(LIBS)
test_headword:
	gcc -g -o headword.exe -DTEST_HEADWORD headword.c -lutils-c $(LIBS)
test_record:
	gcc -g -o record.exe -DTEST_RECORD record.c -lutils-c $(LIBS)
test_sha1:
	gcc -g -o sha1.exe -DTEST_SHA1 sha1.c -lutils-c $(LIBS)
test_urlparse:
	gcc -g -o urlparse.exe -DTEST_URLPARSE urlparse.c -lutils-c $(LIBS)
test_crc32:
	gcc -g -o crc32.exe -DTEST_CRC32 crc32.c -lutils-c $(LIBS)
test_crypt:
	gcc -g -o crypt.exe -DTEST_CRYPT crypt.c -lutils-c $(LIBS)
test_datime:
	gcc -g -o datime.exe -DTEST_DATIME datime.c -lutils-c $(LIBS)
test_dir:
	gcc -g -o dir.exe -DTEST_DIR dir.c -lutils-c $(LIBS)
test_hbm:
	gcc -g -o hbm.exe -DTEST_HBM hbm.c -lutils-c $(LIBS)
test_extract:
	gcc -g -o extract.exe -DTEST_EXTRACT htmlextract.c -lutils-c $(LIBS)
test_html:
	gcc -g -o html.exe -DTEST_HTML html.c -lutils-c $(LIBS)
test_md5:
	gcc -g -o md5.exe -DTEST_MD5 md5.c -lutils-c $(LIBS)
test_rand:
	gcc -g -o rand.exe -DTEST_RAND rand.c -lutils-c $(LIBS)
test_url:
	gcc -g -o url.exe -DTEST_URL url.c -lutils-c $(LIBS)
test_utils:
	gcc -g -o utils.exe -DTEST_UTILS utils.c -lutils-c $(LIBS)
test_regexp:
	gcc -g -o regexp.exe -DTEST_REGEXP regexp.c -lutils-c $(LIBS)
test_if:
	gcc -g -o testif.exe -DTEST_IF testif.c -lutils-c $(LIBS)
test_htmlet:
	gcc -g -o htmlentities.exe -DTEST_HTMLET htmlentities.c -lutils-c $(LIBS)
