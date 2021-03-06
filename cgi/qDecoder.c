/*************************************************************
** qDecoder CGI Library v5.0.8                              **
**                                                          **
**  Official distribution site : ftp://ftp.hongik.com       **
**           Technical contact : nobreak@hongik.com         **
**                                                          **
**                        Developed by 'Seung-young Kim'    **
**                        Last updated at Mar 27, 2000      **
**                                                          **
**      Designed by Perfectionist for Perfectionist!!!      **
**                                                          **
**         Copyright (C) 1999 Hongik Internet, Inc.         **
**         Copyright (C) 1998 Nobreak Technologies, Inc.    **
**         Copyright (C) 1996,1997 Seung-young Kim          **
**                                                          **
** qDecoder is a CGI library for C/C++ language programming **
** and a solution product for developers. The Query Fetch   **
** algorithm of qDecoder which is based on linked-list gives**
** developers more simple library interface without regard  **
** to a method of GET or POST.                              **
** Also because it gives transperance with a subordinate    **
** layer, web-based softwares - CGI - is designed and       **
** embodied in reliability further. The source code for     **
** qDecoder is freely available to everyone.                **
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/stat.h>

#include "qDecoder.h"

/**********************************************
** Internal Functions Definition
**********************************************/

int  _parse_urlencoded(void);
char *_get_query(char *method);
int  _parse_multipart_data(void);
char *_fgetstring(char *buf, int maxlen, FILE *fp);

char _x2c(char hex_up, char hex_low);
char *_makeword(char *str, char stop);
char *_strtok2(char *str, char *token, char *retstop);

char *_EntryValue(Entry *first, char *name);
int  _EntryiValue(Entry *first, char *name);
void _EntryPrint(Entry *first);
void _EntryFree(Entry *first);

/**********************************************
** Static Values Definition used only internal
**********************************************/

static Entry *_first_entry = NULL;
static Entry *_cookie_first_entry = NULL;

static char  *_error_contact_info = NULL;
static char  *_error_log_filename = NULL;

static FILE  *_awkfp = NULL;
static char  _awksep = ' ';

static Entry *_multi_last_entry = NULL;
static char  _multi_last_key[1024];

/**********************************************
** Main Routines
**********************************************/

/**********************************************
** Usage : qDecoder();
** Return: Success number of values, Fail -1
** Do    : Decode Query & Save it in linked list
**         It doesn't care Method
**********************************************/
int qDecoder(void){
  int  amount = -1;
  char *content_type;

  if(_first_entry != NULL) return -1;

  content_type = "";
  content_type = getenv("CONTENT_TYPE");

  if(content_type == NULL) {
    amount = _parse_urlencoded();
  }
  /* for application/x-www-form-urlencoded */
  else if(!strncmp (content_type, "application/x-www-form-urlencoded", strlen("application/x-www-form-urlencoded"))) {
    amount = _parse_urlencoded();
  }
  /* for multipart/form-data */
  else if(!strncmp(content_type, "multipart/form-data", strlen("multipart/form-data"))) {
    amount = _parse_multipart_data();
  }
  else { /* For Oracle Web Server */
    amount = _parse_urlencoded();
  }

  return amount;
}

/* For decode application/s-www-form-urlencoded, used by qDecoder() */
int _parse_urlencoded(void){
  Entry *entries, *back;
  char *query;
  int  amount;
  int  loop;

  entries = back = NULL;

  for(amount = 0, loop = 1; loop <= 2; loop++) {
    if (loop == 1 ) query = _get_query("GET");
    else if(loop == 2 ) query = _get_query("POST");
    else break;
    for(; query && *query; amount++){
      back = entries;
      entries = (Entry *)malloc(sizeof(Entry));
      if(back != NULL) back->next = entries;
      if(_first_entry == NULL) _first_entry = entries;

      entries->value = _makeword(query, '&');
      entries->name  = _makeword(entries->value, '=');
      entries->next  = NULL;

      qURLdecode(entries->name);
      qURLdecode(entries->value);
    }
    if(query)free(query);
  }

  return amount;
}

/* For fetch query used by _parse_urlencoded() */
char *_get_query(char *method){
  char *query;
  int cl, i;

  if(!strcmp(method, "GET")){
    if(getenv("QUERY_STRING") == NULL) return NULL;
    query = strdup(getenv("QUERY_STRING"));
    /* SSI query handling */
    if(!strcmp(query, "") && getenv("REQUEST_URI") != NULL) {
      char *cp;
      for(cp = getenv("REQUEST_URI"); *cp != '\0'; cp++) {
        if(*cp == '?') { cp++; break; }
      }
      free(query);
      query = strdup(cp);
    }

    return query;
  }
  if(!strcmp(method, "POST")){
    if(getenv("REQUEST_METHOD") == NULL) return NULL;
    if(strcmp("POST", getenv("REQUEST_METHOD")))return NULL;
    if(getenv("CONTENT_LENGTH") == NULL) qError("_get_query() : Your browser sent a non-HTTP compliant message.");

    cl = atoi(getenv("CONTENT_LENGTH"));
    query = (char *)malloc(sizeof(char) * (cl + 1));
    for(i = 0; i < cl; i++)query[i] = fgetc(stdin);
    query[i] = '\0';
    return query;
  }
  return NULL;
}

