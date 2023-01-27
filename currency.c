#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <json-c/json.h>

char* toLowerCase(const char s[]) {
  char *output = s + 1;
  output[strlen(output) - 1] = 0;
  int length = strlen(output);
  for(int i = 0; i < length; i++) {
    output[i] = tolower(output[i]);
  }
  return output;
}

float get_rate(char *currency) {
  struct json_object* obj;
  obj = json_tokener_parse(currency);
  return atof(json_object_to_json_string(json_object_object_get(obj, "rate")));
}

char *get_date(char *currency) {
  struct json_object* obj;
  obj = json_tokener_parse(currency);
  return toLowerCase(json_object_to_json_string(json_object_object_get(obj, "exchangedate")));
}

char *get_currency(char *json_currency, char *cc) {
  char *output;
  struct json_object* obj;
  obj = json_tokener_parse(json_currency);

  for(int i = 0; i < json_object_array_length(obj); i++) {
    json_object *iter = json_object_array_get_idx(obj, i);
    if(strcmp(toLowerCase(json_object_to_json_string(json_object_object_get(iter, "cc"))), cc) == 0) {
      strcpy(output, json_object_to_json_string(iter));
      break;
    }
  }
  return output;
}

float calculate_currency(char *currency, char *sum) {
  struct json_object* obj;
  obj = json_tokener_parse(currency);
  return atof(json_object_to_json_string(json_object_object_get(obj, "rate"))) * atof(sum);
}

void print_all_get_currencies(char *json_currency) {
  struct json_object* obj;
  obj = json_tokener_parse(json_currency);

  for(int i = 0; i < json_object_array_length(obj); i++) {
    json_object *iter = json_object_array_get_idx(obj, i);
    printf("%s\t%s\n", json_object_to_json_string(json_object_object_get(iter, "cc")), json_object_to_json_string(json_object_object_get(iter, "txt")));
  }
}