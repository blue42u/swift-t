#include <stdio.h>
#include "adlb.h"
#include <stdlib.h>
#include <unistd.h>

#include "adlb-x.h"
static MPI_Comm comm;
static MPI_Comm work_comm;

static pid_t pid;
#define print(fmt, ...) printf("[%d] " fmt, pid, ## __VA_ARGS__)

void ADLB_Init_comm() {
	int argc = 0;
	char** argv = NULL;
	MPI_Init(&argc, &argv);
	//assert(rc == MPI_SUCCESS);	// Just hope it works...
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	pid = getpid();
}

adlb_code ADLB_Init(int nservers, int ntypes, int type_vect[], int *am_server) {
	return ADLBX_Init(nservers, ntypes, type_vect, am_server,
		comm, &work_comm);
}

adlb_code ADLB_Server(long max_memory) {
	return ADLBX_Server(max_memory);
}

adlb_code ADLB_Version(version* output) {
	return ADLBX_Version(output);
}

int ADLB_Get_rank() {
	int out;
	MPI_Comm_rank(comm, &out);
	return out;
}

int ADLB_Get_size() {
	int out;
	MPI_Comm_size(comm, &out);
	return out;
}

int ADLB_GetRank_workers() {
	int out;
	MPI_Comm_rank(work_comm, &out);
	return out;
}

int ADLB_Is_leader() {
	return ADLBX_GetComm_leaders() != MPI_COMM_NULL;
}

adlb_code ADLB_Put(const void* payload, int length, adlb_put_opts opts) {
	return ADLBX_Put(payload, length, ADLB_RANK_ANY, ADLB_RANK_NULL, 0, opts);
}

adlb_code ADLB_Dput(const void* payload, int length, adlb_put_opts opts,
	const char *name, const adlb_datum_id *wait_ids, int wait_id_count,
	const adlb_datum_id_sub *wait_id_subs, int wait_id_sub_count) {
	return ADLBX_Dput(payload, length, ADLB_RANK_ANY, ADLB_RANK_NULL,
		0, opts, name, wait_ids, wait_id_count, wait_id_subs,
		wait_id_sub_count);
}

adlb_code ADLB_Get(void** payload, int* length, int max_length) {
	MPI_Comm tmp;
	int a,t;
	return ADLBX_Get(0, payload, length, max_length,
		&a, &t, &tmp);
}

adlb_code ADLB_Iget(void* payload, int* length) {
	MPI_Comm tmp;
	int a,t;
	return ADLBX_Iget(0, payload, length, &a,
		&t, &tmp);
}

int ADLB_Locate(adlb_datum_id id) {
	return ADLBX_Locate(id);
}

adlb_code ADLB_Create(adlb_datum_id id, adlb_data_type type,
	adlb_type_extra type_extra,
	adlb_create_props props, adlb_datum_id *new_id) {
	return ADLBX_Create(id, type, type_extra, props, new_id);
}

adlb_code ADLB_Multicreate(ADLB_create_spec *specs, int count) {
	return ADLBX_Multicreate(specs, count);
}


adlb_code ADLB_Create_integer(adlb_datum_id id, adlb_create_props props,
	adlb_datum_id *new_id) {
	return ADLBX_Create_integer(id, props, new_id);
}

adlb_code ADLB_Create_float(adlb_datum_id id, adlb_create_props props,
	adlb_datum_id *new_id) {
	return ADLBX_Create_float(id, props, new_id);
}

adlb_code ADLB_Create_string(adlb_datum_id id, adlb_create_props props,
	adlb_datum_id *new_id) {
	return ADLBX_Create_string(id, props, new_id);
}

adlb_code ADLB_Create_blob(adlb_datum_id id, adlb_create_props props,
	adlb_datum_id *new_id) {
	return ADLBX_Create_blob(id, props, new_id);
}

adlb_code ADLB_Create_ref(adlb_datum_id id, adlb_create_props props,
	adlb_datum_id *new_id) {
	return ADLBX_Create_ref(id, props, new_id);
}

