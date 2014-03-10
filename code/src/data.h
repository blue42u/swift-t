/*
 * Copyright 2013 University of Chicago and Argonne National Laboratory
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 */


/**
 * ADLB Data module
 *
 * All functions return ADLB_DATA_SUCCESS or ADLB_DATA_ERROR_*
 * */

#ifndef DATA_H
#define DATA_H

#include "adlb-defs.h"
#include "adlb_types.h"
#include "notifications.h"

adlb_data_code xlb_data_init(int s, int r);

adlb_data_code xlb_data_declare_struct_type(adlb_struct_type type,
                    const char *type_name,
                    int field_count,
                    const adlb_data_type *field_types,
                    const char **field_names);

adlb_data_code xlb_data_typeof(adlb_datum_id id, adlb_data_type* key_type);


adlb_data_code xlb_data_container_typeof(adlb_datum_id id,
                                     adlb_data_type* key_type,
                                     adlb_data_type* val_type);

/**
  Create a new datum
  type_extra: extra type info, or null if not provided
 */
adlb_data_code xlb_data_create(adlb_datum_id id, adlb_data_type type,
                           const adlb_type_extra *type_extra,
                           const adlb_create_props *props);

adlb_data_code xlb_data_exists(adlb_datum_id id, adlb_subscript subscript,
                           bool* result);

adlb_data_code xlb_data_lock(adlb_datum_id id, int rank, bool* result);

adlb_data_code xlb_data_unlock(adlb_datum_id id);

adlb_data_code xlb_data_subscribe(adlb_datum_id id, adlb_subscript subscript,
                              int rank, int* result);

/**
 * If data at id[subscript] is already set:
 *  -> return data in same way as xlb_data_retrieve
 *  -> fill in notifications with notification work
 *  -> acquire reference counts for data now
 * If data is not set:
 *  -> don't return data
 *  -> don't fill in notifications
 *  -> do add entry so that notifications and refcount acquisition
 *     happens later.
 * In both cases:
 *  -> consume single read reference count
 */
adlb_data_code
xlb_data_container_reference(adlb_datum_id id, adlb_subscript subscript,
         adlb_datum_id ref_id, adlb_subscript ref_sub,
         adlb_data_type ref_type, adlb_refcounts to_acquire, 
         const adlb_buffer *caller_buffer, adlb_binary_data *result,
         adlb_notif_t *notifications);


adlb_data_code xlb_data_container_size(adlb_datum_id container_id,
                                   int* size);

/**
 Retrieve data for id/subscript pair.
 See xlb_data_retrieve2 for information about arguments
 */
adlb_data_code
xlb_data_retrieve(adlb_datum_id id, adlb_subscript subscript,
                  adlb_data_type* type, const adlb_buffer *caller_buffer,
                  adlb_binary_data *result); 

/**
 Retrieve data for id/subscript pair, plus support acquiring references
 for caller and decrementing references of datum
 id: id of top-level datum
 subscript: subscript to retrieve, ADLB_NO_SUB to retrieve top-level datum
 decr: decrement references of this id
 to_acquire: acquire this many references for any data referenced by
      the retrieved data.
 type: output arg for type of data retrieved
 caller_buffer: optional buffer to provide space for result
 result: output buffer with serialized data, must be freed by
         caller if contains allocated memory
 notifications: notifications to process.  Only will be filled if
        manipulating refcounts through decr or to_acquire.
        Can be NULL if decr == to_acquire == ADLB_NO_RC.
        If not NULL, must be initialized by caller.
 returns ADLB_DATA_ERROR_NOT_FOUND if id not found
            ADLB_DATA_ERROR_SUBSCRIPT_NOT_FOUND if id found, but not subscript
 */
adlb_data_code
xlb_data_retrieve2(adlb_datum_id id, adlb_subscript subscript,
                 adlb_refcounts decr, adlb_refcounts to_acquire,
                 adlb_data_type* type, const adlb_buffer *caller_buffer,
                 adlb_binary_data *result, adlb_notif_t *notifications);

adlb_data_code
xlb_data_enumerate(adlb_datum_id id, int count, int offset,
               bool include_keys, bool include_vals,
               const adlb_buffer *caller_buffer,
               adlb_buffer *data, int* actual,
               adlb_data_type *key_type, adlb_data_type *val_type);

adlb_data_code xlb_data_store(adlb_datum_id id, adlb_subscript subscript,
          const void* buffer, int length, adlb_data_type type,
          adlb_refcounts refcount_decr,
          adlb_notif_t *notifications);


/*
   Struct used to specify if refcounts of referands should be reused
   when a structure is freed
 */
typedef struct {
  // Optional: if non-null, only acquire refcount for this subscript
  adlb_subscript subscript; 
  // how many refcounts to try to acquire on referands
  adlb_refcounts refcounts;
} xlb_acquire_rc;

static const xlb_acquire_rc XLB_NO_ACQUIRE = 
      { .subscript.key = NULL, .subscript.length = 0,
        .refcounts.read_refcount = 0, .refcounts.write_refcount = 0 };

/*
  Modify data reference count

  acquire: acquire this number of references to all ids referenced
           by datum
  garbage_collected: can be NULL if you don't need info
 */
adlb_data_code xlb_data_reference_count(adlb_datum_id id,
                adlb_refcounts change,
                xlb_acquire_rc acquire, bool *garbage_collected,
                adlb_notif_t *notifications);

const char*
xlb_data_rc_type_tostring(adlb_refcount_type rc_type);

adlb_data_code xlb_data_insert_atomic(adlb_datum_id container_id,
                                  adlb_subscript subscript,
                                  bool* created, bool *value_present);

adlb_data_code xlb_data_unique(adlb_datum_id* result);

adlb_data_code xlb_data_finalize(void);

#endif