/* For decode multipart/form-data, used by qDecoder() */
int _parse_multipart_data(void) {
  Entry *entries, *back;
  char *query;
  int  amount;

  char *name = NULL, *value = NULL, *filename = NULL;
  int  valuelen;

  char boundary[256],     boundaryEOF[256];
  char rnboundaryrn[256], rnboundaryEOF[256];
  int  boundarylen,        boundaryEOFlen;

  char buf[1024];
  int  c, c_count;

  int  finish;

  entries = back = NULL;

  /* For parse GET method */
  query = _get_query("GET");
  for(amount = 0; query && *query; amount++){
    back = entries;
    entries = (Entry *)malloc(sizeof(Entry));
    if(back != NULL) back->next = entries;
    if(_first_entry == NULL) _first_entry = entries;

    entries->value = _makeword(query, '&');
    entries->name  = _makeword(entries->value, '=');
    entries->next  = NULL;

    qURLdecode(entries->name);
    qURLdecode(entries->value);
  }
  if(query)free(query);

  /* For parse multipart/form-data method */
  /* find boundary string */
  sprintf(boundary,    "--%s", strstr(getenv("CONTENT_TYPE"), "boundary=") + strlen("boundary="));
  /* This is not necessary but, I can not trust MS Explore */
  qRemoveSpace(boundary);

  sprintf(boundaryEOF, "%s--", boundary);

  sprintf(rnboundaryrn, "\r\n%s\r\n", boundary);
  sprintf(rnboundaryEOF, "\r\n%s", boundaryEOF);

  boundarylen    = strlen(boundary);
  boundaryEOFlen = strlen(boundaryEOF);

  /* If you want to observe the string from stdin, enable this section */
  /* This section is made for debugging                                */
  if(0) {
    int i, j;
    qContentType("text/html");

    printf("Content Length = %s<br>\n", getenv("CONTENT_LENGTH"));
    printf("Boundary len %d : %s<br>\n", (int)strlen(boundary), boundary);
    for(i=0; boundary[i] !='\0'; i++) printf("%02X ",boundary[i]);
    printf("<p>\n");

    for(j = 1; _fgetstring(buf, sizeof(buf), stdin) != NULL; j++) {
      printf("Line %d, len %d : %s<br>\n", j, (int)strlen(buf), buf);
      for(i=0; buf[i] !='\0'; i++) printf("%02X ",buf[i]);
      printf("<p>\n");
    }
    exit(0);
  }

  /* check boundary */
  if(_fgetstring(buf, sizeof(buf), stdin) == NULL) qError("_parse_multipart_data() : Your browser sent a non-HTTP compliant message.");

  /* for explore 4.0 of NT, it sent \r\n before starting, fucking Micro$oft */
  if(!strcmp(buf, "\r\n")) _fgetstring(buf, sizeof(buf), stdin);

  if(strncmp(buf, boundary, boundarylen) != 0) qError("_parse_multipart_data() : String format invalid.");

  for(finish = 0; finish != 1; amount++){
    /* get name field */
    _fgetstring(buf, sizeof(buf), stdin);
    name = (char *)malloc(sizeof(char) * (strlen(buf) - strlen("Content-Disposition: form-data; name=\"") + 1));
    strcpy(name, buf + strlen("Content-Disposition: form-data; name=\""));
    for(c_count = 0; (name[c_count] != '\"') && (name[c_count] != '\0'); c_count++);
    name[c_count] = '\0';

    /* get filename field */
    if(strstr(buf, "; filename=\"") != NULL) {
      int erase;
      filename = (char *)malloc(sizeof(char) * (strlen(buf) - strlen("Content-Disposition: form-data; name=\"") + 1));
      strcpy(filename, strstr(buf, "; filename=\"") + strlen("; filename=\""));
      for(c_count = 0; (filename[c_count] != '\"') && (filename[c_count] != '\0'); c_count++);
      filename[c_count] = '\0';
      /* erase '\' */
      for(erase = 0, c_count = strlen(filename) - 1; c_count >= 0; c_count--) {
        if(erase == 1) filename[c_count]= ' ';
        else {
          if(filename[c_count] == '\\') {
            erase = 1;
            filename[c_count] = ' ';
          }
        }
      }
      qRemoveSpace(filename);
    }
    else filename = "";

    /* skip header */
    for(;;) {
      _fgetstring(buf, sizeof(buf), stdin);
      if (!strcmp(buf, "\r\n")) break;
    }

    /* get value field */
    for(valuelen = (1024 * 16), c_count = 0; (c = fgetc(stdin)) != EOF; ) {
      if(c_count == 0) {
        value = (char *)malloc(sizeof(char) * valuelen);
        if(value == NULL) qError("_parse_multipart_data() : Memory allocation fail.");
      }
      else if(c_count == valuelen - 1) {
        char *valuetmp;
        int  i;

        valuelen *= 2;

        /* Here, we do not use realloc(). Because sometimes it is unstable. */
        valuetmp = (char *)malloc(sizeof(char) * valuelen);
        if(valuetmp == NULL) qError("_parse_multipart_data() : Memory allocation fail.");
        for(i = 0; i < c_count; i++) valuetmp[i] = value[i];
        free(value);
        value = valuetmp;
      }
      value[c_count++] = (char)c;

      /* check end */
      if((c == '\n') || (c == '-')) {
        value[c_count] = '\0';

        if((c_count - (2 + boundarylen + 2)) >= 0) {
          if(!strcmp(value + (c_count - (2 + boundarylen + 2)), rnboundaryrn)) {
            value[c_count - (2 + boundarylen + 2)] = '\0';
            valuelen = c_count - (2 + boundarylen + 2);
            break;
          }
        }
        if((c_count - (2 + boundaryEOFlen)) >= 0) {
          if(!strcmp(value + (c_count - (2 + boundaryEOFlen)), rnboundaryEOF)) {
            value[c_count - (2 + boundaryEOFlen)] = '\0';
            valuelen = c_count - (2 + boundaryEOFlen);
            finish = 1;
            break;
          }
        }

        /* For Micro$oft Explore on MAC, they do not follow rules */
        if((c_count - (boundarylen + 2)) == 0) {
          char boundaryrn[256];
          sprintf(boundaryrn, "%s\r\n", boundary);
          if(!strcmp(value, boundaryrn)) {
            value[0] = '\0';
            valuelen = 0;
            break;
          }
        }
        if((c_count - boundaryEOFlen) == 0) {
          if(!strcmp(value, boundaryEOF)) {
            value[0] = '\0';
            valuelen = 0;
            finish = 1;
            break;
          }
        }
      }
    }

    if(c == EOF) qError("_parse_multipart_data() : Internal bug at '%s'.", name);

    /* store in linked list */
    /* store data */
    back = entries;
    entries = (Entry *)malloc(sizeof(Entry));
    if(back != NULL) back->next = entries;
    if(_first_entry == NULL) _first_entry = entries;

    entries->name  = name;
    entries->value = value;
    entries->next  = NULL;

    if(strcmp(filename, "") != 0) {
      /* store data length, 'NAME.length'*/
      back = entries;
      entries = (Entry *)malloc(sizeof(Entry));
      back->next = entries;

      entries->name  = (char *)malloc(sizeof(char) * (strlen(name) + strlen(".length") + 1));
      entries->value = (char *)malloc(sizeof(char) * 20 + 1);
      sprintf(entries->name,  "%s.length", name);
      sprintf(entries->value, "%d", valuelen);

      /* store transfer filename, 'NAME.filename'*/
      back = entries;
      entries = (Entry *)malloc(sizeof(Entry));
      back->next = entries;

      entries->name  = (char *)malloc(sizeof(char) * (strlen(name) + strlen(".filename") + 1));
      entries->value = filename;
      sprintf(entries->name,  "%s.filename", name);
      entries->next  = NULL;
    }
  }

  return amount;
}

/**********************************************
** Usage : qValue(Query Name);
** Return: Success pointer of value string, Fail NULL
** Do    : Find value string pointer
**         It find value in linked list
**********************************************/
char *qValue(char *format, ...){
  char name[1024];
  int status;
  va_list arglist;

  va_start(arglist, format);
  status = vsprintf(name, format, arglist);
  if(strlen(name) + 1 > sizeof(name) || status == EOF) qError("qValue(): Message is too long or invalid");
  va_end(arglist);

  if(_first_entry == NULL) qDecoder();
  return _EntryValue(_first_entry, name);
}

/**********************************************
** Usage : qiValue(Query Name);
** Return: Success integer of value string, Fail 0
** Do    : Find value string pointer and convert to integer
**********************************************/
int qiValue(char *format, ...){
  char name[1024];
  int status;
  va_list arglist;

  va_start(arglist, format);
  status = vsprintf(name, format, arglist);
  if(strlen(name) + 1 > sizeof(name) || status == EOF) qError("qiValue(): Message is too long or invalid");
  va_end(arglist);

  if(_first_entry == NULL) qDecoder();
  return _EntryiValue(_first_entry, name);
}

/**********************************************
** Usage : qValueDefault(Default String, Query Name);
** Return: Success pointer of value string, Fail using default string.
** Do    : If the query is not found, default string is used
**         instead.
**********************************************/
char *qValueDefault(char *defstr, char *format, ...){
  char name[1024];
  int status;
  va_list arglist;
  char *value;

  va_start(arglist, format);
  status = vsprintf(name, format, arglist);
  if(strlen(name) + 1 > sizeof(name) || status == EOF) qError("qValueNotEmpty(): Message is too long or invalid");
  va_end(arglist);

  if(_first_entry == NULL) qDecoder();
  if((value = _EntryValue(_first_entry, name)) == NULL) value = defstr;

  return value;
}

/**********************************************
** Usage : qValueNotEmpty(Error Message, Query Name);
** Return: Success pointer of value string, Fail error message.
** Do    : Find value string pointer which is not empty and NULL.
**         When the query is not found or the value string is
**         empty, error message will be shown using qError().
**********************************************/
char *qValueNotEmpty(char *errmsg, char *format, ...){
  char name[1024];
  int status;
  va_list arglist;
  char *value;

  va_start(arglist, format);
  status = vsprintf(name, format, arglist);
  if(strlen(name) + 1 > sizeof(name) || status == EOF) qError("qValueNotEmpty(): Message is too long or invalid");
  va_end(arglist);

  if(_first_entry == NULL) qDecoder();
  if((value = _EntryValue(_first_entry, name)) == NULL) qError("%s", errmsg);
  if(!strcmp(value, "")) qError("%s", errmsg);

  return value;
}

/**********************************************
** Usage : qValueFirst(Query Name);
** Return: Success pointer of first value string, Fail NULL
** Do    : Find first value string pointer
**********************************************/
char *qValueFirst(char *format, ...){
  int status;
  va_list arglist;

  va_start(arglist, format);
  status = vsprintf(_multi_last_key, format, arglist);
  if(strlen(_multi_last_key) + 1 > sizeof(_multi_last_key) || status == EOF) qError("qValueFirst(): Message is too long or invalid");
  va_end(arglist);

  if(_first_entry == NULL) qDecoder();
  _multi_last_entry = _first_entry;

  return qValueNext();
}

/**********************************************
** Usage : qValueNext();
** Return: Success pointer of next value string, Fail NULL
** Do    : Find next value string pointer
**********************************************/
char *qValueNext(void) {
  Entry *entries;

  for(entries = _multi_last_entry; entries; entries = entries->next){
    if(!strcmp(_multi_last_key, entries->name)) {
      _multi_last_entry = entries->next;
      return (entries->value);
    }
  }
  _multi_last_entry = NULL;
  strcpy(_multi_last_key, "");

  return NULL;
}

