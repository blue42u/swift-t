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


#ifndef ADLB_X_TYPES_H
#define ADLB_X_TYPES_H

#include "adlb_types.h"

/*
   Declare a struct type.  This function must be called on all
   processes separately.
   type: the index of the struct type
   type_name: name of the type, which must not conflict with an
         existing adlb type (e.g. int), or an already-declared struct
         type.  This can be later used to create structs of that type.
   field_count: number of fields
   field_types: types of fields
   field_names: names of fields
 */
adlb_data_code
ADLBX_Declare_struct_type(adlb_struct_type type,
                    const char *type_name,
                    int field_count,
                    const adlb_struct_field_type *field_types,
                    const char **field_names);

/*
   Retrieve info about struct type. Returns type error if not found.
   Any pointer arguments can be left NULL if info not needed.
 */
adlb_data_code
ADLBX_Lookup_struct_type(adlb_struct_type type,
                  const char **type_name, int *field_count,
                  const adlb_struct_field_type **field_types,
                  char const* const** field_names);

/*
   Get a packed representation, i.e. one in which the data is in
   contiguous memory.
   This will return an internal pointer if possible.
   If this isn't possible it uses the provided caller_buffer if large enough,
   or allocates memory
 */
adlb_data_code
ADLBX_Pack(const adlb_datum_storage *d, adlb_data_type type,
          const adlb_buffer *caller_buffer,
          adlb_binary_data *result);
/*
  Append data to a buffer, prefixing with size stored as vint,
  so that contiguously stored entries can be correctly extracted.
  This will use tmp_buf as temporary storage if needed, and resize
  output if needed.
  type: this must be provided if data is to interpreted as a particular
        ADLB data type upon decoding.
 */
adlb_data_code
ADLBX_Append_buffer(adlb_data_type type, const void *data, size_t length,
        bool prefix_len, adlb_buffer *output, bool *output_caller_buffer,
        size_t *output_pos);

/*
  Pack a datum into a buffer.
  This will use tmp_buf as temporary storage if needed, and resize
  output if needed.

  prefix_len: include prefix with size stored as vint, so that
      contiguously stored datums can be correctly extracted.
  tmp_buf: temporary storage that datum may be serialized into if
           needed
  output: the output buffer to append to
  output_caller_buffer: set to false if we allocate a new buffer,
                        untouched otherwise
  output_pos: current offset into output buffer, which is updated
            for any appended data
 */
adlb_data_code
ADLBX_Pack_buffer(const adlb_datum_storage *d, adlb_data_type type,
        bool prefix_len, const adlb_buffer *tmp_buf, adlb_buffer *output,
        bool *output_caller_buffer, size_t *output_pos);

/*
  Whether we pad the vint size to VINT_MAX_BYTES when appending
  to buffer;
 */
bool ADLBX_pack_pad_size(adlb_data_type type);

/*
   Unpack data from buffer into adlb_datum_storage, allocating new
   memory if necessary.  The unpacked data won't hold any pointers
   into the buffer.  Compound data types are initialized.

   refcounts: number of refcounts to initialize any refs inside
              the unpacked data structure
 */
adlb_data_code
ADLBX_Unpack(adlb_datum_storage *d, adlb_data_type type,
            const void *buffer, size_t length, adlb_refc refcounts);

/*
  Same as ADLB_Unpack, with more options.
  Buffer cannot be const since we may take ownership of buffer.

  copy_buffer: If false, buffer must not be modified.  If true,
      function can optionally take ownership of buffer.
  init_compound: if true, compound data types (containers, structs, etc)
  that support incremental stores are assumed to be initialized and
  shouldn't be reinitialized.
  took_ownership: Set to indicate whether the function took ownership
        of the buffer.  Can be left as NULL.  Always false if
        copy_buffer is true.
 */
adlb_data_code
ADLBX_Unpack2(adlb_datum_storage *d, adlb_data_type type,
          void *buffer, size_t length, bool copy_buffer,
          adlb_refc refcounts, bool init_compound, bool *took_ownership);

