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


#ifndef ADLB_P_H
#define ADLB_P_H

#include "adlb.h"

/*
   These are the functions available to ADLB application code
 */

adlb_code ADLBP_Init(int nservers, int ntypes, int type_vect[],
                     int *am_server, MPI_Comm adlb_comm,
                     MPI_Comm *worker_comm);
adlb_code ADLBP_Put(const void* payload, int length, int target, int answer,
                    int type, adlb_put_opts opts);
adlb_code ADLBP_Dput(const void* payload, int length, int target,
        int answer, int type, adlb_put_opts opts, const char *name,
        const adlb_datum_id *wait_ids, int wait_id_count, 
        const adlb_datum_id_sub *wait_id_subs, int wait_id_sub_count);
adlb_code ADLBP_Get(int type_requested, void** payload,
                    int* length, int max_length,
                    int* answer, int* type_recvd, MPI_Comm* comm);
adlb_code ADLBP_Iget(int type_requested, void* payload, int* length,
                     int* answer, int* type_recvd, MPI_Comm* comm);
adlb_code ADLBP_Aget(int type_requested, adlb_payload_buf payload,
                     adlb_get_req *req);
adlb_code ADLBP_Amget(int type_requested, int nreqs, bool wait,
                     const adlb_payload_buf* payloads,
                     adlb_get_req *reqs);
adlb_code ADLBP_Aget_test(adlb_get_req *req, int* length,
                    int* answer, int* type_recvd, MPI_Comm* comm);
adlb_code ADLBP_Aget_wait(adlb_get_req *req, int* length,
                    int* answer, int* type_recvd, MPI_Comm* comm);
adlb_code ADLBP_Create(adlb_datum_id id, adlb_data_type type,
                       adlb_type_extra type_extra,
                       adlb_create_props props,
                       adlb_datum_id *new_id);
adlb_code ADLBP_Multicreate(ADLB_create_spec *specs, int count);
adlb_code ADLBP_Add_dsym(adlb_dsym symbol, adlb_dsym_data data);
adlb_dsym_data ADLBP_Dsym(adlb_dsym symbol);
adlb_code ADLBP_Exists(adlb_datum_id id, adlb_subscript subscript, bool* result,
                       adlb_refc decr);
adlb_code ADLBP_Refcount_get(adlb_datum_id id, adlb_refc *result,
                              adlb_refc decr);
adlb_code ADLBP_Store(adlb_datum_id id, adlb_subscript subscript,
          adlb_data_type type, const void *data, size_t length,
          adlb_refc refcount_decr, adlb_refc store_refcounts);
adlb_code ADLBP_Retrieve(adlb_datum_id id, adlb_subscript subscript,
      adlb_retrieve_refc refcounts,
      adlb_data_type* type, void* data, size_t* length);
adlb_code ADLBP_Enumerate(adlb_datum_id container_id,
                   int count, int offset, adlb_refc decr,
                   bool include_keys, bool include_vals,
                   void** data, size_t* length, int* records,
                   adlb_type_extra *kv_type);
adlb_code ADLBP_Read_refcount_enable(void);
adlb_code ADLBP_Refcount_incr(adlb_datum_id id, adlb_refc change);
adlb_code ADLBP_Insert_atomic(adlb_datum_id id, adlb_subscript subscript,
                        adlb_retrieve_refc refcounts,
                        bool *result, bool *value_present,
                        void *data, size_t *length, adlb_data_type *type);
adlb_code ADLBP_Subscribe(adlb_datum_id id, adlb_subscript subscript,
                          int work_type, int* subscribed);
adlb_code ADLBP_Container_reference(adlb_datum_id id, adlb_subscript subscript,
                adlb_datum_id ref_id, adlb_subscript ref_subscript,
                adlb_data_type ref_type, adlb_refc transfer_refs,
                int ref_write_decr);
adlb_code ADLBP_Unique(adlb_datum_id *result);
adlb_code ADLBP_Alloc_global(int count, adlb_datum_id *start);
adlb_code ADLBP_Typeof(adlb_datum_id id, adlb_data_type* type);
adlb_code ADLBP_Container_typeof(adlb_datum_id id, adlb_data_type* key_type,
                                 adlb_data_type* val_type);
adlb_code ADLBP_Container_size(adlb_datum_id container_id, int* size,
                               adlb_refc decr);
adlb_code ADLBP_Lock(adlb_datum_id id, bool* result);
adlb_code ADLBP_Unlock(adlb_datum_id id);
adlb_code ADLBP_Finalize(void);

#endif