/**********************************************
** Usage : qPrint(Pointer of the first Entry);
** Do    : Print all parsed values & names for debugging
**********************************************/
void qPrint(void){
  if(_first_entry == NULL) qDecoder();
  _EntryPrint(_first_entry);
}

/**********************************************
** Usage : qFree(Pointer of the first Entry);
** Do    : Make free of linked list memory
**********************************************/
void qFree(void){
 _EntryFree(_first_entry);
}

/**********************************************
** Usage : qGetFirstEntry();
** Do    : Return _first_entry;
**********************************************/
Entry *qGetFirstEntry(void){
  if(_first_entry == NULL)qDecoder();
  return _first_entry;
}

/**********************************************
** Usage : qfDecoder(filename);
** Return: Success pointer of the first entry, Fail NULL
** Do    : Save file into linked list
           # is used for comments
**********************************************/
Entry *qfDecoder(char *filename){
  FILE  *fp;
  Entry *first, *entries, *back;
  char  *buf;

  fp = fopen(filename, "rt");
  if(fp == NULL) return NULL;

  first = entries = back = NULL;

  while((buf = qfGetLine(fp)) != NULL){
    qRemoveSpace(buf);
    if((buf[0] == '#') || (buf[0] == '\0')) continue;

    back = entries;
    entries = (Entry *)malloc(sizeof(Entry));
    if(back != NULL) back->next = entries;
    if(first == NULL) first = entries;

    entries->value = strdup(buf);
    entries->name  = _makeword(entries->value, '=');
    entries->next  = NULL;

    qRemoveSpace(entries->name);
    qRemoveSpace(entries->value);

    free(buf);
  }

  fclose(fp);
  return first;
}

char *qfValue(Entry *first, char *format, ...){
  char name[1024];
  int status;
  va_list arglist;

  va_start(arglist, format);
  status = vsprintf(name, format, arglist);
  if(strlen(name) + 1 > sizeof(name) || status == EOF) qError("qfValue(): Message is too long or invalid");
  va_end(arglist);

  return _EntryValue(first, name);
}

int qfiValue(Entry *first, char *format, ...){
  char name[1024];
  int status;
  va_list arglist;

  va_start(arglist, format);
  status = vsprintf(name, format, arglist);
  if(strlen(name) + 1 > sizeof(name) || status == EOF) qError("qfiValue(): Message is too long or invalid");
  va_end(arglist);

  return _EntryiValue(first, name);
}

void qfPrint(Entry *first){
  _EntryPrint(first);
}

void qfFree(Entry *first){
  _EntryFree(first);
}

/**********************************************
** Usage : qsDecoder(string);
** Return: Success pointer of the first entry, Fail NULL
** Do    : Save string into linked list
           # is used for comments
**********************************************/
Entry *qsDecoder(char *str){
  Entry *first, *entries, *back;
  char  *org, *buf, *offset;
  int  eos;

  if(str == NULL) return NULL;

  first = entries = back = NULL;

  if((org = strdup(str)) == NULL) qError("qsDecoder(): Memory allocation fail.");

  for(buf = offset = org, eos = 0; eos == 0; ) {
    for(buf = offset; *offset != '\n' && *offset != '\0'; offset++);
    if(*offset == '\0') eos = 1;
    else *offset = '\0', offset++;

    qRemoveSpace(buf);
    if((buf[0] == '#') || (buf[0] == '\0')) continue;

    back = entries;
    entries = (Entry *)malloc(sizeof(Entry));
    if(back != NULL) back->next = entries;
    if(first == NULL) first = entries;

    entries->value = strdup(buf);
    entries->name  = _makeword(entries->value, '=');
    entries->next  = NULL;

    qRemoveSpace(entries->name);
    qRemoveSpace(entries->value);
  }

  free(org);

  return first;
}

char *qsValue(Entry *first, char *format, ...){
  char name[1024];
  int status;
  va_list arglist;

  va_start(arglist, format);
  status = vsprintf(name, format, arglist);
  if(strlen(name) + 1 > sizeof(name) || status == EOF) qError("qsValue(): Message is too long or invalid");
  va_end(arglist);

  return _EntryValue(first, name);
}

int qsiValue(Entry *first, char *format, ...){
  char name[1024];
  int status;
  va_list arglist;

  va_start(arglist, format);
  status = vsprintf(name, format, arglist);
  if(strlen(name) + 1 > sizeof(name) || status == EOF) qError("qsiValue(): Message is too long or invalid");
  va_end(arglist);

  return _EntryiValue(first, name);
}

void qsPrint(Entry *first){
  _EntryPrint(first);
}

void qsFree(Entry *first){
  _EntryFree(first);
}

/**********************************************
** Usage : qcDecoder();
** Return: Success number of values, Fail -1
** Do    : Decode COOKIES & Save it in linked list
**********************************************/
int qcDecoder(void){
  Entry *entries, *back;
  char *query;
  int  amount;

  if(_cookie_first_entry != NULL) return -1;

  if(getenv("HTTP_COOKIE") == NULL) return 0;
  query = strdup(getenv("HTTP_COOKIE"));

  entries = back = NULL;

  for(amount = 0; *query; amount++){
    back = entries;
    entries = (Entry *)malloc(sizeof(Entry));
    if(back != NULL) back->next = entries;
    if(_cookie_first_entry == NULL) _cookie_first_entry = entries;

    entries->value = _makeword(query, ';');
    entries->name  = _makeword(entries->value, '=');
    entries->next  = NULL;

    qURLdecode(entries->name);
    qURLdecode(entries->value);
    qRemoveSpace(entries->name);
  }
  free(query);

  return amount;
}

char *qcValue(char *format, ...){
  char name[1024];
  int status;
  va_list arglist;

  va_start(arglist, format);
  status = vsprintf(name, format, arglist);
  if(strlen(name) + 1 > sizeof(name) || status == EOF) qError("qcValue(): Message is too long or invalid");
  va_end(arglist);

  if(_cookie_first_entry == NULL) qcDecoder();
  return _EntryValue(_cookie_first_entry, name);
}

int qciValue(char *format, ...){
  char name[1024];
  int status;
  va_list arglist;

  va_start(arglist, format);
  status = vsprintf(name, format, arglist);
  if(strlen(name) + 1 > sizeof(name) || status == EOF) qError("qciValue(): Message is too long or invalid");
  va_end(arglist);

  if(_cookie_first_entry == NULL) qcDecoder();
  return _EntryiValue(_cookie_first_entry, name);
}

void qcPrint(void){
  if(_cookie_first_entry == NULL)qcDecoder();
  _EntryPrint(_cookie_first_entry);
}

void qcFree(void){
 _EntryFree(_cookie_first_entry);
}

/**********************************************
** Usage : qSetCookie();
** Do    : Set Cookie
**
** The 'exp_days' is number of days which expire the cookie
** The current time + exp_days will be set
** This is must called before qContentType();
**
** ex) qSetCookie("NAME", "Kim", 30, NULL, NULL, NULL);
**********************************************/
void qSetCookie(char *name, char *value, int exp_days, char *path, char *domain, char *secure){
  char *Name, *Value;
  char cookie[(4 * 1024) + 256];

  /* Name=Value */
  Name = qURLencode(name), Value = qURLencode(value);
  sprintf(cookie, "%s=%s", Name, Value);
  free(Name), free(Value);

  if(exp_days != 0) {
    time_t plus_sec;
    char gmt[256];
    plus_sec = (time_t)(exp_days * 24 * 60 * 60);
    qGetGMTime(gmt, plus_sec);
    strcat(cookie, "; expires=");
    strcat(cookie, gmt);
  }

  if(path != NULL) {
    if(path[0] != '/') qError("qSetCookie(): Path string(%s) must start with '/' character.", path);
    strcat(cookie, "; path=");
    strcat(cookie, path);
  }

  if(domain != NULL) {
    if(strstr(domain, "/") != NULL || strstr(domain, ".") == NULL) qError("qSetCookie(): Invalid domain name(%s).", domain);
    strcat(cookie, "; domain=");
    strcat(cookie, domain);
  }

  if(secure != NULL) {
    strcat(cookie, "; secure");
  }

  printf("Set-Cookie: %s\n", cookie);
}

