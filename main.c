#include <string.h>
#include <json-c/json.h>

#include <curl/curl.h>

#include "main.h"

size_t cb(void *data, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct Memory *mem = (struct Memory *)userp;
 
  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if(ptr == NULL)
    return 0;  /* out of memory! */
 
  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;
 
  return realsize;
}

int main(int argc, char *argv[]) {
  CURL *curl = curl_easy_init();
  CURLcode res;
  
  struct Memory chunk;
  chunk.response = malloc(1);
  chunk.size = 0;

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?json");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);

    if(CURLE_OK == res) {
      char *json_currency = chunk.response;
      if(argc == 1) {
        print_all_get_currencies(json_currency);
        return 0;
      }
      char *item = get_currency(json_currency, argv[1]);
      printf("Information for the day: %s\nRate: %.2f UAH\n", get_date(item), get_rate(item));
      if(argc == 3) {
        printf("Result calculation: %.2f\n", calculate_currency(item, argv[2]));
      }
    }

    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }

  return 0;
}