adlb_code ADLB_Create_struct(adlb_datum_id id, adlb_create_props props,
	adlb_struct_type struct_type, adlb_datum_id *new_id) {
	return ADLBX_Create_struct(id, props, struct_type, new_id);
}

adlb_code ADLB_Create_container(adlb_datum_id id,
	adlb_data_type key_type,
	adlb_data_type val_type,
	adlb_create_props props,
	adlb_datum_id *new_id) {
	return ADLBX_Create_container(id, key_type, val_type, props, new_id);
}

adlb_code ADLB_Create_multiset(adlb_datum_id id,
	adlb_data_type val_type,
	adlb_create_props props,
	adlb_datum_id *new_id) {
	return ADLBX_Create_multiset(id, val_type, props, new_id);
}

adlb_code ADLB_Add_dsym(adlb_dsym symbol, adlb_dsym_data data) {
	return ADLBX_Add_dsym(symbol, data);
}

adlb_dsym_data ADLB_Dsym(adlb_dsym symbol) {
	return ADLBX_Dsym(symbol);
}

adlb_code ADLB_Exists(adlb_datum_id id, adlb_subscript subscript, bool* result,
	adlb_refc decr) {
	return ADLBX_Exists(id, subscript, result, decr);
}

adlb_code ADLB_Refcount_get(adlb_datum_id id, adlb_refc *result,
	adlb_refc decr) {
	return ADLBX_Refcount_get(id, result, decr);
}

adlb_code ADLB_Store(adlb_datum_id id, adlb_subscript subscript,
	adlb_data_type type, const void *data, size_t length,
	adlb_refc refcount_decr, adlb_refc store_refcounts) {
	return ADLBX_Store(id, subscript, type, data, length, refcount_decr,
		store_refcounts);
}

adlb_code ADLB_Retrieve(adlb_datum_id id, adlb_subscript subscript,
	adlb_retrieve_refc refcounts, adlb_data_type* type,
	void* data, size_t* length) {
	return ADLBX_Retrieve(id, subscript, refcounts, type, data, length);
}

adlb_code ADLB_Enumerate(adlb_datum_id container_id,
	int count, int offset, adlb_refc decr,
	bool include_keys, bool include_vals,
	void** data, size_t* length, int* records,
	adlb_type_extra *kv_type) {
	return ADLBX_Enumerate(container_id, count, offset, decr, include_keys,
		include_vals, data, length, records, kv_type);
}

adlb_code ADLB_Read_refcount_enable(void) {
	return ADLBX_Read_refcount_enable();
}

adlb_code ADLB_Refcount_incr(adlb_datum_id id, adlb_refc change) {
	return ADLBX_Refcount_incr(id, change);
}

adlb_code ADLB_Insert_atomic(adlb_datum_id id, adlb_subscript subscript,
	adlb_retrieve_refc refcounts,
	bool *result, bool *value_present,
	void *data, size_t *length, adlb_data_type *type) {
	return ADLBX_Insert_atomic(id, subscript, refcounts, result,
		value_present, data, length, type);
}

adlb_code ADLB_Subscribe(adlb_datum_id id, adlb_subscript subscript,
	int work_type, int* subscribed) {
	return ADLBX_Subscribe(id, subscript, work_type, subscribed);
}

adlb_code ADLB_Container_reference(adlb_datum_id id, adlb_subscript subscript,
	adlb_datum_id ref_id, adlb_subscript ref_subscript,
	adlb_data_type ref_type, adlb_refc transfer_refs,
	int ref_write_decr) {
	return ADLBX_Container_reference(id, subscript, ref_id, ref_subscript,
		ref_type, transfer_refs, ref_write_decr);
}

adlb_code ADLB_Unique(adlb_datum_id *result) {
	return ADLBX_Unique(result);
}

adlb_code ADLB_Alloc_global(int count, adlb_datum_id *start) {
	return ADLBX_Alloc_global(count, start);
}