/**********************************************
** Usage : qAwkOpen(filename, field-separator);
** Return: Success 1, Fail 0
** Do    : Open file to scan pattern. (similar to unix command awk)
** ex) qAwkOpen("source.dat", ':');
**********************************************/
int qAwkOpen(char *filename, char separator) {
  if (_awkfp != NULL) qError("qAwkOpen(): There is already opened handle.");
  if((_awkfp = fopen(filename, "rt")) == NULL) return 0;
  _awksep = separator;
  return 1;
}

/**********************************************
** Usage : qAwkNext(array);
** Return: Success number of field, End of file -1
** Do    : Scan one line (Unlimited line length)
** ex) char array[10][256];
**     qAwkNext(array);
**********************************************/
int qAwkNext(char array[][256]) {
  char *buf;
  char *bp1, *bp2;
  int i, exitflag;

  if (_awkfp == NULL) qError("qAwkNext(): There is no opened handle.");
  if((buf = qRemoveSpace(qfGetLine(_awkfp))) == NULL) return -1;

  for(i = exitflag = 0, bp1 = bp2 = buf; exitflag == 0; i++) {
    for(; *bp2 != _awksep && *bp2 != '\0'; bp2++);
    if(*bp2 == '\0') exitflag = 1;
    *bp2 = '\0';
    strcpy(array[i], bp1);
    bp1 = ++bp2;
  }
  free(buf);
  return i;
}

/**********************************************
** Usage : qAwkClose();
** Do    : Close file.
**********************************************/
void qAwkClose(void) {
  if (_awkfp == NULL) qError("qAwkClose(): There is no opened handle.");
  fclose(_awkfp);
  _awkfp = NULL;
}

#define	SSI_INCLUDE_START	"<!--#include file=\""
#define SSI_INCLUDE_END		"\"-->"
/**********************************************
** Usage : qSedStr(string pointer, fpout, arg) {
** Return: Success 1, Fail 0
** Do    : Stream Editor.
**********************************************/
int qSedStr(char *srcstr, FILE *fpout, char **arg) {
  int argc, i, j, k;
  char *mode, **name, **value;
  char sep, *tmp;

  if(srcstr == NULL) return 0;

  /* Arguments count */
  if(arg == NULL) argc = 0;
  else for(argc = 0; arg[argc] != NULL; argc++);

  /* Memory allocation */
  if(argc == 0) {
    mode = NULL;
    name = value = NULL;
  }
  else {
    mode  = (char *)malloc(sizeof(char) * argc);
    name  = (char **)malloc(sizeof(char *) * argc);
    value = (char **)malloc(sizeof(char *) * argc);
  }

  /* Make arguments comparision list */
  for(i = 0; i < argc; i++) {
    mode[i] = arg[i][0];
    sep = arg[i][1];

    /* name */
    name[i] = (char *)malloc(strlen(arg[i]) + 1);
    for(j = 2, k = 0; arg[i][j] != sep; j++, k++) {
      if (arg[i][j] == '\0') qError("qSedStr(): Premature end of argument '%s'.", arg[i]);
      name[i][k] = arg[i][j];
    }
    name[i][k] = '\0';

    /* value */
    value[i] = (char *)malloc(strlen(arg[i]) + 1);
    for(j++, k = 0; arg[i][j] != sep; j++, k++) {
      if (arg[i][j] == '\0') qError("qSedStr(): Premature end of argument '%s'.", arg[i]);
      value[i][k] = arg[i][j];
    }
    value[i][k] = '\0';
  }

  /* Parsing */
  for(tmp = srcstr; *tmp != '\0'; tmp++) {
    int flag;

    /* SSI invocation */
    if(!strncmp(tmp, SSI_INCLUDE_START, strlen(SSI_INCLUDE_START))) {
      char ssi_inc_file[1024], *endp;
      if((endp = strstr(tmp, SSI_INCLUDE_END)) != NULL) {
        tmp += strlen(SSI_INCLUDE_START);
        strncpy(ssi_inc_file, tmp, endp - tmp);
        ssi_inc_file[endp-tmp] = '\0';
        tmp = (endp + strlen(SSI_INCLUDE_END)) - 1;

        if(qCheckFile(ssi_inc_file) == 1) qSedFile(ssi_inc_file, fpout, arg);
        else printf("[qSedStr: an error occurred while processing 'include' directive - file(%s) open fail]", ssi_inc_file);
      }
      else printf("[qSedStr: an error occurred while processing 'include' directive - ending tag not found]");
      continue;
    }

    /* Pattern Matching */
    for(i = flag = 0; i < argc && flag == 0; i++) {
      switch(mode[i]) {
        case 's': {
          if(!strncmp(tmp, name[i], strlen(name[i]))) {
            fprintf(fpout, "%s", value[i]);
            tmp += strlen(name[i]) - 1;
            flag = 1;
          }
          break;
        }
        default : {
          qError("qSedStr(): Unknown mode '%c'.", mode[i]);
          break;
        }
      }
      if(flag == 1) break;  /* No more comparison is necessary. */
    }
    if(flag == 0) fprintf(fpout, "%c", *tmp);
  }

  /* Free */
  for(i = 0; i < argc; i++) {
    free(name[i]);
    free(value[i]);
  }
  if (mode  != NULL) free(mode);
  if (name  != NULL) free(name);
  if (value != NULL) free(value);

  return 1;
}

/**********************************************
** Usage : qSedFile(filename, fpout, arg) {
** Return: Success 1, Fail open fail 0
** Do    : Stream Editor.
**********************************************/
int qSedFile(char *filename, FILE *fpout, char **arg) {
  char *sp;
  int flag;

  if((sp = qReadFile(filename, NULL)) == NULL) return 0;
  flag = qSedStr(sp, fpout, arg);
  free(sp);

  return flag;
}

/**********************************************
** Usage : qArgMake(Query string, Token Array);
** Return: number of parsed tokens
** Do    : This function parses query string and stores
**         each query into token array.
** ex) char *query="I am a \"pretty girl\".", *qlist[MAX_TOKENS];
**     int queries;
**     queries = qArgMake(query, qlist);
**********************************************/
int qArgMake(char *str, char **qlist) {
  char *query, *sp, *qp;
  int argc;

  query = sp = qRemoveSpace(strdup(str));

  for(argc = 0; *sp != '\0';) {
    switch(*sp) {
      case ' ': { /* skip space */
      	sp++;
        break;
      }
      case '"': { /* Double quotation arounded string */
      	qlist[argc] = qp = (char *)malloc(strlen(query) +  1);
      	for(sp++; *sp != '\0'; sp++, qp++) {
          if(*sp == '"') { sp++; break; }
      	  *qp = *sp;
      	}
      	*qp = '\0';
      	if(strlen(qlist[argc]) > 0) argc++;
      	break;
      }
      default: {
      	qlist[argc] = qp = (char *)malloc(strlen(query) +  1);
      	for(; *sp != '\0'; sp++, qp++) {
          if(*sp == ' ' || *sp == '"') break;
      	  *qp = *sp;
      	}
      	*qp = '\0';
      	argc++;
      	break;
      }
    }
  }
  qlist[argc] = NULL;
  free(query);
  return argc;
}

/**********************************************
** Usage : qArgFree(Pointer of Token Array);
** Do    : Free malloced token array.
**********************************************/
void qArgFree(char **qlist) {
  char **qp;
  for(qp = qlist; *qp; qp++) free(*qp);
  *qlist = NULL;
}

/**********************************************
** Usage : qArgFree(Pointer of Token Array);
** Do    : Print all parsed tokens for debugging.
**********************************************/
void qArgPrint(char **qlist) {
  char **qp;

  qContentType("text/html");

  for(qp = qlist; *qp; qp++) {
    printf("'%s' (%d bytes)<br>\n" , *qp, strlen(*qp));
  }
}

/**********************************************
** Usage : qArgMatch(Target string, Token Array);
** Return: Number of token matched. (Do not allow duplicated token matches)
**         This value is not equal to a value of qArgEmprint()
**********************************************/
int qArgMatch(char *str, char **qlist) {
  char **qp;
  int i;

  /* To keep the value i over zero */
  if(!*qlist) return 0;

  for(qp = qlist, i = 0; *qp != NULL; qp++) if(qStristr(str, *qp)) i++;

  return i;
}

