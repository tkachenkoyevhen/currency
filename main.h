struct Memory {
  char *response;
  size_t size;
};

char* toLowerCase(const char s[]);
float get_rate(char *currency);
char *get_date(char *currency);
char *get_currency(char *json_currency, char *cc);
float calculate_currency(char *currency, char *sum);
void print_all_get_currencies(char *json_currency);