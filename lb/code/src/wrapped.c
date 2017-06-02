#include "adlb-x.h"
#include <stdio.h>

#define DEBUG 0

static MPI_Comm comm;
static MPI_Comm work_comm;

/*
   These are the functions available to ADLB application code
 */
MPI_Comm ADLB_Init_comm() {
	int argc = 0;
	char** argv = NULL;
	MPI_Init(&argc, &argv);
	//assert(rc == MPI_SUCCESS);	// Just hope it works...
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	return comm;
}

adlb_code ADLB_Init(int nservers, int ntypes, int type_vect[],
                    int *am_server) {
	adlb_code r = ADLBX_Init(nservers, ntypes, type_vect,
		am_server, comm, &work_comm);
#if DEBUG
	printf("Init %x %x\n", adlb_comm, *worker_comm);
#endif
	return r;
}

adlb_code ADLB_Server(long max_memory) {
#if DEBUG
	printf("Server\n");
#endif
	return ADLBX_Server(max_memory);
}

adlb_code ADLB_Version(version* output) {
#if DEBUG
	printf("Version\n");
#endif
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

MPI_Comm ADLB_GetComm_workers(void) {
	MPI_Comm c = ADLBX_GetComm_workers();
#if DEBUG
	printf("GetComm_workers %x\n", c);
#endif
	return c;
}

MPI_Comm ADLB_GetComm_leaders(void) {
	MPI_Comm c = ADLBX_GetComm_leaders();
#if DEBUG
	printf("GetComm_leaders %x\n", c);
#endif
	return c;
}

adlb_code ADLB_Hostmap_stats(unsigned int* count,
                             unsigned int* name_max) {
#if DEBUG
	printf("Hostmap_stats\n");
#endif
	return ADLBX_Hostmap_stats(count, name_max);
}

adlb_code ADLB_Hostmap_lookup(const char* name, int count,
                              int* output, int* actual) {
#if DEBUG
	printf("Hostmap_lookup\n");
#endif
	return ADLBX_Hostmap_lookup(name, count, output, actual);
}

/**
   Obtain RS-separated buffer of host names
   @param output: OUT Buffer into which to write result
   @param max: Maximal number of characters to write
   @param offset: start with this hostname
   @param actual: out number of hostnames written
 */
adlb_code ADLB_Hostmap_list(char* output, unsigned int max,
                            unsigned int offset, int* actual) {
#if DEBUG
	printf("Hostmap_list\n");
#endif
	return ADLBX_Hostmap_list(output, max, offset, actual);
}

/*
  Put a task into the global task queue.

  @param payload: data buffer containing task data
  @param length: length of the payload in bytes
  @param target: target rank for task, adlb_rank_any if any target
  @param answer: answer rank passed to receiver of task
  @param type: task type
  @param priority: priority of task
  @param parallelism: number of ranks to execute task
              (1 for serial tasks, > 1 for parallel tasks)
  @param opts: additional options
 */
adlb_code ADLB_Put(const void* payload, int length, int target, int answer,
                   int type, adlb_put_opts opts) {
#if DEBUG
	printf("Put\n");
#endif
	return ADLBX_Put(payload, length, target, answer, type, opts);
}

/*
  Put a data-dependent task into the global task queue.  The task will
  be released and eligible to be matched to an ADLB_Get call once all
  specified ids reach write refcount 0, and all specified id/subscript
  pairs are assigned.  Most parameters are identical to adlb_put, except:
  @param wait_ids: array of ids to wait for
  @param wait_id_count: length of wait_ids array
  @param wait_id_subs: array of id/subscript pairs to wait for
  @param wait_id_sub_count: length of wait_id_subs array
 */
adlb_code ADLB_Dput(const void* payload, int length, int target,
        int answer, int type, adlb_put_opts opts, const char *name,
        const adlb_datum_id *wait_ids, int wait_id_count, 
        const adlb_datum_id_sub *wait_id_subs, int wait_id_sub_count) {
#if DEBUG
	printf("Dput\n");
#endif
	return ADLBX_Dput(payload, length, target, answer, type, opts, name, wait_ids, wait_id_count, wait_id_subs, wait_id_sub_count);
}

/*
  Get a task from the global task queue.
  @param type_requested: the type of work requested
  @param payload IN/OUT Pointer into which to receive task
                        May be changed if too small, in which case
                        caller must free the new value
                        Caller should compare payload before and after
  @param length IN/OUT original initial/actual length of payload
  @param length IN Limit for allocating new payload
  @param answer OUT parameter for answer rank specified in ADLB_Put
                    for task
  @param type_recvd OUT parameter for actual type of task
  @param comm   OUT parameter for MPI communicator to use for
                executing parallel task
 */
adlb_code ADLB_Get(int type_requested, void** payload,
                   int* length, int max_length,
                   int* answer, int* type_recvd, MPI_Comm* comm) {
	MPI_Comm tmp;
	if(comm == NULL) comm = &tmp;
	adlb_code r = ADLBX_Get(type_requested, payload, length, max_length, answer, type_recvd, comm);
#if DEBUG
	printf("Get %x\n", *comm);
#endif
	return r;
}

/*
 Polling equivalent of ADLB_Get.  Returns ADLB_NOTHING if no
 matching task are available.  Other return codes are same as
 ADLB_Get

  NOTE: Iget does not currently support parallel tasks
*/
adlb_code ADLB_Iget(int type_requested, void* payload, int* length,
                    int* answer, int* type_recvd, MPI_Comm* comm) {
	adlb_code r = ADLBX_Iget(type_requested, payload, length, answer, type_recvd, comm);
#if DEBUG
	printf("Iget %x\n", *comm);
#endif
	return r;
}
/*
  Non-blocking equivalent of ADLB_Get.  Matching requests should be
  filled in in the order that they are posted (i.e. if work matches two
  requests from a client, the first request that was posted will be
  filled first.

  If a work unit doesn't fit in the posted buffer, a runtime error
  will occur.

  TODO: currently we assume that only requests for the same type
        will be issues concurrently
  payload: will be retained by ADLB until request is completed.
  req: handle used to check for completion, filled in by function
 */
adlb_code ADLB_Aget(int type_requested, adlb_payload_buf payload,
                     adlb_get_req *req) {
#if DEBUG
	printf("Aget\n");
#endif
	return ADLBX_Aget(type_requested, payload, req);
}

/*
  Same as ADLB_Aget except initiates multiple requests at once.
  
  nreqs: number of requests to initiate
  wait: wait for first request to be filled (or shutdown to occur),
       Return immediately if 0 requests.
       After returns, ADLB_Aget_wait will immediately succeed
       on first request returned.
  payloads: array of nreqs payload buffers
  reqs: array of nreqs requests, filled in with request handles
 */
adlb_code ADLB_Amget(int type_requested, int nreqs, bool wait,
                     const adlb_payload_buf* payloads,
                     adlb_get_req *reqs) {
#if DEBUG
	printf("Amget\n");
#endif
	return ADLBX_Amget(type_requested, nreqs, wait, payloads, reqs);
}

/*
  Test if a get request completed without blocking.

  Return codes match ADLB_Get
 */
adlb_code ADLB_Aget_test(adlb_get_req *req, int* length,
                    int* answer, int* type_recvd, MPI_Comm* comm) {
#if DEBUG
	printf("Aget_test\n");
#endif
	return ADLBX_Aget_test(req, length, answer, type_recvd, comm);
}

/*
  Wait until a get request completes.
  Return codes match ADLB_Get
 */
adlb_code ADLB_Aget_wait(adlb_get_req *req, int* length,
                    int* answer, int* type_recvd, MPI_Comm* comm) {
	adlb_code r = ADLBX_Aget_wait(req, length, answer, type_recvd, comm);
#if DEBUG
	printf("Aget_wait %x\n", *comm);
#endif
	return r;
}

/**
   Obtain server rank responsible for data id
 */
int ADLB_Locate(adlb_datum_id id) {
#if DEBUG
	printf("Locate\n");
#endif
	return ADLBX_Locate(id);
}

// Applications should not call these directly but
// should use the typed forms defined below
// Can be called locally on server where data resides
adlb_code ADLB_Create(adlb_datum_id id, adlb_data_type type,
                      adlb_type_extra type_extra,
                      adlb_create_props props, adlb_datum_id *new_id) {
#if DEBUG
	printf("Create\n");
#endif
	return ADLBX_Create(id, type, type_extra, props, new_id);
}

// Create multiple variables.
// Currently we assume that spec[i].id is ADLB_DATA_ID_NULL and
// will be filled in with a new id
// Can be called locally on server where data resides
adlb_code ADLB_Multicreate(ADLB_create_spec *specs, int count) {
#if DEBUG
	printf("Multicreate\n");
#endif
	return ADLBX_Multicreate(specs, count);
}


adlb_code ADLB_Create_integer(adlb_datum_id id, adlb_create_props props,
                              adlb_datum_id *new_id) {
#if DEBUG
	printf("Create_integer\n");
#endif
	return ADLBX_Create_integer(id, props, new_id);
}

adlb_code ADLB_Create_float(adlb_datum_id id, adlb_create_props props,
                              adlb_datum_id *new_id) {
#if DEBUG
	printf("Create_float\n");
#endif
	return ADLBX_Create_float(id, props, new_id);
}

adlb_code ADLB_Create_string(adlb_datum_id id, adlb_create_props props,
                              adlb_datum_id *new_id) {
#if DEBUG
	printf("Create_string\n");
#endif
	return ADLBX_Create_string(id, props, new_id);
}

adlb_code ADLB_Create_blob(adlb_datum_id id, adlb_create_props props,
                              adlb_datum_id *new_id) {
#if DEBUG
	printf("Create_blob\n");
#endif
	return ADLBX_Create_blob(id, props, new_id);
}

adlb_code ADLB_Create_ref(adlb_datum_id id, adlb_create_props props,
                              adlb_datum_id *new_id) {
#if DEBUG
	printf("Create_ref\n");
#endif
	return ADLBX_Create_ref(id, props, new_id);
}
/**
 * Struct type: specify struct type, or leave as ADLB_STRUCT_TYPE_NULL to
 *              resolve upon assigning typed struct value
 */
adlb_code ADLB_Create_struct(adlb_datum_id id, adlb_create_props props,
                             adlb_struct_type struct_type, adlb_datum_id *new_id) {
#if DEBUG
	printf("Create_struct\n");
#endif
	return ADLBX_Create_struct(id, props, struct_type, new_id);
}

adlb_code ADLB_Create_container(adlb_datum_id id,
                                adlb_data_type key_type, 
                                adlb_data_type val_type, 
                                adlb_create_props props,
                                adlb_datum_id *new_id) {
#if DEBUG
	printf("Create_container\n");
#endif
	return ADLBX_Create_container(id, key_type, val_type, props, new_id);
}

adlb_code ADLB_Create_multiset(adlb_datum_id id,
                                adlb_data_type val_type, 
                                adlb_create_props props,
                                adlb_datum_id *new_id) {
#if DEBUG
	printf("Create_multiset\n");
#endif
	return ADLBX_Create_multiset(id, val_type, props, new_id);
}
/*
  Add debug symbol entry, overwriting any existing entry.
  Only adds to local table (not on other ranks).
  
  symbol: debug symbol identifier, should not be ADLB_DSYM_NULL
  data: associated null-terminated data string, will be copied.
 */
adlb_code ADLB_Add_dsym(adlb_dsym symbol, adlb_dsym_data data) {
#if DEBUG
	printf("Add_dsym\n");
#endif
	return ADLBX_Add_dsym(symbol, data);
}

/*
  Retrieve debug symbol entry from local debug symbol table.
  
  symbol: a debug symbol identifier
  return: entry previous added for symbol, or NULL values if not present
 */
adlb_dsym_data ADLB_Dsym(adlb_dsym symbol) {
#if DEBUG
	printf("Dsym\n");
#endif
	return ADLBX_Dsym(symbol);
}

adlb_code ADLB_Exists(adlb_datum_id id, adlb_subscript subscript, bool* result,
                       adlb_refc decr) {
#if DEBUG
	printf("Exists\n");
#endif
	return ADLBX_Exists(id, subscript, result, decr);
}

/**
 * Find out the current reference counts for a datum.
 *
 * E.g. if you want to find out that the datum is closed with refcount 0
 * This will succeed with zero refcounts if datum isn't found.
 *
 * result: refcounts of id after decr applied
 * decr: amount to decrement refcounts
 */
adlb_code ADLB_Refcount_get(adlb_datum_id id, adlb_refc *result,
                              adlb_refc decr) {
#if DEBUG
	printf("Refcount_get\n");
#endif
	return ADLBX_Refcount_get(id, result, decr);
}

/*
  Store value into datum
  data: binary representation
  length: length of binary representation
  refcount_decr: refcounts to to decrement on this id
  store_refcounts: refcounts to include for any refs in this data
  returns: ADLB_SUCCESS if store succeeded
           ADLB_REJECTED if id/subscript already assigned and cannot be
                         overwritten
           ADLB_ERROR for other errors
 */
adlb_code ADLB_Store(adlb_datum_id id, adlb_subscript subscript,
          adlb_data_type type, const void *data, size_t length,
          adlb_refc refcount_decr, adlb_refc store_refcounts) {
#if DEBUG
	printf("Store\n");
#endif
	return ADLBX_Store(id, subscript, type, data, length, refcount_decr, store_refcounts);
}

/*
   Retrieve contents of datum.
    
   refcounts: specify how reference counts should be changed
      read_refcount: decrease read refcount of this datum
      incr_read_referand: increase read refcount of referands,
                to ensure they aren't cleaned up prematurely
   type: output arg for the type of the datum
   data: a buffer of at least size ADLB_DATA_MAX
   length: output arg for data size in bytes
 */
adlb_code ADLB_Retrieve(adlb_datum_id id, adlb_subscript subscript,
      adlb_retrieve_refc refcounts, adlb_data_type* type,
      void* data, size_t* length) {
#if DEBUG
	printf("Retrieve\n");
#endif
	return ADLBX_Retrieve(id, subscript, refcounts, type, data, length);
}

/*
   List contents of container
   
   data: binary encoded keys and values (if requested), with each member
          encoded as follows:
        - key length encoded with vint_encode()
        - key data without null terminator
        - value length encoded with vint_encode()
        - value data encoded with ADLB_Pack
   length: number of bytes of data
   records: number of elements returned
 */
adlb_code ADLB_Enumerate(adlb_datum_id container_id,
                   int count, int offset, adlb_refc decr,
                   bool include_keys, bool include_vals,
                   void** data, size_t* length, int* records,
                   adlb_type_extra *kv_type) {
#if DEBUG
	printf("Enumerate\n");
#endif
	return ADLBX_Enumerate(container_id, count, offset, decr, include_keys, include_vals, data, length, records, kv_type);
}

// Switch on read refcounting and memory management, which is off by default
adlb_code ADLB_Read_refcount_enable(void) {
#if DEBUG
	printf("Read_refcount_enable\n");
#endif
	return ADLBX_Read_refcount_enable();
}

adlb_code ADLB_Refcount_incr(adlb_datum_id id, adlb_refc change) {
#if DEBUG
	printf("Refcount_incr\n");
#endif
	return ADLBX_Refcount_incr(id, change);
}

/*
  Try to reserve an insert position in container
  result: true if could be created, false if already present/reserved
  value_present: true if value was present (not just reserved)
  data: optionally, if this is not NULL, return the existing value in
        this buffer of at least size ADLB_DATA_MAX
  length: length of existing value if found
  type: type of existing value
  refcounts: refcounts to apply.
        if data exists, apply all refcounts
        if placeholder exists, don't apply
        if nothing exists, don't apply
 */
adlb_code ADLB_Insert_atomic(adlb_datum_id id, adlb_subscript subscript,
                        adlb_retrieve_refc refcounts,
                        bool *result, bool *value_present,
                        void *data, size_t *length, adlb_data_type *type) {
#if DEBUG
	printf("Insert_atomic\n");
#endif
	return ADLBX_Insert_atomic(id, subscript, refcounts, result, value_present, data, length, type);
}

/*
  returns: ADLB_SUCCESS if datum found
       ADLB_NOTHING if datum not found (can indicate it was gced)
 */
adlb_code ADLB_Subscribe(adlb_datum_id id, adlb_subscript subscript,
                          int work_type, int* subscribed) {
#if DEBUG
	printf("Subscribe\n");
#endif
	return ADLBX_Subscribe(id, subscript, work_type, subscribed);
}

adlb_code ADLB_Container_reference(adlb_datum_id id, adlb_subscript subscript,
                adlb_datum_id ref_id, adlb_subscript ref_subscript,
                adlb_data_type ref_type, adlb_refc transfer_refs,
                int ref_write_decr) {
#if DEBUG
	printf("Container_reference\n");
#endif
	return ADLBX_Container_reference(id, subscript, ref_id, ref_subscript, ref_type, transfer_refs, ref_write_decr);
}

/*
 * Allocate a unique data ID
 */
adlb_code ADLB_Unique(adlb_datum_id *result) {
#if DEBUG
	printf("Unique\n");
#endif
	return ADLBX_Unique(result);
}

/*
 * Allocates a range of count data IDs ADLB ranks.  This is useful for
 * implementing, for example, globally shared variables.  The allocated
 * range is inclusive of start and end and will have count members, i.e.
 * is [start, start + count).
 *
 * Must be called collectively by all ranks of the ADLB communicator.
 * The same range will be returned on all ranks.  This will return
 * once all globals have been correctly set up.
 *
 * Start and end are arbitrarily selected by the function.
 * The allocated IDs will be negative, so will not conflict with any
 * user-defined IDs in the positive range or IDs allocated by ADLB_Unique
 * or a ADLB_Create or related call.  If the allocated range conflicts
 * with any IDs already created, this will return an error.
 */
adlb_code ADLB_Alloc_global(int count, adlb_datum_id *start) {
#if DEBUG
	printf("Alloc_global\n");
#endif
	return ADLBX_Alloc_global(count, start);
}

adlb_code ADLB_Typeof(adlb_datum_id id, adlb_data_type* type) {
#if DEBUG
	printf("Typeof\n");
#endif
	return ADLBX_Typeof(id, type);
}

adlb_code ADLB_Container_typeof(adlb_datum_id id, adlb_data_type* key_type,
                                 adlb_data_type* val_type) {
#if DEBUG
	printf("Container_typeof\n");
#endif
	return ADLBX_Container_typeof(id, key_type, val_type);
}

adlb_code ADLB_Container_size(adlb_datum_id container_id, int* size,
                              adlb_refc decr) {
#if DEBUG
	printf("Container_size\n");
#endif
	return ADLBX_Container_size(container_id, size, decr);
}

adlb_code ADLB_Lock(adlb_datum_id id, bool* result) {
#if DEBUG
	printf("Lock\n");
#endif
	return ADLBX_Lock(id, result);
}

adlb_code ADLB_Unlock(adlb_datum_id id) {
#if DEBUG
	printf("Unlock\n");
#endif
	return ADLBX_Unlock(id);
}

/**
  Get information about a type based on name.
  Returns error if not found
 */
adlb_code ADLB_Data_string_totype(const char* type_string,
              adlb_data_type* type, adlb_type_extra *extra) {
#if DEBUG
	printf("Data_string_totype\n");
#endif
	return ADLBX_Data_string_totype(type_string, type, extra);
}

const char *ADLB_Data_type_tostring(adlb_data_type type) {
#if DEBUG
	printf("Data_type_tostring\n");
#endif
	return ADLBX_Data_type_tostring(type);
}

/*
  Convert string to placement enum value.
  Case insensitive.
 */
adlb_code ADLB_string_to_placement(const char *string,
                           adlb_placement *placement) {
#if DEBUG
	printf("string_to_placement\n");
#endif
	return ADLBX_string_to_placement(string, placement);
}

adlb_code ADLB_Server_idle(int rank, int64_t check_attempt, bool* result,
                 int *request_counts, int *untargeted_work_counts) {
#if DEBUG
	printf("Server_idle\n");
#endif
	return ADLBX_Server_idle(rank, check_attempt, result, request_counts, untargeted_work_counts);
}

adlb_code ADLB_Finalize(void) {
#if DEBUG
	printf("Finalize\n");
#endif
	return ADLBX_Finalize();
}

/**
   Tell server to fail.
 */
adlb_code ADLB_Fail(int code) {
#if DEBUG
	printf("Fail\n");
#endif
	return ADLBX_Fail(code);
}

void ADLB_Abort(int code) {
#if DEBUG
	printf("Abort\n");
#endif
	ADLBX_Abort(code);
}