/**********************************************
** Usage : qArgEmprint(mode, Target string, Token Array);
** Return: Number of matches (Allow duplicated token matches)
**         This value is not equal to a value of qArgMatch().
** Do    : Make matched token bold in target string.
**********************************************/
int qArgEmprint(int mode, char *str, char **qlist) {
  char *sp, *freestr, *buf, *bp, *op;
  int  i, j, flag, matches;

  if(!*qlist) {
    qPuts(mode, str);
    return 0;
  }

  /* Set character pointer */
  op = str;
  sp = freestr = strdup(str);
  qStrupr(sp);

  if((bp = buf = (char *)malloc(strlen(str) + 1)) == NULL) qError("Memory allocation fail.");

  for(matches = 0; *sp != '\0';) {
    for(i = 0, flag = 0; qlist[i] != NULL; i++) {
      if(!qStrincmp(sp, qlist[i], strlen(qlist[i]))) {
        *bp = '\0'; /* Mark string end */
        qPuts(mode, buf); /* flash buffer */
        bp = buf; /* reset buffer pointer */
      	printf("<b>");
        for(j = 1; j <= strlen(qlist[i]); j++) {
          printf("%c", *op++);
          sp++;
        }
      	printf("</b>");
      	flag = 1;
      	matches++;
      	break;
      }
    }
    if(flag == 0) {
      *bp++ = *op++;
      sp++;
    }
  }
  *bp = '\0'; /* Mark string end */
  qPuts(mode, buf); /* Flash buffer */

  free(buf);
  free(freestr);

  return matches;
}

/**********************************************
** Usage : qURLencode(string to encode);
** Return: Pointer of encoded str which is memory allocated.
** Do    : Encode string
**********************************************/
char *qURLencode(char *str){
  char *encstr, buf[2+1];
  int i, j;
  unsigned char c;
  if(str == NULL) return NULL;
  if((encstr = (char *)malloc((strlen(str) * 3) + 1)) == NULL) return NULL;

  for(i = j = 0; str[i]; i++){
    c = (unsigned char)str[i];
         if ((c >= '0') && (c <= '9')) encstr[j++] = c;
    else if ((c >= 'A') && (c <= 'Z')) encstr[j++] = c;
    else if ((c >= 'a') && (c <= 'z')) encstr[j++] = c;
    else if ((c == '@') || (c == '.') || (c == '/') || (c == '_') || (c == '-')) encstr[j++] = c;
    else {
      sprintf(buf, "%02x", c);
      encstr[j++] = '%';
      encstr[j++] = buf[0];
      encstr[j++] = buf[1];
    }
  }
  encstr[j] = '\0';

  return encstr;
}

/**********************************************
** Usage : qURLdecode(Query Pointer);
** Return: Pointer of Query string
** Do    : Decode query string
**********************************************/
char *qURLdecode(char *str){
  int i, j;

  if(!str) return NULL;
  for(i = j = 0; str[j]; i++, j++){
    switch(str[j]){
      case '+':{
        str[i] = ' ';
        break;
      }
      case '%':{
        str[i] = _x2c(str[j + 1], str[j + 2]);
        j += 2;
        break;
      }
      default:{
        str[i] = str[j];
        break;
      }
    }
  }
  str[i]='\0';

  return str;
}

/**********************************************
** Usage : qContentType(Mime Type);
** Do    : Print Content Type Once
**********************************************/
void qContentType(char *mimetype){
  static int flag = 0;

  if(flag)return;

  printf("Content-Type: %s%c%c", mimetype, 10, 10);
  flag = 1;
}

/**********************************************
** Usage : qPrintf(Mode, Format, Arg);
** Return: Sucess number of output bytes, Fail EOF
** Do    : Print message like printf
**         Mode : see qPuts()
**********************************************/
int qPrintf(int mode, char *format, ...){
  char buf[1024];
  int  status;
  va_list arglist;

  va_start(arglist, format);
  if((status = vsprintf(buf, format, arglist)) == EOF) return status;
  if(strlen(buf) + 1 > sizeof(buf))qError("qPrintf(): Message is too long");

  qPuts(mode, buf);

  return status;
}

/**********************************************
** Usage : qPuts(Mode, String pointer);
** Do    : print HTML link as multi mode
**         Mode 0  : Same as printf()
**         Mode 1  : Print HTML TAG
**         Mode 2  : Mode 1 + Auto Link
**         Mode 3  : Mode 2 + Auto Link to _top frame
**         Mode 4  : Waste HTML TAG
**         Mode 5  : Mode 4 + Auto Link
**         Mode 6  : Mode 5 + Auto Link to _top frame
**
**         Mode 10  :Mode 0 + Convert
**         Mode 11 : Print HTML TAG + Convert
**         Mode 12 : Mode 11 + Auto Link
**         Mode 13 : Mode 12 + Auto Link to _top frame
**         Mode 14 : Waste HTML TAG + Convert
**         Mode 15 : Mode 14 + Auto Link
**         Mode 16 : Mode 15 + Auto Link to _top frame
**
**         Convert : " "   -> " "
**                   "  "  -> " &nbsp;"
**                   "   " -> " &nbsp;&nbsp;"
**                   "\n"   -> "<br>\n"
**                   "\r\n" -> "<br>\n"
**
** You can use 1x mode, to wrap long lines with no <pre> tag.
** Note) It modify argument string...
**********************************************/
void qPuts(int mode, char *buf){

  if(buf == NULL) return;

  if(mode == 0) printf("%s", buf);
  else if(mode == 10) {
    int i;
    for(i = 0; buf[i] != '\0'; i++) {
      switch(buf[i]) {
        case ' '  : {
          if((i > 0) && (buf[i - 1] == ' ')) printf("&nbsp;");
          else printf(" ");
          break;
        }
        case '\r' : {
          break;
        }
        case '\n' : {
          printf("<br>\n");
          break;
        }
        default   : {
          printf("%c", buf[i]);
          break;
        }
      }
    }
  }
  else {
    char *ptr, retstop, lastretstop, *target, *token;
    int printhtml, autolink, convert, linkflag, ignoreflag;

    /* set defaults, mode 2*/
    printhtml = 1;
    autolink  = 1;
    target    = "_top";
    convert   = 0;

    switch(mode){
      case 1  : {printhtml = 1, autolink = 0, target = "";     convert = 0; break;}
      case 2  : {printhtml = 1, autolink = 1, target = "";     convert = 0; break;}
      case 3  : {printhtml = 1, autolink = 1, target = "_top"; convert = 0; break;}
      case 4  : {printhtml = 0, autolink = 0, target = "";     convert = 0; break;}
      case 5  : {printhtml = 0, autolink = 1, target = "";     convert = 0; break;}
      case 6  : {printhtml = 0, autolink = 1, target = "_top"; convert = 0; break;}
      case 11 : {printhtml = 1, autolink = 0, target = "";     convert = 1; break;}
      case 12 : {printhtml = 1, autolink = 1, target = "";     convert = 1; break;}
      case 13 : {printhtml = 1, autolink = 1, target = "_top"; convert = 1; break;}
      case 14 : {printhtml = 0, autolink = 0, target = "";     convert = 1; break;}
      case 15 : {printhtml = 0, autolink = 1, target = "";     convert = 1; break;}
      case 16 : {printhtml = 0, autolink = 1, target = "_top"; convert = 1; break;}

      default: {qError("_autolink() : Invalid Mode (%d)", mode); break;}
    }

    token = " `(){}[]<>&\"',\r\n";
    lastretstop = '0'; /* any character except space */
    ptr = _strtok2(buf, token, &retstop);

    for(linkflag = ignoreflag = 0; ptr != NULL;){

      /* auto link */
      if(ignoreflag == 0) {
        if(autolink == 1){
          if(!strncmp(ptr, "http://",        7)) linkflag = 1;
          else if(!strncmp(ptr, "ftp://",    6)) linkflag = 1;
          else if(!strncmp(ptr, "telnet://", 9)) linkflag = 1;
          else if(!strncmp(ptr, "news:",     5)) linkflag = 1;
          else if(!strncmp(ptr, "mailto:",   7)) linkflag = 1;
          else if(qCheckEmail(ptr) == 1)         linkflag = 2;
          else linkflag = 0;
        }
        if(linkflag == 1) printf("<a href=\"%s\" target=\"%s\">%s</a>", ptr, target, ptr);
        else if(linkflag == 2) printf("<a href=\"mailto:%s\">%s</a>", ptr, ptr);
        else printf("%s", ptr);
      }

      /* print */
      if(printhtml == 1){
        if     (retstop == '<')  printf("&lt;");
        else if(retstop == '>')  printf("&gt;");
        else if(retstop == '\"') printf("&quot;");
        else if(retstop == '&')  printf("&amp;");

        else if(retstop == ' '  && convert == 1) {
          if(lastretstop == ' ' && strlen(ptr) == 0) printf("&nbsp;");
          else printf(" ");
        }
        else if(retstop == '\r' && convert == 1); /* skip when convert == 1 */
        else if(retstop == '\n' && convert == 1) printf("<br>\n");

        else if(retstop != '\0') printf("%c", retstop);
      }
      else {
        if     (retstop == '<') ignoreflag = 1;
        else if(retstop == '>') ignoreflag = 0;

        else if(retstop == '\"' && ignoreflag == 0) printf("&quot;");
        else if(retstop == '&'  && ignoreflag == 0) printf("&amp;");

        else if(retstop == ' '  && ignoreflag == 0 && convert == 1) {
          if(lastretstop == ' ' && strlen(ptr) == 0) printf("&nbsp;");
          else printf(" ");
        }
        else if(retstop == '\r' && ignoreflag == 0 && convert == 1); /* skip when convert == 1 */
        else if(retstop == '\n' && ignoreflag == 0 && convert == 1) printf("<br>\n");

        else if(retstop != '\0' && ignoreflag == 0) printf("%c", retstop);

      }

      lastretstop = retstop;
      ptr = _strtok2(NULL, token, &retstop);
    }
  }
}

