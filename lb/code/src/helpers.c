#include "adlb.h"
#include <string.h>
#include <ctype.h>

adlb_code ADLB_string_to_placement(const char *string,
                           adlb_placement *placement)
{
  const size_t MAX_PLACEMENT_LEN = 64;

  char buf[MAX_PLACEMENT_LEN + 1];
  strncpy(buf, string, MAX_PLACEMENT_LEN);
  buf[MAX_PLACEMENT_LEN] = '\0';

  for (char *p = buf; *p != '\0'; p++)
  {
    *p = (char)tolower(*p);
  }

  if (strcmp(buf, "random") == 0)
  {
    *placement = ADLB_PLACE_RANDOM;
  }
  else if (strcmp(buf, "local") == 0)
  {
    *placement = ADLB_PLACE_LOCAL;
  }
  else if (strcmp(buf, "default") == 0)
  {
    *placement = ADLB_PLACE_DEFAULT;
  }
  else
  {
    return ADLB_ERROR;
  }
  return ADLB_SUCCESS;
}

/**
   Convert given data type number to output string representation
 */
const char
*ADLB_Data_type_tostring(adlb_data_type type)
{
  switch(type)
  {
    case ADLB_DATA_TYPE_INTEGER:
      return "integer";
    case ADLB_DATA_TYPE_FLOAT:
      return "float";
    case ADLB_DATA_TYPE_STRING:
      return "string";
    case ADLB_DATA_TYPE_BLOB:
      return "blob";
    case ADLB_DATA_TYPE_CONTAINER:
      return "container";
    case ADLB_DATA_TYPE_MULTISET:
      return "multiset";
    case ADLB_DATA_TYPE_REF:
      return "ref";
    case ADLB_DATA_TYPE_STRUCT:
      return "struct";
    case ADLB_DATA_TYPE_NULL:
      return "ADLB_DATA_TYPE_NULL";
    default:
      return "<invalid type>";
  }
}

adlb_code ADLB_Data_string_totype(const char *type_string, adlb_data_type* type,
                           adlb_type_extra* extra)
{
  if (strcmp(type_string, "integer") == 0) *type = ADLB_DATA_TYPE_INTEGER;
  else if (strcmp(type_string, "float") == 0) *type = ADLB_DATA_TYPE_FLOAT;
  else if (strcmp(type_string, "string") == 0) *type = ADLB_DATA_TYPE_STRING;
  else if (strcmp(type_string, "blob") == 0) *type = ADLB_DATA_TYPE_BLOB;
  else if (strcmp(type_string, "container") == 0) *type = ADLB_DATA_TYPE_CONTAINER;
  else if (strcmp(type_string, "multiset") == 0) *type = ADLB_DATA_TYPE_MULTISET;
  else if (strcmp(type_string, "ref") == 0) *type = ADLB_DATA_TYPE_REF;
  else if (strcmp(type_string, "struct") == 0) *type = ADLB_DATA_TYPE_STRUCT;
  else if (strcmp(type_string, "ADLB_DATA_TYPE_NULL") == 0) *type = ADLB_DATA_TYPE_NULL;
  else return ADLB_ERROR;
  *extra = ADLB_TYPE_EXTRA_NULL;
  return ADLB_SUCCESS;
}
