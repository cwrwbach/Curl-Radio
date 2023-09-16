#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>

//Info:
//https://curl.se/libcurl/c/curl_easy_setopt.html
//https://thecodeartist.blogspot.com/2013/02/shoutcast-internet-radio-protocol.html
// The Curl API: https://curl.se/libcurl/c/


//Some URLS:
//curl_easy_setopt(curl_handle, CURLOPT_URL, "http://media-ice.musicradio.com:80/ClassicFMMP3");
//curl_easy_setopt(curl_handle, CURLOPT_URL, "http://media-the.musicradio.com/ClassicFM-M-Relax");
//curl_easy_setopt(curl_handle, CURLOPT_URL, "http://s1.voscast.com:11392/stream");
//curl_easy_setopt(curl_handle, CURLOPT_URL, "http://cast1.torontocast.com:1950/stream");

//"http://icecast.thisisdax.com/SmoothUKMP3"
 
FILE *bodyfile;
FILE *headerfile;

unsigned int debug;


static size_t data_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
char * test =  strstr(ptr,"Stream");
char  * t2 = ptr;
unsigned char show;
if(test > 0)
    {
    printf(" HIT %d: %d \n",debug,test);
    printf(" %s \n",test);
    }

show = t2[debug++];

//if(debug < 256)
 //   printf("%2.2d: %2.2x %c \n",debug,show,show);

//if(debug %40 == 0) printf("\n");
size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
return written;
}

static size_t xxxheader_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
char * test =  strstr(ptr,"Stream");

test = stream;

//if(test > 0)
    {
    printf(" HEAD %d: %d \n",debug,test);
    printf(" %s \n",test);
    }
return 10; //written;
}


static size_t header_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
size_t written = fwrite(ptr, size, nmemb, stream);
printf(" writ %d %s \n",written,ptr);
return written;
}


//size_t written = fwrite(ptr, size, nmemb, (FILE * )stream);
//written = fwrite(headerfile,1,100,stream);



//fwrite(headerfile,1,100,(FILE *) stream);



//---
 
int main(void)
{
CURL *curl_handle;
char * curl_ver;
static const char *bodyfilename = "body-ok.mp3";
static const char *headerfilename = "header-ok.mp3";

debug = 0;
curl_ver = curl_version(); 
printf(" CURL based ICECAST client.\nVESION: %s \n\n",curl_ver);

curl_global_init(CURL_GLOBAL_ALL);
curl_handle = curl_easy_init();
 
struct curl_slist *headers=NULL;

//Set the URL  
curl_easy_setopt(curl_handle, CURLOPT_URL,"http://icecast.thisisdax.com/SmoothUKMP3");
//curl_easy_setopt(curl_handle, CURLOPT_URL,"http://media-ice.musicradio.com:80/ClassicFMMP3");
//curl_easy_setopt(curl_handle, CURLOPT_URL,"http://media-the.musicradio.com/ClassicFM-M-Relax");

//Disable progress meter? 
// curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
 
//send all data to this function 
curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, data_callback);

//curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, header_callback);

//open the body file 
bodyfile = fopen(bodyfilename, "wb");
if(!bodyfile) 
    {
    curl_easy_cleanup(curl_handle);
    fclose(bodyfile);
    return -1;
    }


//open the header file 
headerfile = fopen(headerfilename, "wb");
if(!bodyfile) 
    {
    curl_easy_cleanup(curl_handle);
    fclose(headerfile);
    return -1;
    }


headers = curl_slist_append(headers, "Icy-MetaData:1"); //Definitely needed
curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers); //Needed

  /* we want the headers be written to this file handle */
//curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, headerfile); //not needed

//curl_easy_setopt(curl_handle,CURLOPT_HEADER,1);

//curl_easy_setopt(curl_handle,CURLOPT_VERBOSE,1);

//we want the body be written to this file handle instead of stdout */
curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, bodyfile);

//we want the headers be written to this file handle instead of stdout */
//curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, headerfile);


//Execute Curl - and loop for ever
curl_easy_perform(curl_handle); //does not return

fclose(bodyfile);
curl_easy_cleanup(curl_handle);
 
return 0;
}