/**********************************************
** Usage : qError(Format, Arg);
** Do    : Print error message
**********************************************/
void qError(char *format, ...){
  char buf[1024];
  int status;
  int logstatus;
  va_list arglist;

  va_start(arglist, format);

  status = vsprintf(buf, format, arglist);
  if(strlen(buf) + 1 > sizeof(buf) || status == EOF){
    printf("qError(): Message is too long or invalid");
    exit(1);
  }

  logstatus = 0;
  if(_error_log_filename != NULL) {
    FILE *fp;

    if((fp = fopen(_error_log_filename, "at")) == NULL) logstatus = -1;
    else {
      char *http_user_agent, *remote_host;
      struct tm *time;

      time = qGetTime();

      if((http_user_agent = getenv("HTTP_USER_AGENT")) == NULL) http_user_agent = "null";
      if((remote_host     = getenv("REMOTE_HOST"))     == NULL) {
        /* Fetch for Apache 1.3 */
        if((remote_host     = getenv("REMOTE_ADDR"))   == NULL) remote_host = "null";
      }

      fprintf(fp, "%04d/%02d/%02d(%02d:%02d) : '%s' from %s (%s)\n",
              time->tm_year, time->tm_mon, time->tm_mday, time->tm_hour, time->tm_min,
              buf, remote_host, http_user_agent);

      fclose(fp);
      logstatus = 1;
    }
  }

  if(getenv("REMOTE_ADDR") == NULL)  {
    printf("Error: %s\n", buf);
  }
  else {
    qContentType("text/html");

    if(_error_contact_info != NULL) {
      strcat(buf, _error_contact_info);
    }
    if(logstatus == -1) strcat(buf, " [ERROR LOGGING FAIL]");

    printf("<html>\n");
    printf("<head>\n");
    printf("<title>Error: %s</title>\n", buf);
    printf("<script language='JavaScript'>\n");
    printf("  alert(\"%s\");\n", buf);
    printf("  history.back();\n");
    printf("</script>\n");
    printf("</head>\n");
    printf("</html>\n");
  }

  qFree();
  qcFree();

  exit(1);
}

/**********************************************
** Usage : qErrorLog(logFilename);
** Do    : Turn Error log on
**********************************************/
void qErrorLog(char *filename) {
  _error_log_filename = filename;
}

/**********************************************
** Usage : qErrorContact(logFilename);
** Do    : Error contact information
**********************************************/
void qErrorContact(char *msg) {
  _error_contact_info = msg;
}


/**********************************************
** Usage : qCgienv(Pointer of Cgienv);
** Do    : Get environment of CGI
**********************************************/
void qCgienv(Cgienv *env){
  struct tm *envtime;

  envtime = qGetTime();

  env->auth_type         = qGetEnv("AUTH_TYPE", NULL);
  env->content_length    = qGetEnv("CONTENT_LENGTH", NULL);
  env->content_type      = qGetEnv("CONTENT_TYPE", NULL);
  env->document_root     = qGetEnv("DOCUMENT_ROOT", NULL);
  env->gateway_interface = qGetEnv("GATEWAY_INTERFACE", NULL);
  env->http_accept       = qGetEnv("HTTP_ACCEPT", NULL);
  env->http_cookie       = qGetEnv("HTTP_COOKIE", NULL);
  env->http_user_agent   = qGetEnv("HTTP_USER_AGENT", NULL);
  env->query_string      = qGetEnv("QUERY_STRING", NULL);
  env->remote_addr       = qGetEnv("REMOTE_ADDR", NULL);
  env->remote_host       = qGetEnv("REMOTE_HOST", env->remote_addr);
  env->remote_user       = qGetEnv("REMOTE_USER", NULL);
  env->remote_port       = qGetEnv("REMOTE_PORT", NULL);
  env->request_method    = qGetEnv("REQUEST_METHOD", NULL);
  env->script_name       = qGetEnv("SCRIPT_NAME", NULL);
  env->script_filename   = qGetEnv("SCRIPT_FILENAME", NULL);
  env->server_name       = qGetEnv("SERVER_NAME", NULL);
  env->server_protocol   = qGetEnv("SERVER_PROTOCOL", NULL);
  env->server_port       = qGetEnv("SERVER_PORT", NULL);
  env->server_software   = qGetEnv("SERVER_SOFTWARE", NULL);
  env->server_admin      = qGetEnv("SERVER_ADMIN", NULL);

  /* qDecoder Supported Extended Informations */
  env->year = envtime->tm_year;
  env->mon  = envtime->tm_mon;
  env->day  = envtime->tm_mday;
  env->hour = envtime->tm_hour;
  env->min  = envtime->tm_min;
  env->sec  = envtime->tm_sec;
}

/**********************************************
** Usage : qGetEnv(Environment string name, Fail return string pointer);
** Return: Environment string or 'nullstr'.
** Do    : Get environment string.
**         When 'getenv' does not find 'envname', it will return 'nullstr.
**********************************************/
char *qGetEnv(char *envname, char *nullstr) {
  char *envstr;

  if((envstr = getenv(envname)) != NULL) return envstr;

  return nullstr;
}

/**********************************************
** Usage : qCGIname();
** Return: CGI filename.
**********************************************/
char *qCGIname(void) {
  static char cgi_name[256];
  char *c;

  if(getenv("SCRIPT_NAME") == NULL) return NULL;

  strcpy(cgi_name, getenv("SCRIPT_NAME"));

  /* Fetch filename in string which include directory name */
  for(c = cgi_name + strlen(cgi_name) - 1; c >= cgi_name && !(*c == '/' || *c == '\\'); c--);
  for(; c >= cgi_name; c--) *c = ' ';
  qRemoveSpace(cgi_name);

  return cgi_name;
}

/**********************************************
** Usage : qGetTime(void);
** Return: Pointer of struct tm
** Do    : Get Time
**********************************************/
struct tm *qGetTime(void){
  time_t nowtime;
  static struct tm *nowlocaltime;

  time(&nowtime);
  nowlocaltime = localtime(&nowtime);
  nowlocaltime->tm_year += 1900;
  nowlocaltime->tm_mon++;

  return nowlocaltime;
}

/**********************************************
** Usage : qGetGMTime(gmt, plus_sec);
** Do    : Make string of GMT Time for Cookie
** Return: Amount second from 1970/00/00 00:00:00
**
** plus_sec will be added to current time;
**********************************************/
time_t qGetGMTime(char *gmt, time_t plus_sec) {
  time_t nowtime;
  struct tm *nowgmtime;

  nowtime = time(NULL);
  nowtime += plus_sec;
  nowgmtime = gmtime(&nowtime);

  strftime(gmt, 256, "%a, %d-%b-%Y %H:%M:%S %Z", nowgmtime);

  return nowtime;
}

/**********************************************
** Usage : qCheckFile(Filename);
** Return: If file exist, return 1. Or return 0;
** Do    : Check filethat file is existGet environment of CGI
**********************************************/
int qCheckFile(char *filename){
  FILE *fp;
  fp = fopen(filename, "rb");
  if(fp == NULL) return 0;
  fclose(fp);
  return 1;
}