adlb_code ADLB_Typeof(adlb_datum_id id, adlb_data_type* type) {
	return ADLBX_Typeof(id, type);
}

adlb_code ADLB_Container_typeof(adlb_datum_id id, adlb_data_type* key_type,
	adlb_data_type* val_type) {
	return ADLBX_Container_typeof(id, key_type, val_type);
}

adlb_code ADLB_Container_size(adlb_datum_id container_id, int* size,
	adlb_refc decr) {
	return ADLBX_Container_size(container_id, size, decr);
}

adlb_code ADLB_Lock(adlb_datum_id id, bool* result) {
	return ADLBX_Lock(id, result);
}

adlb_code ADLB_Unlock(adlb_datum_id id) {
	return ADLBX_Unlock(id);
}

adlb_code ADLB_Finalize(void) {
	adlb_code o = ADLBX_Finalize();
	MPI_Comm_free(&comm);
	MPI_Finalize();
	return o;
}

adlb_code ADLB_Fail(int code) {
	return ADLBX_Fail(code);
}

void ADLB_Abort(int code) {
	ADLBX_Abort(code);
}

#include "adlb-x_types.h"

adlb_data_code ADLB_Declare_struct_type(adlb_struct_type type,
	const char *type_name,
	int field_count,
	const adlb_struct_field_type *field_types,
	const char **field_names) {
	return ADLBX_Declare_struct_type(type, type_name, field_count,
		field_types, field_names);
}

adlb_data_code ADLB_Lookup_struct_type(adlb_struct_type type,
	const char **type_name, int *field_count,
	const adlb_struct_field_type **field_types,
	char const* const** field_names) {
	return ADLBX_Lookup_struct_type(type, type_name, field_count, field_types,
		field_names);
}

adlb_data_code ADLB_Pack(const adlb_datum_storage *d, adlb_data_type type,
	const adlb_buffer *caller_buffer,
	adlb_binary_data *result) {
	return ADLBX_Pack(d, type, caller_buffer, result);
}

adlb_data_code ADLB_Append_buffer(adlb_data_type type, const void *data,
	size_t length, bool prefix_len, adlb_buffer *output,
	bool *output_caller_buffer, size_t *output_pos) {
	return ADLBX_Append_buffer(type, data, length, prefix_len, output,
		output_caller_buffer, output_pos);
}

adlb_data_code ADLB_Pack_buffer(const adlb_datum_storage *d, adlb_data_type type,
	bool prefix_len, const adlb_buffer *tmp_buf, adlb_buffer *output,
	bool *output_caller_buffer, size_t *output_pos) {
	return ADLBX_Pack_buffer(d, type, prefix_len, tmp_buf, output,
		output_caller_buffer, output_pos);
}

bool ADLB_pack_pad_size(adlb_data_type type) {
	return ADLBX_pack_pad_size(type);
}

adlb_data_code ADLB_Unpack(adlb_datum_storage *d, adlb_data_type type,
	const void *buffer, size_t length, adlb_refc refcounts) {
	return ADLBX_Unpack(d, type, buffer, length, refcounts);
}

adlb_data_code ADLB_Unpack2(adlb_datum_storage *d, adlb_data_type type,
	void *buffer, size_t length, bool copy_buffer,
	adlb_refc refcounts, bool init_compound, bool *took_ownership) {
	return ADLBX_Unpack2(d, type, buffer, length, copy_buffer,
		refcounts, init_compound, took_ownership);
}

adlb_data_code ADLB_Unpack_buffer(adlb_data_type type,
	const void *buffer, size_t length, size_t *pos,
	const void **entry, size_t* entry_length) {
	return ADLBX_Unpack_buffer(type, buffer, length, pos, entry,
		entry_length);
}

adlb_data_code ADLB_Init_compound(adlb_datum_storage *d, adlb_data_type type,
	adlb_type_extra type_extra, bool must_init) {
	return ADLBX_Init_compound(d, type, type_extra, must_init);
}

