#include <stdio.h>
#include "adlb.h"
#include "adlb_types.h"
#include <stdlib.h>
#include <unistd.h>

#include "xtask_api.h"
static int id, servers, workers;

static pid_t pid;
#define print(fmt, ...) printf("[%d] " fmt, pid, ## __VA_ARGS__)

struct dec_entry {
	char* name;
	int count;
	adlb_struct_field_type* fields;
	char** names;
};
static struct dec_entry* structs;
static int numstructs;

static int used_dids;

void ADLB_Init_comm() {
	workers = 3;	// Hardcoded for now
	id = xtask_setup(64, workers);
	pid = getpid();

	servers = 1;	// We assume one server. Hopefully it works out....
	structs = NULL;
	used_dids = 0;
}

adlb_code ADLB_Init(int nservers, int ntypes, int type_vect[], int *am_server) {
	// Servers are workers with a low enough id.
	if(id < servers) *am_server = 1;
	else *am_server = 0;

	xlb_data_types_init();
	return ADLB_SUCCESS;
}

adlb_code ADLB_Server(long max_memory) {
	print("STUB Server!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Version(version* output) {
	output->major = 0;
	output->minor = 9;
	output->revision = 0;
	return ADLB_SUCCESS;
}

int ADLB_Get_rank() {
	print("Get_rank: %d %d\n", id, id-servers);
	return id - servers;
}

int ADLB_Get_size() {
	return workers;
}

int ADLB_GetRank_workers() {
	return id - servers;
}

int ADLB_Is_leader() {
	print("STUB Is_leader!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Put(const void* payload, int length, adlb_put_opts opts) {
	print("STUB Put!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Dput(const void* payload, int length, adlb_put_opts opts,
	const char *name, const adlb_datum_id *wait_ids, int wait_id_count,
	const adlb_datum_id_sub *wait_id_subs, int wait_id_sub_count) {

	print("Ignoring Dput, waiting on ");
	for(int i=0; i<wait_id_count; i++) printf("%ld,", wait_ids[i]);
	printf(" and ");
	for(int i=0; i<wait_id_sub_count; i++)
		printf("%ld[*],", wait_id_subs[i].id);
	printf(".\n");

	return ADLB_SUCCESS;
}

adlb_code ADLB_Get(void** payload, int* length, int max_length) {
	print("STUB Get!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Iget(void* payload, int* length) {
	print("STUB Iget!\n");
	return ADLB_SUCCESS;
}

int ADLB_Locate(adlb_datum_id id) {
	print("STUB Locate!\n");
	return 0;
}

adlb_code ADLB_Create(adlb_datum_id id, adlb_data_type type,
	adlb_type_extra type_extra,
	adlb_create_props props, adlb_datum_id *new_id) {
	print("STUB Create!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Multicreate(ADLB_create_spec *specs, int count) {
	print("Ignoring Multicreate:\n");
	for(int i=0; i<count; i++) {
		specs[i].id = used_dids++;
		print("\tid=%ld, type=%d\n", specs[i].id, specs[i].type);
		if(specs[i].type_extra.valid) {
			if(specs[i].type == ADLB_DATA_TYPE_CONTAINER)
				print("\t\textra: key=%d, val=%d\n",
					specs[i].type_extra.CONTAINER.key_type,
					specs[i].type_extra.CONTAINER.val_type);
			if(specs[i].type == ADLB_DATA_TYPE_MULTISET)
				print("\t\textra: val=%d\n",
					specs[i].type_extra.MULTISET.val_type);
			if(specs[i].type == ADLB_DATA_TYPE_STRUCT)
				print("\t\textra: struct_type=%d\n",
					specs[i].type_extra.STRUCT.struct_type);
		}
	}
	return ADLB_SUCCESS;
}


adlb_code ADLB_Create_integer(adlb_datum_id id, adlb_create_props props,
	adlb_datum_id *new_id) {
	print("STUB Create_integer!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Create_float(adlb_datum_id id, adlb_create_props props,
	adlb_datum_id *new_id) {
	print("STUB Create_float!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Create_string(adlb_datum_id id, adlb_create_props props,
	adlb_datum_id *new_id) {
	print("STUB Create_string!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Create_blob(adlb_datum_id id, adlb_create_props props,
	adlb_datum_id *new_id) {
	print("STUB Create_blob!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Create_ref(adlb_datum_id id, adlb_create_props props,
	adlb_datum_id *new_id) {
	print("STUB Create_ref!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Create_struct(adlb_datum_id id, adlb_create_props props,
	adlb_struct_type struct_type, adlb_datum_id *new_id) {
	print("STUB Create_struct!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Create_container(adlb_datum_id id,
	adlb_data_type key_type,
	adlb_data_type val_type,
	adlb_create_props props,
	adlb_datum_id *new_id) {
	print("STUB Create_container!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Create_multiset(adlb_datum_id id,
	adlb_data_type val_type,
	adlb_create_props props,
	adlb_datum_id *new_id) {
	print("STUB Create_multiset!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Add_dsym(adlb_dsym symbol, adlb_dsym_data data) {
	print("STUB Add_dsym!\n");
	return ADLB_SUCCESS;
}

adlb_dsym_data ADLB_Dsym(adlb_dsym symbol) {
	print("STUB Dsym!\n");
	return (adlb_dsym_data){"STUB", "stub"};
}

adlb_code ADLB_Exists(adlb_datum_id id, adlb_subscript subscript, bool* result,
	adlb_refc decr) {
	print("STUB Exists!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Refcount_get(adlb_datum_id id, adlb_refc *result,
	adlb_refc decr) {
	print("STUB Refcount_get!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Store(adlb_datum_id id, adlb_subscript subscript,
	adlb_data_type type, const void *data, size_t length,
	adlb_refc refcount_decr, adlb_refc store_refcounts) {
	print("STUB Store!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Retrieve(adlb_datum_id id, adlb_subscript subscript,
	adlb_retrieve_refc refcounts, adlb_data_type* type,
	void* data, size_t* length) {
	print("STUB Retrieve!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Enumerate(adlb_datum_id container_id,
	int count, int offset, adlb_refc decr,
	bool include_keys, bool include_vals,
	void** data, size_t* length, int* records,
	adlb_type_extra *kv_type) {
	print("STUB Enumerate!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Read_refcount_enable(void) {
	print("STUB Read_refcount_enable!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Refcount_incr(adlb_datum_id id, adlb_refc change) {
	print("STUB Refcount_incr!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Insert_atomic(adlb_datum_id id, adlb_subscript subscript,
	adlb_retrieve_refc refcounts,
	bool *result, bool *value_present,
	void *data, size_t *length, adlb_data_type *type) {
	print("STUB Insert_atomic!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Subscribe(adlb_datum_id id, adlb_subscript subscript,
	int work_type, int* subscribed) {
	print("STUB Subscribe!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Container_reference(adlb_datum_id id, adlb_subscript subscript,
	adlb_datum_id ref_id, adlb_subscript ref_subscript,
	adlb_data_type ref_type, adlb_refc transfer_refs,
	int ref_write_decr) {
	print("STUB Container_reference!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Unique(adlb_datum_id *result) {
	print("STUB Unique!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Alloc_global(int count, adlb_datum_id *start) {
	print("STUB Alloc_global!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Typeof(adlb_datum_id id, adlb_data_type* type) {
	print("STUB Typeof!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Container_typeof(adlb_datum_id id, adlb_data_type* key_type,
	adlb_data_type* val_type) {
	print("STUB Container_typeof!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Container_size(adlb_datum_id container_id, int* size,
	adlb_refc decr) {
	print("STUB Container_size!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Lock(adlb_datum_id id, bool* result) {
	print("STUB Lock!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Unlock(adlb_datum_id id) {
	print("STUB Unlock!\n");
	return ADLB_SUCCESS;
}

adlb_code ADLB_Finalize(void) {
	xtask_cleanup();
	return ADLB_SUCCESS;
}

adlb_code ADLB_Fail(int code) {
	print("STUB Fail!\n");
	return ADLB_SUCCESS;
}

void ADLB_Abort(int code) {
	print("STUB Abort!\n");
}

#include "adlb_types.h"

adlb_data_code ADLB_Declare_struct_type(adlb_struct_type type,
	const char *type_name,
	int field_count,
	const adlb_struct_field_type *field_types,
	const char **field_names) {

	if(numstructs <= type) {
		numstructs = type+1;
		structs = realloc(structs, numstructs*sizeof(struct dec_entry));
	}

	structs[type] = (struct dec_entry){
		.name = strdup(type_name),
		.count = field_count,
		.fields = malloc(field_count*sizeof(adlb_struct_field_type)),
		.names = malloc(field_count*sizeof(char*)),
	};
	for(int i=0; i<field_count; i++) {
		structs[type].fields[i] = field_types[i];
		structs[type].names[i] = strdup(field_names[i]);
	}

	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Lookup_struct_type(adlb_struct_type type,
	const char **type_name, int *field_count,
	const adlb_struct_field_type **field_types,
	char const* const** field_names) {

	if(type < numstructs) {
		if(type_name) *type_name = structs[type].name;
		if(field_count) *field_count = structs[type].count;
		if(field_types) *field_types = structs[type].fields;
		if(field_names) *field_names = structs[type].names;
		return ADLB_DATA_SUCCESS;
	} else return ADLB_DATA_ERROR_UNKNOWN;
}

adlb_data_code ADLB_Pack(const adlb_datum_storage *d, adlb_data_type type,
	const adlb_buffer *caller_buffer,
	adlb_binary_data *result) {
	print("STUB Pack!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Append_buffer(adlb_data_type type, const void *data,
	size_t length, bool prefix_len, adlb_buffer *output,
	bool *output_caller_buffer, size_t *output_pos) {
	print("STUB Append_buffer!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Pack_buffer(const adlb_datum_storage *d, adlb_data_type type,
	bool prefix_len, const adlb_buffer *tmp_buf, adlb_buffer *output,
	bool *output_caller_buffer, size_t *output_pos) {
	print("STUB Pack_buffer!\n");
	return ADLB_DATA_SUCCESS;
}

bool ADLB_pack_pad_size(adlb_data_type type) {
	print("STUB pack_pad_size!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Unpack(adlb_datum_storage *d, adlb_data_type type,
	const void *buffer, size_t length, adlb_refc refcounts) {
	print("STUB Unpack!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Unpack2(adlb_datum_storage *d, adlb_data_type type,
	void *buffer, size_t length, bool copy_buffer,
	adlb_refc refcounts, bool init_compound, bool *took_ownership) {
	print("STUB Unpack2!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Unpack_buffer(adlb_data_type type,
	const void *buffer, size_t length, size_t *pos,
	const void **entry, size_t* entry_length) {
	print("STUB Unpack_buffer!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Init_compound(adlb_datum_storage *d, adlb_data_type type,
	adlb_type_extra type_extra, bool must_init) {
	print("STUB Init_compound!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Pack_container(const adlb_container *container,
	const adlb_buffer *tmp_buf, adlb_buffer *output,
	bool *output_caller_buffer, size_t *output_pos) {
	print("STUB Pack_container!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Pack_container_hdr(int elems, adlb_data_type key_type,
	adlb_data_type val_type, adlb_buffer *output,
	bool *output_caller_buffer, size_t *output_pos) {
	print("STUB Pack_container_hdr!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Unpack_container(adlb_container *container, const void *data,
	size_t length, adlb_refc refcounts, bool init_cont) {
	print("STUB Unpack_container!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Unpack_container_hdr(const void *data, size_t length,
	size_t *pos, int *entries, adlb_data_type *key_type,
	adlb_data_type *val_type) {
	print("STUB Unpack_container_hdr!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Unpack_container_entry(adlb_data_type key_type,
	adlb_data_type val_type, const void *data, size_t length,
	size_t *pos, const void **key, size_t *key_len,
	const void **val, size_t *val_len) {
	print("STUB Unpack_container_entry!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Pack_multiset(const adlb_multiset_ptr ms,
	const adlb_buffer *tmp_buf, adlb_buffer *output,
	bool *output_caller_buffer, size_t *output_pos) {
	print("STUB Pack_multiset!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Pack_multiset_hdr(int elems, adlb_data_type elem_type,
	adlb_buffer *output, bool *output_caller_buffer, size_t *output_pos) {
	print("STUB Pack_multiset_hdr!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Unpack_multiset(adlb_multiset_ptr *ms, const void *data,
	size_t length, adlb_refc refcounts, bool init_ms) {
	print("STUB Unpack_multiset!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Unpack_multiset_hdr(const void *data, size_t length,
	size_t *pos, int *entries, adlb_data_type *elem_type) {
	print("STUB Unpack_multiset_hdr!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Unpack_multiset_entry(adlb_data_type elem_type,
	const void *data, size_t length, size_t *pos,
	const void **elem, size_t *elem_len) {
	print("STUB Unpack_multiset_entry!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Pack_struct(const adlb_struct *s,
	const adlb_buffer *caller_buffer, adlb_binary_data *result) {
	print("STUB Pack_struct!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Unpack_struct(adlb_struct **s, const void *data, size_t length,
	adlb_refc refcounts, bool init_struct) {
	print("STUB Unpack_struct!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Free_storage(adlb_datum_storage *d, adlb_data_type type) {
	print("STUB Free_storage!\n");
	return ADLB_DATA_SUCCESS;
}

adlb_data_code ADLB_Int64_parse(const char *str, size_t length, int64_t *result) {
	print("STUB Int64_parse!\n");
	return ADLB_DATA_SUCCESS;
}

char* ADLB_Data_repr(const adlb_datum_storage *d, adlb_data_type type) {
	print("STUB Data_repr!\n");
	return ADLB_DATA_SUCCESS;
}