/**********************************************
** Usage : qCatFile(filename);
** Return: Success number of characters, Fail -1
** Do    : Stream out file.
**********************************************/
int qCatFile(char *filename) {
  FILE *fp;
  int c, counter;

  if((fp = fopen(filename, "rb")) == NULL) return -1;
  for(counter = 0; (c = fgetc(fp)) != EOF; counter++) {
    printf("%c", c);
  }

  fclose(fp);
  return counter;
}

/**********************************************
** Usage : qReadFile(filename, string pointer);
** Return: Success stream pointer, Fail NULL
** Do    : Read file to malloced memory.
**********************************************/
char *qReadFile(char *filename, int *size) {
  FILE *fp;
  struct stat fstat;
  char *sp, *tmp;
  int c, i;

  if(size != NULL) *size = 0;
/*  if (lstat(filename, &fstat) < 0) return NULL;*/
  if((fp = fopen(filename, "rb")) == NULL) return NULL;

  sp = (char *)malloc(fstat.st_size + 1);
  for(tmp = sp, i = 0; (c = fgetc(fp)) != EOF; tmp++, i++) *tmp = (char)c;
  *tmp = '\0';

  if(fstat.st_size != i) qError("qReadFile: Size(File:%d, Readed:%s) mismatch.", fstat.st_size, i);
  fclose(fp);
  if(size != NULL) *size = i;
  return sp;
}

/**********************************************
** Usage : qSaveStr(string pointer, string size, filename, mode, permission)
** Return: Success number bytes stored, File open fail -1,
           Can not adjust permission -2
** Do    : Store string to file.
**********************************************/
int qSaveStr(char *sp, int spsize, char *filename, char *mode, mode_t perm) {
  FILE *fp;
  int i;
  if((fp = fopen(filename, mode)) == NULL) return -1;
  for(i = 0; i < spsize; i++) fputc(*sp++, fp);
  fclose(fp);
  if(chmod(filename, perm) != 0) return -2;

  return i;
}

/*********************************************
** Usage : qfGetLine(fp);
** Return: Success string pointer, End of file NULL
** Do    : Read one line from file pointer and alocate
**         memory to save string. And there is no limit
**         about length.
**********************************************/
char *qfGetLine(FILE *fp) {
  int memsize;
  int c, c_count;
  char *string = NULL;

  for(memsize = 1024, c_count = 0; (c = fgetc(fp)) != EOF;) {
    if(c_count == 0) {
      string = (char *)malloc(sizeof(char) * memsize);
      if(string == NULL) qError("qfGetLine() : Memory allocation fail.");
    }
    else if(c_count == memsize - 1) {
      char *stringtmp;
      int  i;

      memsize *= 2;

      /* Here, we do not use realloc(). Because sometimes it is unstable. */
      stringtmp = (char *)malloc(sizeof(char) * (memsize + 1));
      if(stringtmp == NULL) qError("qfGetLine() : Memory allocation fail.");
      for(i = 0; i < c_count; i++) stringtmp[i] = string[i];
      free(string);
      string = stringtmp;
    }
    string[c_count++] = (char)c;
    if((char)c == '\n') break;
  }

  if(c_count == 0 && c == EOF) return NULL;

  string[c_count] = '\0';

  return string;
}

/**********************************************
** Usage : qDownload(filename);
** Do    : Pump file to stdout, do not call qContentType().
** Return: Success 1, File not found 0
**********************************************/
int qDownload(char *filename) {
  return qDownloadMime(filename, "application/octet-stream");
}

/**********************************************
** Usage : qDownloadMime(filename, mime);
** Do    : Pump file to stdout, should not call qContentType().
**         if mime is 'application/octet-stream', forcing download.
** Return: Success 1, File not found 0
**********************************************/
int qDownloadMime(char *filename, char *mime) {
  char *file, *c, *disposition;

  if(mime == NULL) mime = "application/octet-stream";

  if(filename == NULL) qError("qDownload(): Null pointer can not be used.");
  if(qCheckFile(filename) == 0) return 0;

  file = strdup(filename);

  /* Fetch filename in string which include directory name */
  for(c = file + strlen(file) - 1; c >= file && !(*c == '/' || *c == '\\'); c--);
  for(; c >= file; c--) *c = ' ';
  qRemoveSpace(file);

  if(!strcmp(mime, "application/octet-stream")) disposition = "attachment";
  else disposition = "inline";
  printf("Content-Disposition: %s;filename=\"%s\"\n", disposition, file);
  printf("Content-Transfer-Encoding: binary\n");
  printf("Accept-Ranges: bytes\n");
  printf("Content-Length: %ld\n", qFileSize(filename));
  printf("Connection: close\n");
  printf("Content-Type: %s\n", mime);
  printf("\n");
  free(file);

  qCatFile(filename);
  return 1;
}

/**********************************************
** Usage : qFileSize(filename);
** Return: Size of file in byte, File not found -1
**********************************************/
long qFileSize(char *filename) {
  struct stat finfo;

/*  if (lstat(filename, &finfo) < 0) return -1;*/

  return finfo.st_size;
}

/**********************************************
** Usage : qRedirect(url);
** Do    : Redirect page using Location response-header.
**********************************************/
void qRedirect(char *url) {
  printf("Location: %s\n\n", url);
}

/**********************************************
** Usage : qReadCounter(filename);
** Return: Success counter value, Fail 0
** Do    : Read counter value
**********************************************/
int qReadCounter(char *filename){
  FILE *fp;
  int  counter;

  if((fp = fopen(filename, "rt")) == NULL) return 0;
  fscanf(fp, "%d", &counter);
  fclose(fp);
  return counter;
}

/**********************************************
** Usage : qSaveCounter(filename, number);
** Return: Success 1, Fail 0
** Do    : Save counter value
**********************************************/
int qSaveCounter(char *filename, int number){
  FILE *fp;

  if((fp = fopen(filename, "wt")) == NULL) return 0;
  fprintf(fp, "%d\n", number);
  fclose(fp);
  return 1;
}

/**********************************************
** Usage : qUpdateCount(filename, number);
** Return: Success current value + number, Fail 0
** Do    : Update counter value
**********************************************/
int qUpdateCounter(char *filename, int number){
  FILE *fp;
  int counter = 0;

  if((fp = fopen(filename, "r+t")) != NULL) {
    fscanf(fp, "%d", &counter);
    fseek(fp, 0, SEEK_SET);
  }
  else if((fp = fopen(filename, "wt")) == NULL) return 0;
  counter += number;
  fprintf(fp, "%d\n", counter);
  fclose(fp);
  return counter;
}


/**********************************************
** Usage : qCheckEmail(E-mail Address);
** Return: If it is valid return 1. Or return 0;
** Do    : Check E-mail address
**********************************************/
int qCheckEmail(char *email){
  int i, alpa, dot, gol;

  if(email == NULL) return 0;

  for(i = alpa = dot = gol = 0; email[i] != '\0'; i++){
    switch(email[i]) {
      case '@' : {
        if(alpa == 0) return 0;
        if(gol > 0)   return 0;
        gol++;
        break;
      }
      case '.' : {
        if((i > 0)   && (email[i - 1] == '@')) return 0;
        if((gol > 0) && (email[i - 1] == '.')) return 0;
        dot++;
        break;
      }
      default  : {
        alpa++;
             if((email[i] >= '0') && (email[i] <= '9')) break;
        else if((email[i] >= 'A') && (email[i] <= 'Z')) break;
        else if((email[i] >= 'a') && (email[i] <= 'z')) break;
        else if((email[i] == '-') || (email[i] == '_')) break;
        else return 0;
      }
    }
  }

  if((alpa <= 3) || (gol == 0) || (dot == 0))return 0;

  return 1;
}

/**********************************************
** Usage : qCheckURL(internet address);
** Return: If it is valid return 1. Or return 0;
** Do    : Check URL
**********************************************/
int qCheckURL(char *url){
  if(!strncmp(url, "http://", 7)) return 1;
  else if(!strncmp(url, "ftp://", 6)) return 1;
  else if(!strncmp(url, "telnet://", 9)) return 1;
  else if(!strncmp(url, "mailto:", 7)) return 1;
  else if(!strncmp(url, "news:", 5)) return 1;
  return 0;
}

