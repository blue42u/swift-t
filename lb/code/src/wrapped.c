#include <stdio.h>
#include "adlb.h"
#include <stdlib.h>

#define BACKEND_xtask

#if defined BACKEND_adlb
#include "adlb-x.h"
static MPI_Comm comm;
static MPI_Comm work_comm;
#elif defined BACKEND_xtask
#include "xtask_api.h"
static int id, servers, workers;
#endif

void ADLB_Init_comm() {
#if defined BACKEND_adlb
	int argc = 0;
	char** argv = NULL;
	MPI_Init(&argc, &argv);
	//assert(rc == MPI_SUCCESS);	// Just hope it works...
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
#elif defined BACKEND_xtask
	workers = 2;
	id = xtask_setup(64, workers);	// Hardcoded for now
#endif
}

adlb_code ADLB_Init(int nservers, int ntypes, int type_vect[], int *am_server) {
#if defined BACKEND_adlb
	return ADLBX_Init(nservers, ntypes, type_vect, am_server,
		comm, &work_comm);
#elif defined BACKEND_xtask
	// Servers are workers with a low enough id.
	servers = nservers;
	if(id < nservers) *am_server = 1;
	else *am_server = 0;
	return ADLB_SUCCESS;
#endif
}

adlb_code ADLB_Server(long max_memory) {
#if defined BACKEND_adlb
	return ADLBX_Server(max_memory);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Version(version* output) {
#if defined BACKEND_adlb
	return ADLBX_Version(output);
#elif defined BACKEND_xtask
	output->major = 0;
	output->minor = 9;
	output->revision = 0;
	return ADLB_SUCCESS;
#endif
}

int ADLB_Get_rank() {
#if defined BACKEND_adlb
	int out;
	MPI_Comm_rank(comm, &out);
	return out;
#elif defined BACKEND_xtask
	return id;
#endif
}

int ADLB_Get_size() {
#if defined BACKEND_adlb
	int out;
	MPI_Comm_size(comm, &out);
	return out;
#elif defined BACKEND_xtask
	return workers;
#endif
}

int ADLB_GetRank_workers() {
#if defined BACKEND_adlb
	int out;
	MPI_Comm_rank(work_comm, &out);
	return out;
#elif defined BACKEND_xtask
	return id - servers;
#endif
}

int ADLB_Is_leader() {
#if defined BACKEND_adlb
	return ADLBX_GetComm_leaders() != MPI_COMM_NULL;
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Put(const void* payload, int length, adlb_put_opts opts) {
#if defined BACKEND_adlb
	return ADLBX_Put(payload, length, ADLB_RANK_ANY, ADLB_RANK_NULL, 0, opts);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Dput(const void* payload, int length, adlb_put_opts opts,
	const char *name, const adlb_datum_id *wait_ids, int wait_id_count,
        const adlb_datum_id_sub *wait_id_subs, int wait_id_sub_count) {
#if defined BACKEND_adlb
	return ADLBX_Dput(payload, length, ADLB_RANK_ANY, ADLB_RANK_NULL,
		0, opts, name, wait_ids, wait_id_count, wait_id_subs,
		wait_id_sub_count);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Get(void** payload, int* length, int max_length) {
#if defined BACKEND_adlb
	MPI_Comm tmp;
	int a,t;
	return ADLBX_Get(0, payload, length, max_length,
		&a, &t, &tmp);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Iget(void* payload, int* length) {
#if defined BACKEND_adlb
	MPI_Comm tmp;
	int a,t;
	return ADLBX_Iget(0, payload, length, &a,
		&t, &tmp);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

int ADLB_Locate(adlb_datum_id id) {
#if defined BACKEND_adlb
	return ADLBX_Locate(id);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Create(adlb_datum_id id, adlb_data_type type,
                      adlb_type_extra type_extra,
                      adlb_create_props props, adlb_datum_id *new_id) {
#if defined BACKEND_adlb
	return ADLBX_Create(id, type, type_extra, props, new_id);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Multicreate(ADLB_create_spec *specs, int count) {
#if defined BACKEND_adlb
	return ADLBX_Multicreate(specs, count);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}


adlb_code ADLB_Create_integer(adlb_datum_id id, adlb_create_props props,
                              adlb_datum_id *new_id) {
#if defined BACKEND_adlb
	return ADLBX_Create_integer(id, props, new_id);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Create_float(adlb_datum_id id, adlb_create_props props,
                              adlb_datum_id *new_id) {
#if defined BACKEND_adlb
	return ADLBX_Create_float(id, props, new_id);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Create_string(adlb_datum_id id, adlb_create_props props,
                              adlb_datum_id *new_id) {
#if defined BACKEND_adlb
	return ADLBX_Create_string(id, props, new_id);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Create_blob(adlb_datum_id id, adlb_create_props props,
                              adlb_datum_id *new_id) {
#if defined BACKEND_adlb
	return ADLBX_Create_blob(id, props, new_id);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Create_ref(adlb_datum_id id, adlb_create_props props,
                              adlb_datum_id *new_id) {
#if defined BACKEND_adlb
	return ADLBX_Create_ref(id, props, new_id);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Create_struct(adlb_datum_id id, adlb_create_props props,
                             adlb_struct_type struct_type, adlb_datum_id *new_id) {
#if defined BACKEND_adlb
	return ADLBX_Create_struct(id, props, struct_type, new_id);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Create_container(adlb_datum_id id,
                                adlb_data_type key_type,
                                adlb_data_type val_type,
                                adlb_create_props props,
                                adlb_datum_id *new_id) {
#if defined BACKEND_adlb
	return ADLBX_Create_container(id, key_type, val_type, props, new_id);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Create_multiset(adlb_datum_id id,
                                adlb_data_type val_type,
                                adlb_create_props props,
                                adlb_datum_id *new_id) {
#if defined BACKEND_adlb
	return ADLBX_Create_multiset(id, val_type, props, new_id);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Add_dsym(adlb_dsym symbol, adlb_dsym_data data) {
#if defined BACKEND_adlb
	return ADLBX_Add_dsym(symbol, data);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_dsym_data ADLB_Dsym(adlb_dsym symbol) {
#if defined BACKEND_adlb
	return ADLBX_Dsym(symbol);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Exists(adlb_datum_id id, adlb_subscript subscript, bool* result,
                       adlb_refc decr) {
#if defined BACKEND_adlb
	return ADLBX_Exists(id, subscript, result, decr);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Refcount_get(adlb_datum_id id, adlb_refc *result,
                              adlb_refc decr) {
#if defined BACKEND_adlb
	return ADLBX_Refcount_get(id, result, decr);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Store(adlb_datum_id id, adlb_subscript subscript,
          adlb_data_type type, const void *data, size_t length,
          adlb_refc refcount_decr, adlb_refc store_refcounts) {
#if defined BACKEND_adlb
	return ADLBX_Store(id, subscript, type, data, length, refcount_decr,
		store_refcounts);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Retrieve(adlb_datum_id id, adlb_subscript subscript,
      adlb_retrieve_refc refcounts, adlb_data_type* type,
      void* data, size_t* length) {
#if defined BACKEND_adlb
	return ADLBX_Retrieve(id, subscript, refcounts, type, data, length);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Enumerate(adlb_datum_id container_id,
                   int count, int offset, adlb_refc decr,
                   bool include_keys, bool include_vals,
                   void** data, size_t* length, int* records,
                   adlb_type_extra *kv_type) {
#if defined BACKEND_adlb
	return ADLBX_Enumerate(container_id, count, offset, decr, include_keys,
		include_vals, data, length, records, kv_type);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Read_refcount_enable(void) {
#if defined BACKEND_adlb
	return ADLBX_Read_refcount_enable();
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Refcount_incr(adlb_datum_id id, adlb_refc change) {
#if defined BACKEND_adlb
	return ADLBX_Refcount_incr(id, change);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Insert_atomic(adlb_datum_id id, adlb_subscript subscript,
                        adlb_retrieve_refc refcounts,
                        bool *result, bool *value_present,
                        void *data, size_t *length, adlb_data_type *type) {
#if defined BACKEND_adlb
	return ADLBX_Insert_atomic(id, subscript, refcounts, result,
		value_present, data, length, type);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Subscribe(adlb_datum_id id, adlb_subscript subscript,
                          int work_type, int* subscribed) {
#if defined BACKEND_adlb
	return ADLBX_Subscribe(id, subscript, work_type, subscribed);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Container_reference(adlb_datum_id id, adlb_subscript subscript,
                adlb_datum_id ref_id, adlb_subscript ref_subscript,
                adlb_data_type ref_type, adlb_refc transfer_refs,
                int ref_write_decr) {
#if defined BACKEND_adlb
	return ADLBX_Container_reference(id, subscript, ref_id, ref_subscript,
		ref_type, transfer_refs, ref_write_decr);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Unique(adlb_datum_id *result) {
#if defined BACKEND_adlb
	return ADLBX_Unique(result);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Alloc_global(int count, adlb_datum_id *start) {
#if defined BACKEND_adlb
	return ADLBX_Alloc_global(count, start);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Typeof(adlb_datum_id id, adlb_data_type* type) {
#if defined BACKEND_adlb
	return ADLBX_Typeof(id, type);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Container_typeof(adlb_datum_id id, adlb_data_type* key_type,
                                 adlb_data_type* val_type) {
#if defined BACKEND_adlb
	return ADLBX_Container_typeof(id, key_type, val_type);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Container_size(adlb_datum_id container_id, int* size,
                              adlb_refc decr) {
#if defined BACKEND_adlb
	return ADLBX_Container_size(container_id, size, decr);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Lock(adlb_datum_id id, bool* result) {
#if defined BACKEND_adlb
	return ADLBX_Lock(id, result);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Unlock(adlb_datum_id id) {
#if defined BACKEND_adlb
	return ADLBX_Unlock(id);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Finalize(void) {
#if defined BACKEND_adlb
	adlb_code o = ADLBX_Finalize();
	MPI_Comm_free(&comm);
	MPI_Finalize();
	return o;
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

adlb_code ADLB_Fail(int code) {
#if defined BACKEND_adlb
	return ADLBX_Fail(code);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}

void ADLB_Abort(int code) {
#if defined BACKEND_adlb
	ADLBX_Abort(code);
#elif defined BACKEND_xtask
	printf("STUB! Crashing now...\n");
	exit(5);
#endif
}