/*
  Helper to unpack data from buffer.  This will simply
  find the packed data for the next item in the buffer and
  advance the read position
  buffer, length: buffer packed with ADLB_Pack_buffer
  type: data type, or ADLB_DATA_TYPE_NULL if uninterpreted
  pos: input/output.  Caller provides current read position in buffer,
        this function sets it to the start of the next entry
  entry, entry_length: output. packed field data
  returns: ADLB_DATA_SUCCESS when field successfully returned,
           ADLB_DONE when exhausted,
           ADLB_DATA_INVALID if not able to decode
 */
adlb_data_code
ADLBX_Unpack_buffer(adlb_data_type type,
        const void *buffer, size_t length, size_t *pos,
        const void **entry, size_t* entry_length);

/**
  Initialize a compound data type
  must_init: if true, fail if cannot be initialized, e.g. if we don't
             have full type info.
 */
adlb_data_code
ADLBX_Init_compound(adlb_datum_storage *d, adlb_data_type type,
          adlb_type_extra type_extra, bool must_init);

/*
  Multiset is packed with a header, then a series of entries.
  Each entry is a key and value packed with ADLB_Append_buffer.
 */
adlb_data_code
ADLBX_Pack_container(const adlb_container *container,
          const adlb_buffer *tmp_buf, adlb_buffer *output,
          bool *output_caller_buffer, size_t *output_pos);

adlb_data_code
ADLBX_Pack_container_hdr(int elems, adlb_data_type key_type,
        adlb_data_type val_type, adlb_buffer *output,
        bool *output_caller_buffer, size_t *output_pos);

/*
 init_cont: if true, initialize new container
            if not, insert into existing container
 */
adlb_data_code
ADLBX_Unpack_container(adlb_container *container,
    const void *data, size_t length, adlb_refc refcounts,
    bool init_cont);

adlb_data_code
ADLBX_Unpack_container_hdr(const void *data, size_t length, size_t *pos,
        int *entries, adlb_data_type *key_type, adlb_data_type *val_type);

adlb_data_code
ADLBX_Unpack_container_entry(adlb_data_type key_type,
          adlb_data_type val_type, const void *data, size_t length,
          size_t *pos, const void **key, size_t *key_len,
          const void **val, size_t *val_len);

/*
  Multiset is packed with a header, then a series of
  entries.  Each entry is packed with ADLB_Append_buffer.
 */
adlb_data_code
ADLBX_Pack_multiset(const adlb_multiset_ptr ms,
          const adlb_buffer *tmp_buf, adlb_buffer *output,
          bool *output_caller_buffer, size_t *output_pos);

adlb_data_code
ADLBX_Pack_multiset_hdr(int elems, adlb_data_type elem_type,
    adlb_buffer *output, bool *output_caller_buffer, size_t *output_pos);

adlb_data_code
ADLBX_Unpack_multiset(adlb_multiset_ptr *ms, const void *data,
        size_t length, adlb_refc refcounts, bool init_ms);

adlb_data_code
ADLBX_Unpack_multiset_hdr(const void *data, size_t length, size_t *pos,
                int *entries, adlb_data_type *elem_type);

adlb_data_code
ADLBX_Unpack_multiset_entry(adlb_data_type elem_type,
          const void *data, size_t length, size_t *pos,
          const void **elem, size_t *elem_len);

adlb_data_code
ADLBX_Pack_struct(const adlb_struct *s, const adlb_buffer *caller_buffer,
                 adlb_binary_data *result);

adlb_data_code
ADLBX_Unpack_struct(adlb_struct **s, const void *data, size_t length,
                   adlb_refc refcounts, bool init_struct);

// Free any memory used
adlb_data_code
ADLBX_Free_storage(adlb_datum_storage *d, adlb_data_type type);

/**
 * Parse signed 64-bit integer from fixed-length string
 */
adlb_data_code
ADLBX_Int64_parse(const char *str, size_t length, int64_t *result);

/*
   Create string with human-readable representation of datum.
   Caller must free string.
   Note: this is currently not fast or robust, only used for
   printing debug information
  */
char *
ADLBX_Data_repr(const adlb_datum_storage *d, adlb_data_type type);

#endif