/**********************************************
** Usage : qRemoveSpace(Source string);
** Return: Pointer of str
** Do    : Remove Space before string & after string
**         Remove CR, LF
**********************************************/
char *qRemoveSpace(char *str){
  int i, j;

  if(!str)return NULL;

  for(j = 0; str[j] == ' ' || str[j] == 9 || str[j] == '\r' || str[j] == '\n'; j++);
  for(i = 0; str[j] != '\0'; i++, j++) str[i] = str[j];
  for(i--; (i >= 0) && (str[i] == ' ' || str[i] == 9 || str[i] == '\r' || str[i] == '\n'); i--);
  str[i+1] = '\0';

  return str;
}

/**********************************************
** Usage : qStr09AZaz(string);
** Return: Valid 1, Invalid 0
** Do    : Check characters of string is in 0-9, A-Z, a-z
**********************************************/
int qStr09AZaz(char *str){
  for(; *str; str++){
    if((*str >= '0') && (*str <= '9')) continue;
    else if((*str >= 'A') && (*str <= 'Z')) continue;
    else if((*str >= 'a') && (*str <= 'z')) continue;
    else return 0;
  }
  return 1;
}

/**********************************************
** Usage : qStrupr(string);
** Return: Pointer of converted string.
** Do    : Convert small character to big character.
**********************************************/
char *qStrupr(char *str) {
  char *cp;

  if(!str) return NULL;
  for(cp = str; *cp; cp++) if(*cp >= 'a' && *cp <= 'z') *cp -= 32;
  return str;
}

/**********************************************
** Usage : qStrlwr(string);
** Return: Pointer of converted string.
** Do    : Convert big character to small character.
**********************************************/
char *qStrlwr(char *str) {
  char *cp;

  if(!str) return NULL;
  for(cp = str; *cp; cp++) if(*cp >= 'A' && *cp <= 'Z') *cp += 32;
  return str;
}


/**********************************************
** Usage : qStristr(big, small);
** Return: Pointer of token string located in original string, Fail NULL
** Do    : Find token with no case-censitive
**********************************************/
char *qStristr(char *big, char *small) {
  char *bigp, *smallp, *retp;

  if(big == NULL || small == NULL) return 0;

  if((bigp = strdup(big)) == NULL) return 0;
  if((smallp = strdup(small)) == NULL) { free(bigp); return 0; }

  qStrupr(bigp), qStrupr(smallp);

  retp = strstr(bigp, smallp);
  if(retp != NULL) retp = big + (retp - bigp);
  free(bigp), free(smallp);

  return retp;
}

/**********************************************
** Usage : qStricmp(char *orgstr, char *tokstr);
** Return: s1 < s2 less than 0, s1 = s2 0, s1 > s2 greater than 0
** Do    : qDecoder implementation of stricmp() because
**         some systems do not support this function.
**********************************************/
int qStricmp(char *s1, char *s2) {
  char *bs1, *bs2;
  int result;

  if(s1 == NULL || s2 == NULL) return 0;
  if((bs1 = strdup(s1)) == NULL) return 0;
  if((bs2 = strdup(s2)) == NULL) { free(bs1); return 0; }

  qStrupr(bs1), qStrupr(bs2);
  result = strcmp(bs1, bs2);
  free(bs1), free(bs2);

  return result;
}

/**********************************************
** Usage : qStrnicmp(s1, s2, length);
** Return: Same as strncmp().
** Do    : Compare n-byte strings with no case-censitive.
**********************************************/
int qStrincmp(char *s1, char *s2, size_t len) {
  char *s1p, *s2p;
  int result;

  if(s1 == NULL || s2 == NULL) return 0;

  if((s1p = strdup(s1)) == NULL) return 0;
  if((s2p = strdup(s2)) == NULL) { free(s1p); return 0; }

  qStrupr(s1p), qStrupr(s2p);

  result = strncmp(s1p, s2p, len);
  free(s1p), free(s2p);

  return result;
}

/**********************************************
** Usage : qitocomma(value);
** Return: String pointer
** Do    : Convert integer to comma string
**         printf("Output: %s", qitocomma(1234567));
**         Output: -1,234,567,000
**********************************************/
char *qitocomma(int value) {
  static char str[14+1];
  char buf[10+1], *strp = str, *bufp;

  if(value < 0) *strp++ = '-';
  sprintf(buf, "%d", abs(value));
  for (bufp = buf; *bufp != '\0'; strp++, bufp++) {
    *strp = *bufp;
    if((strlen(bufp)) % 3 == 1 && *(bufp + 1) != '\0') *(++strp) = ',';
  }
  *strp = '\0';

  return str;
}

/**********************************************
***********************************************
** You don't need to use below functions
** It is used by qDecoder();
***********************************************
**********************************************/

/**********************************************
** Usage : _x2c(HEX Up character, HEX Low character);
** Return: Hex value which is changed
** Do    : Change two hex character to one hex value
**********************************************/
char _x2c(char hex_up, char hex_low){
  char digit;

  digit = 16 * (hex_up >= 'A' ? ((hex_up & 0xdf) - 'A') + 10 : (hex_up - '0'));
  digit += (hex_low >= 'A' ? ((hex_low & 0xdf) - 'A') + 10 : (hex_low - '0'));

  return (digit);
}


/**********************************************
** Usage : _makeword(Source string, Stop character);
** Return: Pointer of Parsed string
** Do    : It copy source string before stop character
**         The pointer of source string direct after stop character
**********************************************/
char *_makeword(char *str, char stop){
  char *word;
  int  len, i;

  for(len = 0; ((str[len] != stop) && (str[len])); len++);
  word = (char *)malloc(sizeof(char) * (len + 1));

  for(i = 0; i < len; i++)word[i] = str[i];
  word[i] = '\0';

  if(str[len])len++;
  for(i = len; str[i]; i++)str[i - len] = str[i];
  str[i - len] = '\0';

  return (word);
}

/*********************************************
** Usage : _strtok2(String, TokenStopString, ReturnStopCharacter);
** Do    : Find token string (usage like strtok())
** Return: Pointer of token & character of stop
**********************************************/
char *_strtok2(char *str, char *token, char *retstop){
  static char *tokensp, *tokenep;
  int i, j;

  if(str != NULL) tokensp = tokenep = str;
  else tokensp = tokenep;

  for(i = strlen(token);*tokenep;tokenep++){
    for(j = 0; j < i; j++){
      if(*tokenep == token[j]){
        *retstop = token[j];
        *tokenep = '\0';
        tokenep++;
        return tokensp;
      }

    }
  }

  *retstop = '\0';
  if(tokensp != tokenep) return tokensp;
  return NULL;
}

/*********************************************
** Usage : This function is perfectly same as fgets();
**********************************************/
char *_fgetstring(char *buf, int maxlen, FILE *fp) {
  int i, c;

  for(i = 0; i < (maxlen - 1); i++) {
    c = fgetc(fp);
    if(c == EOF) break;
    buf[i] = (char)c;
    if(c == '\n') {
      i++;
      break;
    }
  }
  if(i == 0) return NULL;

  buf[i] = '\0';
  return buf;
}

/**********************************************
** Linked List(Entry) Routines
**********************************************/

/**********************************************
** Usage : _EntryValue(Pointer of the first Entry, Name);
** Return: Success pointer of value string, Fail NULL
** Do    : Find value string pointer
**         It find value in linked list
**********************************************/
char *_EntryValue(Entry *first, char *name){
  Entry *entries;

  for(entries = first; entries; entries = entries->next){
    if(!strcmp(name, entries->name))return (entries->value);
  }
  return NULL;
}

/**********************************************
** Usage : _EntryiValue(Pointer of the first Entry, Name);
** Return: Success integer of value string, Fail 0
** Do    : Find value string pointer and convert to integer
**********************************************/
int _EntryiValue(Entry *first, char *name){
  char *str;

  str = _EntryValue(first, name);
  if(str == NULL) return 0;
  return atoi(str);
}

/**********************************************
** Usage : _EntryPrint(Pointer of the first Entry);
** Do    : Print all parsed value & name for debugging
**********************************************/
void _EntryPrint(Entry *first){
  Entry *entries;

  qContentType("text/html");

  for(entries = first; entries; entries = entries->next){
    printf("'%s' = '%s'<br>\n" , entries->name, entries->value);
  }
}

/**********************************************
** Usage : _EntryFree(Pointer of the first Entry);
** Do    : Make free of linked list memory
**********************************************/
void _EntryFree(Entry *first){
  Entry *entries;

  for(; first; first = entries){
    entries = first->next; /* copy next to tmp */
    free(first->name);
    free(first->value);
    free(first);
  }
  first = NULL;
}