adlb_data_code ADLB_Pack_container(const adlb_container *container,
	const adlb_buffer *tmp_buf, adlb_buffer *output,
	bool *output_caller_buffer, size_t *output_pos) {
	return ADLBX_Pack_container(container, tmp_buf, output,
		output_caller_buffer, output_pos);
}

adlb_data_code ADLB_Pack_container_hdr(int elems, adlb_data_type key_type,
	adlb_data_type val_type, adlb_buffer *output,
	bool *output_caller_buffer, size_t *output_pos) {
	return ADLBX_Pack_container_hdr(elems, key_type, val_type, output,
		output_caller_buffer, output_pos);
}

adlb_data_code ADLB_Unpack_container(adlb_container *container, const void *data,
	size_t length, adlb_refc refcounts, bool init_cont) {
	return ADLBX_Unpack_container(container, data, length, refcounts,
		init_cont);
}

adlb_data_code ADLB_Unpack_container_hdr(const void *data, size_t length,
	size_t *pos, int *entries, adlb_data_type *key_type,
	adlb_data_type *val_type) {
	return ADLBX_Unpack_container_hdr(data, length, pos, entries, key_type,
		val_type);
}

adlb_data_code ADLB_Unpack_container_entry(adlb_data_type key_type,
	adlb_data_type val_type, const void *data, size_t length,
	size_t *pos, const void **key, size_t *key_len,
	const void **val, size_t *val_len) {
	return ADLBX_Unpack_container_entry(key_type, val_type, data, length,
		pos, key, key_len, val, val_len);
}

adlb_data_code ADLB_Pack_multiset(const adlb_multiset_ptr ms,
	const adlb_buffer *tmp_buf, adlb_buffer *output,
	bool *output_caller_buffer, size_t *output_pos) {
	return ADLBX_Pack_multiset(ms, tmp_buf, output, output_caller_buffer,
		output_pos);
}

adlb_data_code ADLB_Pack_multiset_hdr(int elems, adlb_data_type elem_type,
	adlb_buffer *output, bool *output_caller_buffer, size_t *output_pos) {
	return ADLBX_Pack_multiset_hdr(elems, elem_type, output,
		output_caller_buffer, output_pos);
}

adlb_data_code ADLB_Unpack_multiset(adlb_multiset_ptr *ms, const void *data,
	size_t length, adlb_refc refcounts, bool init_ms) {
	return ADLBX_Unpack_multiset(ms, data, length, refcounts, init_ms);
}

adlb_data_code ADLB_Unpack_multiset_hdr(const void *data, size_t length,
	size_t *pos, int *entries, adlb_data_type *elem_type) {
	return ADLBX_Unpack_multiset_hdr(data, length, pos, entries, elem_type);
}

adlb_data_code ADLB_Unpack_multiset_entry(adlb_data_type elem_type,
	const void *data, size_t length, size_t *pos,
	const void **elem, size_t *elem_len) {
	return ADLBX_Unpack_multiset_entry(elem_type, data, length, pos, elem,
		elem_len);
}

adlb_data_code ADLB_Pack_struct(const adlb_struct *s,
	const adlb_buffer *caller_buffer, adlb_binary_data *result) {
	return ADLBX_Pack_struct(s, caller_buffer, result);
}

adlb_data_code ADLB_Unpack_struct(adlb_struct **s, const void *data, size_t length,
	adlb_refc refcounts, bool init_struct) {
	return ADLBX_Unpack_struct(s, data, length, refcounts, init_struct);
}

adlb_data_code ADLB_Free_storage(adlb_datum_storage *d, adlb_data_type type) {
	return ADLBX_Free_storage(d, type);
}

adlb_data_code ADLB_Int64_parse(const char *str, size_t length, int64_t *result) {
	return ADLBX_Int64_parse(str, length, result);
}

char* ADLB_Data_repr(const adlb_datum_storage *d, adlb_data_type type) {
	return ADLBX_Data_repr(d, type);
}
