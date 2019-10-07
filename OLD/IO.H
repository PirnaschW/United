typedef struct {
  char * description;
  void * ptr;
} ENTITY;

#define  p(x)  (void *) &(ptr->x)

ENTITY list [] = {
  {  "Heimvorteil : %d",
   p(heimvorteil) },
  {  "Heimvorteil : %d", (void *) &(ptr->heimvorteil) },
  {  "Heimvorteil : %d", &(ptr->heimvorteil) },
  {  "Heimvorteil : %d", ptr->heimvorteil },
  {  "Heimvorteil : %d", p(heimvorteil) } };
